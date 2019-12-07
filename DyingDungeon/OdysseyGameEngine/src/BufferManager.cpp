#include "Buffer.h"
#include "BufferManager.h"

namespace Odyssey
{
	BufferManager& BufferManager::getInstance()
	{
		static BufferManager instance;
		return instance;
	}

	std::shared_ptr<Buffer> BufferManager::createBuffer(BufferBindFlag bindFlag, size_t count, UINT stride, const void* data)
	{
		// Create a unique pointer to a buffer
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(bindFlag, count, stride, data);

		// Store the created buffer and generate an ID
		mBufferList.push_back(buffer);

		return buffer;
	}

	Buffer* BufferManager::getBuffer(int index)
	{
		return mBufferList[index].get();
	}
}