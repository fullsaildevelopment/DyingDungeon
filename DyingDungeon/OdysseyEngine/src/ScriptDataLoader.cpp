#include "ScriptDataLoader.h"
#include "AABB.h"
#include "AnimatorDX11.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "RenderDevice.h"
#include "Scene.h"
#include "Transform.h"
#include <iostream>
#include <fstream>
#include "EventManager.h"
#include "RenderManager.h"

namespace Odyssey
{
	ScriptDataLoader::ScriptDataLoader()
	{

	}

	void ScriptDataLoader::importScene(Scene* scene, const char* filename)
	{
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Ensure the file is opened
		if (!file.is_open())
		{
			return;
		}

		// Get the number of meshes
		uint64_t meshCount = 0;
		file.read((char*)&meshCount, sizeof(uint64_t));

		for (unsigned int i = 0; i < meshCount; i++)
		{
			// Create a base entity with no parent
			Entity* entity = scene->createEntity();
			entity->setParent(nullptr);

			// Construct the entity from it's entry in the file
			constructEntity(file, entity);
		}
	}

	void ScriptDataLoader::importModel(Entity*& entity, const char* filename, bool isMultiMesh)
	{
		// Open the model file
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Check if the file is open
		if (!file.is_open()) { return; }

		// Get the number of meshes
		uint64_t numMeshes;
		file.read((char*)&numMeshes, sizeof(uint64_t));

		int begin = 0;
		if (isMultiMesh == false)
		{
			begin = 1;

			// Import the first mesh and assign it to the parameter Entity
			entity->setParent(nullptr);
			constructEntity(file, entity);
		}

		// Iterate over all other meshes in the file
		for (int i = begin; i < numMeshes; i++)
		{
			// Each additional mesh in the file becomes a child to the first mesh
			Entity* child = entity->getScene()->createEntity();

			// Set the parent of the mesh to the first mesh Entity
			child->setParent(entity);

			// Add the child to the first mesh Entity
			entity->addChild(child);

			// Construct the Entity from the file data
			constructEntity(file, child);
		}

		// Check for skeleton data in the file
		SkeletonData skeletonData;
		readSkeletonData(file, skeletonData);

		// If a skeleton was read add an Animator component and set the skeleton
		if (skeletonData.hasSkeleton)
		{
			entity->addComponent<AnimatorDX11>();
			entity->getComponent<AnimatorDX11>()->setSkeleton(skeletonData.skeleton);
		}

		// Close the file
		file.close();
	}

	void ScriptDataLoader::constructEntity(std::fstream& file, Entity*& entity)
	{
		DirectX::XMFLOAT4X4 transform;
		int mesh = -1;
		int material = -1;
		MeshData meshData;
		MaterialData materialData;

		// Read in the transform data
		file.read((char*)&transform, sizeof(DirectX::XMFLOAT4X4));

		// Read in and process the mesh data
		readMeshData(file, meshData);
		processMeshData(meshData, mesh);

		// Read in and process material data
		readMaterialData(file, materialData);
		processMaterialData(materialData, material);

		// Add the Transform component
		if (Transform* tComponent = entity->getComponent<Transform>())
		{
			// Get the position of the attached transform
			DirectX::XMFLOAT3 pos = tComponent->getPosition();

			// Get the rotation of the attached transform
			DirectX::XMFLOAT3 rot = tComponent->getEulerRotation();

			// Get the scale of the attached transform
			DirectX::XMFLOAT3 scale = tComponent->getScale();

			// Remove the previously attached transform component
			entity->removeComponent<Transform>(tComponent);

			// Add a new transform component with the transform read from the file
			entity->addComponent<Transform>(transform);

			// Add the previous position, rotation and scale to the new transform component
			if (tComponent = entity->getComponent<Transform>())
			{
				tComponent->addPosition(pos.x, pos.y, pos.z);
				tComponent->addRotation(rot.x, rot.y, rot.z);
				tComponent->setScale(scale.x, scale.y, scale.z);
			}
		}
		else
		{
			entity->addComponent<Transform>(transform);
		}


		// Add the MeshRenderer component
		entity->addComponent<MeshRenderer>(mesh, material);

		// Add the AABB component
		DirectX::XMFLOAT4X4 globalTransform = entity->getComponent<Transform>()->getGlobalTransform();
		entity->addComponent<AABB>(globalTransform, meshData.vertexList);

		for (int i = 0; i < 4; i++)
		{
			if (materialData.texFilenames[i])
			{
				delete materialData.texFilenames[i];
				materialData.texFilenames[i] = nullptr;
			}
		}
	}
	void ScriptDataLoader::readMeshData(std::fstream& file, MeshData& meshData)
	{
		uint64_t nameLength;

		// Read the mesh's hash ID
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

	void ScriptDataLoader::readMaterialData(std::fstream& file, MaterialData& materialData)
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

	void ScriptDataLoader::readSkeletonData(std::fstream& file, SkeletonData& skeletonData)
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

	void ScriptDataLoader::processMeshData(MeshData& meshData, int& mesh)
	{
		// Check if the mesh has already been imported
		if (mMeshMap.count(meshData.hashID) != 0)
		{
			// This is a duplicate mesh, reference the same pointer
			mesh = mMeshMap[meshData.hashID];
		}
		else
		{
			// This is a new mesh, allocate a new mesh pointer and put it in the map
			mesh = RenderManager::getInstance().createMesh(meshData.vertexList, meshData.indexList);
			mMeshMap[meshData.hashID] = mesh;
		}
	}

	void ScriptDataLoader::processMaterialData(MaterialData& materialData, int& material)
	{
		// Create a blank material
		material = RenderManager::getInstance().createMaterial();

		// Get the raw pointer of the material
		Material* materialPtr = RenderManager::getInstance().getMaterial(material);

		// Set the diffuse color of the material
		DirectX::XMFLOAT4 diffuseColor = { materialData.texColors[0].x, materialData.texColors[0].y, materialData.texColors[0].z, 1.0f };
		materialPtr->setDiffuseColor(diffuseColor);

		// Iterate over the possible imported textures
		for (int i = 0; i < 4; i++)
		{
			// Check for a valid texture filename
			if (materialData.texFilenames[i])
			{
				// Convert the filename to a string
				std::string fname = materialData.texFilenames[i];
				
				// Check the texture map for a duplicate import
				if (mTextureMap.count(fname) != 0)
				{
					// Duplicate found, assign the previously imported texture
					materialPtr->setTexture((TextureType)i, mTextureMap[materialData.texFilenames[i]]);
				}
				else
				{
					// No duplicate found, create a new texture and set it in the material.
					int texture = RenderManager::getInstance().createTexture((TextureType)i, materialData.texFilenames[i]);
					materialPtr->setTexture((TextureType)i, texture);
				
					// Add the texture to the import map
					mTextureMap[fname] = texture;
				}
			}
		}
	}
}