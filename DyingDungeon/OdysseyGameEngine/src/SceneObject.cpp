#include "RenderManager.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "SceneObject.h"
#include <fstream>
#include "MeshRenderer.h"

namespace Odyssey
{
	SceneObject::SceneObject()
	{
		// Default state for scene objects is non-renderable without a parent or children
		mParent = nullptr;
		DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
	}

	

	void SceneObject::importModel(const char* filename)
	{
		// Open the mesh file
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Check if the file is open
		if (!file.is_open()) { return; }

		// Get the number of meshes
		uint64_t numMeshes;
		file.read((char*)&numMeshes, sizeof(uint64_t));

		// This object is at the identity by default
		DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());

		// Iterate over the number of meshes and child the meshes to this scene object
		for (int i = 0; i < numMeshes; i++)
		{
			// Make a child scene object
			std::shared_ptr<SceneObject> child = std::make_shared<SceneObject>();
			// Read in the mesh's world matrix
			file.read((char*)&child->mWorldMatrix, sizeof(DirectX::XMFLOAT4X4));
			// Import the mesh from the current spot in the file
			std::shared_ptr<Mesh> mesh = MeshManager::getInstance().importMesh(file);
			// Import the material from the current spot in the file
			std::shared_ptr<Material> material = MaterialManager::getInstance().importMaterial(file);
			// Create the mesh renderer for the child scene object
			child->addComponent<MeshRenderer>(child->mWorldMatrix, mesh, material);
			// Set the parent of the child and allow it to be rendered
			child->mParent = this;
			// Calculate the new AABB of the mesh after loading in the vertices
			child->attachAABB();
			child->getAABB()->calculateAABBFromMesh(DirectX::XMLoadFloat4x4(&child->mWorldMatrix), child->getComponent<MeshRenderer>()->getMesh()->getVertexList());
			children.push_back(child);
		}

		// Read in if a skeleton exists
		uint64_t hasSkeleton;
		file.read((char*)&hasSkeleton, sizeof(hasSkeleton));

