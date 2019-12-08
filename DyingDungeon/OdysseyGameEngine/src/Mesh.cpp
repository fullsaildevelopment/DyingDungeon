#include "Mesh.h"
#include "Buffer.h"
#include "BufferManager.h"

namespace Odyssey
{
	Mesh::Mesh()
	{
		mNumberOfIndices = 0;
	}

	Mesh::Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
	{
		mVertexBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::VertexBuffer, vertexList.size(), sizeof(Vertex), (void*)vertexList.data());
		mIndexBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::IndexBuffer, indexList.size(), sizeof(unsigned int), (void*)indexList.data());
		mNumberOfIndices = indexList.size();
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

	std::shared_ptr<Buffer> Mesh::getVertexBuffer()
	{
		return mVertexBuffer;
	}

	std::shared_ptr<Buffer> Mesh::getIndexBuffer()
	{
		return mIndexBuffer;
	}
	
	const int Mesh::getNumberOfIndices()
	{
		return mNumberOfIndices;
	}

	void Mesh::setName(std::string name)
	{
		mName = name;
	}

	void Mesh::setNumberOfindices(int numIndices)
	{
		mNumberOfIndices = numIndices;
	}
}