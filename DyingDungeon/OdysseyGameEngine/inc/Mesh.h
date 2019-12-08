#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class Buffer;

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
		std::shared_ptr<Buffer> getVertexBuffer();
		std::shared_ptr<Buffer> getIndexBuffer();
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