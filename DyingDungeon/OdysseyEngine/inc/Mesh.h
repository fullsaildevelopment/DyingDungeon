#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class Buffer;
	class RenderDevice;

	class Mesh
	{
	public: // Constructors
		Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
		~Mesh() = default;
	public: // Functions
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	public: // Accessors
		Buffer* getVertexBuffer();
		Buffer* getIndexBuffer();
		const int getNumberOfIndices();
	public: // Mutators
		void setName(std::string name);
		void setNumberOfindices(int id);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::string mName;
		int mVertexBuffer;
		int mIndexBuffer;
		int mNumberOfIndices;
	};
}