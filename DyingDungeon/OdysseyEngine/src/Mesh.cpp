#include "Mesh.h"
#include "Buffer.h"
#include "RenderManager.h"

namespace Odyssey
{
	Mesh::Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
	{

		mVertexBuffer = RenderManager::getInstance().createBuffer(BufferBindFlag::VertexBuffer, static_cast<size_t>(vertexList.size()),
			static_cast<UINT>(sizeof(Vertex)), (void*)vertexList.data());

		mIndexBuffer = RenderManager::getInstance().createBuffer(BufferBindFlag::IndexBuffer, static_cast<size_t>(indexList.size()),
			static_cast<UINT>(sizeof(unsigned int)), (void*)indexList.data());
		mNumberOfIndices = static_cast<int>(indexList.size());
	}

	void Mesh::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind the vertex and index buffer to the rendering pipeline.
		RenderManager::getInstance().getBuffer(mVertexBuffer)->bind(context);
		RenderManager::getInstance().getBuffer(mIndexBuffer)->bind(context);
	}

	void Mesh::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind the vertex and index buffer to the rendering pipeline.
		if (RenderManager::getInstance().getBuffer(mVertexBuffer))
		{
			RenderManager::getInstance().getBuffer(mVertexBuffer)->unbind(context, 0, ShaderType::VertexShader);
		}
		
		if (RenderManager::getInstance().getBuffer(mIndexBuffer))
		{
			RenderManager::getInstance().getBuffer(mIndexBuffer)->unbind(context, 0, ShaderType::VertexShader);
		}
	}

	Buffer* Mesh::getVertexBuffer()
	{
		return RenderManager::getInstance().getBuffer(mVertexBuffer);
	}

	Buffer* Mesh::getIndexBuffer()
	{
		return RenderManager::getInstance().getBuffer(mIndexBuffer);
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