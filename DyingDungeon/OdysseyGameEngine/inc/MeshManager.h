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
		std::shared_ptr<Mesh> createPlane(int rows, int cols, float scaleX, float scaleZ, float centerX, float centerZ);
		std::shared_ptr<Mesh> createCube(float scaleX, float scaleY, float scaleZ, float centerX, float centerY, float centerZ);
		std::shared_ptr<Mesh> createMesh(size_t hashID, std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
	private:
		std::map<size_t, std::shared_ptr<Mesh>> meshMap;
	};
}