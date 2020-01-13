#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	enum class LockState
	{
		Read = 0,
		Write = 1
	};
	class ReadWriteLock
	{
	public:
		ReadWriteLock();
	public:
		void lock(LockState state);
		void unlock(LockState state);
	private:
		SRWLOCK mLock;
		LockState mState;
	};
}