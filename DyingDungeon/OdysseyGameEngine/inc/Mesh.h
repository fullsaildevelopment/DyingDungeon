#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class Buffer;
	class RenderDevice;

	class Mesh
	{
	public: // Constructors
		Mesh(RenderDevice& renderDevice, std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
		~Mesh() = default;
	public: // Functions
		void bind();
		void unbind();
	public: // Accessors
		Buffer* getVertexBuffer();
		Buffer* getIndexBuffer();
		const int getNumberOfIndices();
	public: // Mutators
		void setName(std::string name);
		void setNumberOfindices(int id);
	private:
		std::string mName;
		std::shared_ptr<Buffer> mVertexBuffer;
		std::shared_ptr<Buffer> mIndexBuffer;
		int mNumberOfIndices;
	};
}