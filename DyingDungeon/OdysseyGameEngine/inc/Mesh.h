#pragma once
#include "EngineIncludes.h"
#include "Buffer.h"

namespace Odyssey
{
	class Mesh
	{
	public: // Constructors
		Mesh();
		Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
		Mesh(Mesh& other);
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
		std::unique_ptr<Buffer> mVertexBuffer;
		std::unique_ptr<Buffer> mIndexBuffer;
		int mNumberOfIndices;
	};
}