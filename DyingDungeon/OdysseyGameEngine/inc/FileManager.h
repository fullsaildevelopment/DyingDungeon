#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class GameObject;

	class FileManager
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
		static FileManager& getInstance();
		~FileManager() { }
	public:
		void importModel(std::shared_ptr<GameObject> gameObject, const char* filename);
	private:
		void readMeshData(std::fstream& file, MeshData& meshData);
		void readMaterialData(std::fstream& file, MaterialData& materialData);
		void readSkeletonData(std::fstream& file, SkeletonData& skeletonData);
	private:
		FileManager() { }
	};
}