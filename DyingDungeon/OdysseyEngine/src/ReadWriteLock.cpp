#include "ReadWriteLock.h"

namespace Odyssey
{
	ReadWriteLock::ReadWriteLock()
	{
		InitializeSRWLock(&mLock);
	}

	void ReadWriteLock::lock(LockState state)
	{
		switch (state)
		{
			case Odyssey::LockState::Read:
			{
				AcquireSRWLockShared(&mLock);
				break;
			}
			case Odyssey::LockState::Write:
			{
				AcquireSRWLockExclusive(&mLock);
				break;
			}
		}
	}

	void ReadWriteLock::unlock(LockState state)
	{
		switch (state)
		{
			case Odyssey::LockState::Read:
			{
				ReleaseSRWLockShared(&mLock);
				break;
			}
			case Odyssey::LockState::Write:
			{
				ReleaseSRWLockExclusive(&mLock);
				break;
			}
		}
	}
}