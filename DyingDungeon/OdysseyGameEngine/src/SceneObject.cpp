#include "RenderManager.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "GameObject.h"
#include <fstream>
#include "MeshRenderer.h"
#include "Transform.h"

namespace Odyssey
{
	GameObject::GameObject()
	{
		// Default state for scene objects is non-renderable without a parent or children
		mParent = nullptr;
	}

	void GameObject::importModel(const char* filename)
	{
		// Open the mesh file
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Check if the file is open
		if (!file.is_open()) { return; }

		// Get the number of meshes
		uint64_t numMeshes;
		file.read((char*)&numMeshes, sizeof(uint64_t));

		// Iterate over the number of meshes and child the meshes to this scene object
		for (int i = 0; i < numMeshes; i++)
		{
			// Make a child scene object
			std::shared_ptr<GameObject> child = std::make_shared<GameObject>();
			// Read in the mesh's world matrix
			DirectX::XMFLOAT4X4 world;
			file.read((char*)&world, sizeof(DirectX::XMFLOAT4X4));
			child->addComponent<Transform>(world);
			// Import the mesh from the current spot in the file
			std::shared_ptr<Mesh> mesh = MeshManager::getInstance().importMesh(file);
			// Import the material from the current spot in the file
			std::shared_ptr<Material> material = MaterialManager::getInstance().importMaterial(file);
			// Create the mesh renderer for the child scene object
			child->addComponent<MeshRenderer>(world, mesh, material);
			// Set the parent of the child and allow it to be rendered
			child->mParent = this;
			// Calculate the new AABB of the mesh after loading in the vertices
			child->attachAABB();
			child->getAABB()->calculateAABBFromMesh(DirectX::XMLoadFloat4x4(&world), child->getComponent<MeshRenderer>()->getMesh()->getVertexList());
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

	void GameObject::attachParticleSystem()
	{
		if (mParticleSystem == nullptr)
		{
			mParticleSystem = std::make_unique<ParticleSystem>();
		}
	}

	void GameObject::attachAABB()
	{
		if (mAABB == nullptr)
		{
			mAABB = std::make_unique<AABB>();
		}
	}


	ParticleSystem* GameObject::getParticleSystem()
	{
		return mParticleSystem.get();
	}

	ParticleSystem* GameObject::getRootParticleSystem()
	{
		GameObject* parent = mParent;

		while (parent->mParent != nullptr)
		{
			parent = parent->mParent;
		}

		return parent->getParticleSystem();
	}

	AABB* GameObject::getAABB()
	{
		return mAABB.get();
	}

	bool GameObject::hasParticleSystem()
	{
		return (mParticleSystem != nullptr);
	}

	bool GameObject::hasAABB()
	{
		return mAABB != nullptr;
	}

	const std::vector<std::shared_ptr<GameObject>> GameObject::getChildren()
	{
		return children;
	}

	const int GameObject::getChildrenCount()
	{
		return static_cast<int>(children.size());
	}

	GameObject* GameObject::getParent()
	{
		return mParent;
	}

	void GameObject::enableDebug()
	{
		mDebugEnabled = true;
	}

	void GameObject::disableDebug()
	{
		mDebugEnabled = false;
	}

	bool GameObject::getDebugEnabled()
	{
		return mDebugEnabled;
	}
}