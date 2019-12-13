#include "GameObject.h"
#include "FileManager.h"
#include <fstream>
#include "Material.h"
#include "Transform.h"
#include "Mesh.h"
#include "AABB.h"
#include "RenderDevice.h"
#include "Animator.h"

namespace Odyssey
{
	FileManager& FileManager::getInstance()
	{
		static FileManager instance;
		return instance;
	}

	void FileManager::initialize(RenderDevice* renderDevice)
	{
		mRenderDevice = renderDevice;
	}

	// TODO:
	// FileManager::importScene(std::shared_ptr<Scene>, const char*)
	// Each mesh inside the file is a new gameobject

	// TODO:
	// FileManager::importMesh(std::shared_ptr<GameObject>, const char*)
	// A singular mesh without a skeleton

	// TODO:
	// FileManager::importModel(std::shared_ptr<GameObject>, const char*)
	// Parameter game object is the mesh
	// Childed to the mesh is the skeleton processed in the joint heirarchy
	// An animator is attached and sent the joint index -> transform map
	// On animator update, map[animated joint index]->setWorldMatrix(animatedTransform);
	// For more extendability, track previous animated frame's position and rotation, find the delta and set.
	void FileManager::importModel(std::shared_ptr<GameObject> gameObject, const char* filename)
	{
		// Open the model file
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Check if the file is open
		if (!file.is_open()) { return; }

		// Get the number of meshes
		uint64_t numMeshes;
		file.read((char*)&numMeshes, sizeof(uint64_t));

		// Each mesh will become a child game object with the parameter gameobject as the parent
		for (int i = 0; i < numMeshes; i++)
		{
			// Transform Data
			DirectX::XMFLOAT4X4 transform;
			MeshData meshData;
			MaterialData materialData;

			// Make a child game object
			std::shared_ptr<GameObject> child = std::make_shared<GameObject>();
			child->setParent(gameObject.get());
			gameObject->addChild(child);

			// Read in the mesh's world matrix
			file.read((char*)&transform, sizeof(DirectX::XMFLOAT4X4));

			// Read in the mesh data
			readMeshData(file, meshData);
			// Read in material data
			readMaterialData(file, materialData);

			std::shared_ptr<Mesh> mesh;

			if (meshHashMap.count(meshData.hashID) != 0)
			{
				mesh = meshHashMap[meshData.hashID];
			}
			else
			{
				mesh = std::make_shared<Mesh>(*mRenderDevice, meshData.vertexList, meshData.indexList);
				meshHashMap[meshData.hashID] = mesh;
			}

			// Create a blank material
			std::shared_ptr<Material> material = mRenderDevice->createMaterial();

			// Set the diffuse color of the material
			DirectX::XMFLOAT4 diffuseColor = { materialData.texColors[0].x, materialData.texColors[0].y, materialData.texColors[0].z, 1.0f };
			material->setDiffuseColor(diffuseColor);

			// Iterate over the possible imported textures
			for (int i = 0; i < 4; i++)
			{
				// If a filename is found for the texture import it
				if (materialData.texFilenames[i])
				{
					std::string fname = materialData.texFilenames[i];
					if (textureFileMap.count(fname) != 0)
					{
						material->setTexture((TextureType)i, textureFileMap[materialData.texFilenames[i]]);
					}
					else
					{
						// Create the texture and set it in the material
						std::shared_ptr<Texture> texture = mRenderDevice->createTexture((TextureType)i, materialData.texFilenames[i]);
						material->setTexture((TextureType)i, texture);
						textureFileMap[fname] = texture;
					}
				}
			}

			// Add the MeshRenderer component
			child->addComponent<MeshRenderer>(mesh, material);

			// Add the Transform component
			child->addComponent<Transform>(transform);

			// Check if the parent object has a transform component on it
			if (Transform* parentTransform = gameObject->getComponent<Transform>())
			{
				// Convert the transform from the child's local space to it's parent local space aka world space
				DirectX::XMFLOAT4X4 localTransform;
				parentTransform->getLocalTransform(localTransform);
				DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transform), DirectX::XMLoadFloat4x4(&localTransform)));
			}
			
			// Add the AABB component using the world-space transform
			child->addComponent<AABB>(transform, meshData.vertexList);
		}

		// Check for skeleton data in the file
		SkeletonData skeletonData;
		readSkeletonData(file, skeletonData);

		// If a skeleton was read add an Animator component and set the skeleton
		if (skeletonData.hasSkeleton)
		{
			gameObject->addComponent<Animator>(*mRenderDevice);
			gameObject->getComponent<Animator>()->setSkeleton(skeletonData.skeleton);
		}

		// Close the file
		file.close();
	}

	void FileManager::readMeshData(std::fstream& file, MeshData& meshData)
	{
		uint64_t nameLength;

		// Read the hashID
		file.read((char*)&meshData.hashID, sizeof(meshData.hashID));
		// Get the length of the name for the mesh
		file.read((char*)&nameLength, sizeof(nameLength));
		// Resize the name of the mesh
		meshData.name.resize(nameLength);
		// Read in the name of the mesh
		file.read((char*)meshData.name.c_str(), sizeof(char) * nameLength);

		// Get the number of indices for that mesh
		file.read((char*)&meshData.numIndices, sizeof(uint64_t));
		// Resize the index list to match the number of indices
		meshData.indexList.resize(meshData.numIndices);
		// Read into the index list
		file.read((char*)meshData.indexList.data(), sizeof(unsigned int) * meshData.numIndices);

		// Get the number of vertices for that mesh
		file.read((char*)&meshData.numVertices, sizeof(uint64_t));
		// Resize the vertex list to match num vertices
		meshData.vertexList.resize(meshData.numVertices);
		file.read((char*)meshData.vertexList.data(), sizeof(Vertex) * meshData.numVertices);
	}

	void FileManager::readMaterialData(std::fstream& file, MaterialData& materialData)
	{
		// Resize the arrays to hold 4 textures
		materialData.texFilenames.resize(4);
		materialData.texColors.resize(4);
		materialData.texFactors.resize(4);

		// Iterate through all 4 possible textures
		for (int i = 0; i < 4; i++)
		{
			// Read in if the material has this texture
			uint64_t hasTex;
			file.read((char*)&hasTex, sizeof(uint64_t));

			// Texture was not serialized, move on
			if (hasTex == 0)
				continue;

			// Get the size of the filename and allocate an array to hold it
			uint64_t size;
			file.read((char*)&size, sizeof(uint64_t));
			materialData.texFilenames[i] = new char[size];

			// Read in the filename, color and factor of the texture
			file.read((char*)materialData.texFilenames[i], size * sizeof(char));

			// Read in colors and factors for each texture except the normal
			if (i != 3)
			{
				file.read((char*)&materialData.texColors[i], sizeof(DirectX::XMFLOAT3));
				file.read((char*)&materialData.texFactors[i], sizeof(float));
			}
		}
	}

	void FileManager::readSkeletonData(std::fstream& file, SkeletonData& skeletonData)
	{
		file.read((char*)&skeletonData.hasSkeleton, sizeof(skeletonData.hasSkeleton));

		if (skeletonData.hasSkeleton)
		{
			// Create the skeleton
			file.read((char*)&skeletonData.numJoints, sizeof(skeletonData.numJoints));

			skeletonData.skeleton.resize(skeletonData.numJoints);

			// Iterate over each joint
			for (int i = 0; i < skeletonData.numJoints; i++)
			{
				// Read in the length of the name
				uint64_t nameLength = skeletonData.skeleton[i].name.size() + 1;
				// Write the length to the file
				file.read((char*)&nameLength, sizeof(nameLength));
				// Resize the joint's name to fit the name
				skeletonData.skeleton[i].name.resize(nameLength);
				// Write the name to the file
				file.read((char*)skeletonData.skeleton[i].name.c_str(), nameLength * sizeof(char));
				// Write the parent index
				file.read((char*)&skeletonData.skeleton[i].parentIndex, sizeof(skeletonData.skeleton[i].parentIndex));
				// Write the inverse bindpose
				file.read((char*)&skeletonData.skeleton[i].invBindposeTransform, sizeof(DirectX::XMFLOAT4X4));
				// Write the inverse bindpose
				file.read((char*)&skeletonData.skeleton[i].bindposeTransform, sizeof(DirectX::XMFLOAT4X4));
			}
		}
	}
}