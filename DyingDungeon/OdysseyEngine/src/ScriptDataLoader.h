#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "EngineEvents.h"

namespace Odyssey
{
	class Entity;
	class Material;
	class Mesh;
	class Scene;
	class Texture;
	class Transform;

	class ScriptDataLoader
	{
	private:
		struct MeshData
		{
			size_t hashID;
			std::string name;
			uint64_t numIndices;
			std::vector<unsigned int> indexList;
			uint64_t numVertices;
			std::vector<Vertex> vertexList;
		};
		struct MaterialData
		{
			std::vector<const char*> texFilenames;
			std::vector<DirectX::XMFLOAT3> texColors;
			std::vector<float> texFactors;
		};
		struct SkeletonData
		{
			uint64_t hasSkeleton;
			uint64_t numJoints;
			std::vector<Joint> skeleton;
		};

	public:
		ScriptDataLoader();
		void importScene(Scene* scene, const char* filename);
		void importModel(Entity*& entity, const char* filename, bool isMultiMesh);
	private:
		void constructEntity(std::fstream& file, Entity*& entity);
		void readMeshData(std::fstream& file, MeshData& meshData);
		void readMaterialData(std::fstream& file, MaterialData& materialData);
		void readSkeletonData(std::fstream& file, SkeletonData& skeletonData);
		void processMeshData(MeshData& meshData, int& mesh);
		void processMaterialData(MaterialData& materialData, int& material);
	private:
		typedef std::map<size_t, int> MeshHashMap;
		MeshHashMap mMeshMap;
		typedef std::map<std::string, int> TextureFileMap;
		TextureFileMap mTextureMap;
	};
}