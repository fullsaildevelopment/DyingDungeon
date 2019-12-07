#pragma once
#include "EngineIncludes.h"
#include "Mesh.h"

namespace Odyssey
{
	class MeshManager
	{
	public: // Singleton implementation
		static MeshManager& getInstance();
		~MeshManager() { }
	private: // Private constructor
		MeshManager() { }
	public: // Public functions
		std::shared_ptr<Mesh> importMesh(std::fstream& file);
		std::shared_ptr<Mesh> importMesh(const char* filename);
		std::shared_ptr<Mesh> createPlane(int rows, int cols, float scaleX, float scaleZ, float centerX, float centerZ);
		std::shared_ptr<Mesh> createCube(float scaleX, float scaleY, float scaleZ, float centerX, float centerY, float centerZ);
		Mesh* getMesh(int meshID);
		Mesh* getMesh(std::string name);
	private: // Private functions
		std::shared_ptr<Mesh> checkDuplicateImport(const char* filename);
	private: // Private member variables
		typedef std::vector<std::shared_ptr<Mesh>> MeshList;
		MeshList mMeshList;
		std::map<std::string, int> mMeshMap;
	};
}