		if (hasSkeleton)
		{
			// Create the skeleton
			uint64_t numjoints;
			file.read((char*)&numjoints, sizeof(numjoints));

			// Attach an animator
			addComponent<Animator>();

			// Create a skeleton for importing
			std::vector<Joint> skeleton(numjoints);

			// Iterate over each joint
			for (int i = 0; i < numjoints; i++)
			{
				// Read in the length of the name
				uint64_t nameLength = skeleton[i].name.size() + 1;
				// Write the length to the file
				file.read((char*)&nameLength, sizeof(nameLength));
				// Resize the joint's name to fit the name
				skeleton[i].name.resize(nameLength);
				// Write the name to the file
				file.read((char*)skeleton[i].name.c_str(), nameLength * sizeof(char));
				// Write the inverse bindpose
				file.read((char*)&skeleton[i].invBindposeTransform, sizeof(DirectX::XMFLOAT4X4));
				// Write the parent index
				file.read((char*)&skeleton[i].parentIndex, sizeof(skeleton[i].parentIndex));
			}

			// Send the skeleton to the animator if one exists
			if (mAnimator)
			{
				mAnimator->setSkeleton(skeleton);
			}
		}
		file.close();
	}

	void SceneObject::addPosition(float x, float y, float z)
	{
		// Set the world matrix to a translation matrix specified by x, y and z
		DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(x, y, z), DirectX::XMLoadFloat4x4(&mWorldMatrix)));
		DirectX::XMFLOAT4X4 globalTransform;

		if (mAnimator)
		{
			if (children.size() > 0)
			{
				children[0]->getGlobalTransform(globalTransform);
			}
			else
			{
				getGlobalTransform(globalTransform);
			}
			mAnimator->setWorldMatrix(globalTransform);
		}

		for (std::shared_ptr<SceneObject> child : children)
		{
			if (child->getComponent<MeshRenderer>())
			{
				child->getGlobalTransform(globalTransform);

				if (mAABB)
					mAABB->calculateAABBFromMesh(DirectX::XMLoadFloat4x4(&globalTransform), mMeshRenderer->getMesh()->getVertexList());
			}
		}
	}

	void SceneObject::setPosition(float x, float y, float z)
	{
		// Set the world matrix to a translation matrix specified by x, y and z
		DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixTranslation(x, y, z));
		DirectX::XMFLOAT4X4 globalTransform;

		if (mAnimator)
		{
			if (children.size() > 0)
			{
				children[0]->getGlobalTransform(globalTransform);
			}
			else
			{
				getGlobalTransform(globalTransform);
			}
			mAnimator->setWorldMatrix(globalTransform);
		}
		for (std::shared_ptr<SceneObject> child : children)
		{
			if (child->getComponent<MeshRenderer>())
			{
				child->getGlobalTransform(globalTransform);
				if (mAABB)
					mAABB->calculateAABBFromMesh(DirectX::XMLoadFloat4x4(&globalTransform), mMeshRenderer->getMesh()->getVertexList());
			}
		}
	}

	void SceneObject::setScale(float x, float y, float z)
	{
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(x, y, z);
		scale = DirectX::XMMatrixMultiply(scale, DirectX::XMLoadFloat4x4(&mWorldMatrix));
		DirectX::XMStoreFloat4x4(&mWorldMatrix, scale);

		DirectX::XMFLOAT4X4 globalTransform;

		if (mAnimator)
		{
			if (children.size() > 0)
			{
				children[0]->getGlobalTransform(globalTransform);
			}
			else
			{
				getGlobalTransform(globalTransform);
			}
			mAnimator->setWorldMatrix(globalTransform);
		}

		for (std::shared_ptr<SceneObject> child : children)
		{
			if (child->getAABB())
			{
				child->getGlobalTransform(globalTransform);
				child->mAABB->calculateAABBFromMesh(DirectX::XMLoadFloat4x4(&globalTransform), child->getComponent<MeshRenderer>()->getMesh()->getVertexList());
			}
		}
	}

	void SceneObject::attachParticleSystem()
	{
		if (mParticleSystem == nullptr)
		{
			mParticleSystem = std::make_unique<ParticleSystem>();
		}
	}

	void SceneObject::attachAABB()
	{
		if (mAABB == nullptr)
		{
			mAABB = std::make_unique<AABB>();
		}
	}


	ParticleSystem* SceneObject::getParticleSystem()
	{
		return mParticleSystem.get();
	}

	ParticleSystem* SceneObject::getRootParticleSystem()
	{
		SceneObject* parent = mParent;

		while (parent->mParent != nullptr)
		{
			parent = parent->mParent;
		}

		return parent->getParticleSystem();
	}

	AABB* SceneObject::getAABB()
	{
		return mAABB.get();
	}

	bool SceneObject::hasParticleSystem()
	{
		return (mParticleSystem != nullptr);
	}

	bool SceneObject::hasAABB()
	{
		return mAABB != nullptr;
	}

	void SceneObject::getLocalTransform(DirectX::XMFLOAT4X4& localTransform)
	{
		localTransform = mWorldMatrix;
	}

	void SceneObject::getGlobalTransform(DirectX::XMFLOAT4X4& globalTransform)
	{
		DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4(&mWorldMatrix);

		SceneObject* parent = mParent;

		while (parent != nullptr)
		{
			transform = DirectX::XMMatrixMultiply(transform, DirectX::XMLoadFloat4x4(&parent->mWorldMatrix));
			parent = parent->mParent;
		}

		DirectX::XMStoreFloat4x4(&globalTransform, transform);
	}

	const std::vector<std::shared_ptr<SceneObject>> SceneObject::getChildren()
	{
		return children;
	}

	const int SceneObject::getChildrenCount()
	{
		return static_cast<int>(children.size());
	}

	void SceneObject::enableDebug()
	{
		mDebugEnabled = true;
	}

	void SceneObject::disableDebug()
	{
		mDebugEnabled = false;
	}

	bool SceneObject::getDebugEnabled()
	{
		return mDebugEnabled;
	}
}