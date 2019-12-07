#include "Mesh.h"
#include "Buffer.h"
#include "BufferManager.h"

namespace Odyssey
{
	Mesh::Mesh()
	{
		mNumberOfIndices = -1;
		mVertexList.resize(0);
		mIndexList.resize(0);
	}

	Mesh::Mesh(std::shared_ptr<Buffer> vertexBuffer, std::shared_ptr<Buffer> indexBuffer, int numIndices)
	{
		mVertexBuffer = vertexBuffer;
		mIndexBuffer = indexBuffer;
		mNumberOfIndices = numIndices;
		mVertexList.resize(0);
		mIndexList.resize(0);
	}

	Mesh::Mesh(Mesh& other)
	{
		mNumberOfIndices = other.mNumberOfIndices;
	}

	void Mesh::bind()
	{
		// Bind the vertex and index buffer to the rendering pipeline.
		mVertexBuffer->bind();
		mIndexBuffer->bind();
	}

	void Mesh::unbind()
	{
		// Bind the vertex and index buffer to the rendering pipeline.
		mVertexBuffer->unbind(0, ShaderType::VertexShader);
		mIndexBuffer->unbind(0, ShaderType::VertexShader);
	}

	void Mesh::flipWindingOrder()
	{
		std::reverse(mIndexList.begin(), mIndexList.end());
		mIndexBuffer->updateData(mIndexList.data());
	}

	std::shared_ptr<Buffer> Mesh::getVertexBuffer()
	{
		return mVertexBuffer;
	}

	std::shared_ptr<Buffer> Mesh::getIndexBuffer()
	{
		return mIndexBuffer;
	}

	const std::vector<Vertex> Mesh::getVertexList()
	{
		return mVertexList;
	}

	const int Mesh::getNumberOfIndices()
	{
		return mNumberOfIndices;
	}

	void Mesh::setName(std::string name)
	{
		mName = name;
	}

	void Mesh::setVertexList(std::vector<Vertex> vertexList)
	{
		mVertexList = vertexList;
	}

	void Mesh::setIndexList(std::vector<unsigned int> indexList)
	{
		mIndexList = indexList;
	}

	void Mesh::setNumberOfindices(int numIndices)
	{
		mNumberOfIndices = numIndices;
	}
}