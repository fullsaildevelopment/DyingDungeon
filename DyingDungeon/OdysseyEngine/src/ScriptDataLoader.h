#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "EngineEvents.h"

namespace Odyssey
{
	class Entity;
	class Material;
	class Mesh;
	class RenderDevice;
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
		ScriptDataLoader(std::shared_ptr<RenderDevice> renderDevice);
		void importScene(std::shared_ptr<Scene> scene, const char* filename);
		void importModel(std::shared_ptr<Entity> entity, const char* filename, bool isMultiMesh);
		void onShutdown(EngineShutdownEvent* evnt);
	private:
		void constructEntity(std::fstream& file, std::shared_ptr<Entity> entity);
		void readMeshData(std::fstream& file, MeshData& meshData);
		void readMaterialData(std::fstream& file, MaterialData& materialData);
		void readSkeletonData(std::fstream& file, SkeletonData& skeletonData);
		void processMeshData(MeshData& meshData, std::shared_ptr<Mesh>& mesh);
		void processMaterialData(MaterialData& materialData, std::shared_ptr<Material>& material);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::map<size_t, std::shared_ptr<Mesh>> meshHashMap;
		std::map<std::string, std::shared_ptr<Texture>> textureFileMap;
	};
}