#include "MeshManager.h"
#include "Buffer.h"
#include <fstream>

namespace Odyssey
{
	MeshManager& MeshManager::getInstance()
	{
		static MeshManager instance;
		return instance;
	}

	std::shared_ptr<Mesh> MeshManager::createPlane(DirectX::XMFLOAT2 dimensions, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT3 center)
	{
		// Calculate the number of total vertices and the number of cells in each direction
		unsigned int rows = static_cast<unsigned int>(dimensions.x);
		unsigned int cols = static_cast<unsigned int>(dimensions.y);

		unsigned int numVertices = rows * cols;
		unsigned int cellRows = rows - 1;
		unsigned int cellCols = cols - 1;

		// Calculate the number of triangles to build the grid
		unsigned int numTriangles = cellRows * cellCols * 2;

		// Delta x and z for each cell
		float dx = 1.0f / (float)cellCols;
		float dz = 1.0f / (float)cellRows;

		// Calculate the width and depth of the grid
		float width = (float)cellCols * dx;
		float depth = (float)cellRows * dz;

		// Resize the list to hold the number of vertices
		std::vector<Vertex> vertexList;
		vertexList.resize(numVertices);

		// Offset the x and z so we draw from the center
		float xOffset = -width * 0.5f;
		float zOffset = depth * 0.5f;

		unsigned int vIndex = 0;
		for (float i = 0; i < rows; ++i) {

			for (float j = 0; j < cols; ++j)
			{
				// Negate the depth coordinate to put in
				// quadrant four.  Then offset to center about
				// coordinate system.
				vertexList[vIndex].position.x = ((j * dx + xOffset) * scale.x) + center.x;
				vertexList[vIndex].position.y = center.y;
				vertexList[vIndex].position.z = ((-i * dz + zOffset) * scale.y) + center.z;
				vertexList[vIndex].normal = { 0,1,0 };
				vertexList[vIndex].tex.x = 1.0f - (j * dx);
				vertexList[vIndex].tex.y = (i * dz);
				++vIndex;
			}
		}

		// Resize the indices to hold the number of indices per triangle
		std::vector<unsigned int> indexList;
		indexList.resize(numTriangles * 3);

		// Reset the index counter
		vIndex = 0;

		// Generate indices for each quad
		for (unsigned int i = 0; i < cellRows; ++i)
		{
			for (unsigned int j = 0; j < cellCols; ++j)
			{
				indexList[vIndex] = i * cols + j;
				indexList[vIndex + 1] = i * cols + j + 1;
				indexList[vIndex + 2] = (i + 1) * cols + j;

				indexList[vIndex + 3] = (i + 1) * cols + j;
				indexList[vIndex + 4] = i * cols + j + 1;
				indexList[vIndex + 5] = (i + 1) * cols + j + 1;

				// next quad
				vIndex += 6;
			}
		}

		// Create the mesh and set it's index/vertex data
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertexList, indexList);
		return mesh;
	}

	std::shared_ptr<Mesh> MeshManager::createCube(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 center)
	{
		std::vector<Vertex> vertexList;
		vertexList.resize(8);
		vertexList[0].position = DirectX::XMFLOAT3(-scale.x + center.x, -scale.y + center.y, -scale.z + center.z);
		vertexList[1].position = DirectX::XMFLOAT3(-scale.x + center.x, +scale.y + center.y, -scale.z + center.z);
		vertexList[2].position = DirectX::XMFLOAT3(+scale.x + center.x, +scale.y + center.y, -scale.z + center.z);
		vertexList[3].position = DirectX::XMFLOAT3(+scale.x + center.x, -scale.y + center.y, -scale.z + center.z);
		vertexList[4].position = DirectX::XMFLOAT3(-scale.x + center.x, -scale.y + center.y, +scale.z + center.z);
		vertexList[5].position = DirectX::XMFLOAT3(-scale.x + center.x, +scale.y + center.y, +scale.z + center.z);
		vertexList[6].position = DirectX::XMFLOAT3(+scale.x + center.x, +scale.y + center.y, +scale.z + center.z);
		vertexList[7].position = DirectX::XMFLOAT3(+scale.x + center.x, -scale.y + center.y, +scale.z + center.z);

		std::vector<unsigned int> indexList
		{
			// front face
			0, 1, 2, 0, 2, 3,
			// back face
			4, 6, 5, 4, 7, 6,
			// left face
			4, 5, 1, 4, 1, 0,
			// right face
			3, 2, 6, 3, 6, 7,
			// top face
			1, 5, 6, 1, 6, 2,
			// bottom face
			4, 0, 3, 4, 3, 7
		};

		// Create the mesh and set it's index/vertex data
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertexList, indexList);
		return mesh;
	}

	std::shared_ptr<Mesh> MeshManager::createMesh(size_t hashID, std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
	{
		// Check if we have registered this mesh already
		if (meshMap.count(hashID) != 0)
		{
			// Return the stored mesh
			return meshMap[hashID];
		}

		// Create a new mesh
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertexList, indexList);
		// Store the mesh according to it's hash ID and return the mesh pointer
		meshMap[hashID] = mesh;
		return mesh;
	}
}