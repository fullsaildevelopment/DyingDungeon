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
		std::shared_ptr<Mesh> createPlane(DirectX::XMFLOAT2 dimensions, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT3 center);
		std::shared_ptr<Mesh> createCube(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 center);
		std::shared_ptr<Mesh> createMesh(size_t hashID, std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
	private:
		std::map<size_t, std::shared_ptr<Mesh>> meshMap;
	};
}