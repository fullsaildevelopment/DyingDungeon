#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class Buffer;

	class BufferManager
	{
	public:  // Public Singleton implementation
		static BufferManager& getInstance();
		~BufferManager() { }
	private: // Private Singleton implementation
		BufferManager() { }
	public:  // Public functions
		// Factory method for creating gpu buffers
		std::shared_ptr<Buffer> createBuffer(BufferBindFlag bindFlag, size_t count, UINT stride, const void* data);
		// Returns the raw buffer pointer behind the buffer ID
		Buffer* getBuffer(int bufferID);
	private:
		// Typedef for a list of buffer pointers created by the application
		typedef std::vector<std::shared_ptr<Buffer>> BufferList;
		BufferList mBufferList;
	};
}