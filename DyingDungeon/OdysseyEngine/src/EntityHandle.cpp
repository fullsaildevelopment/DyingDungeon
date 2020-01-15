#include "EntityHandle.h"

namespace Odyssey
{
	EntityHandle::EntityHandle(int id, SceneHandle* handle, int parentID)
	{
		mID = id;
		mHandle = handle;
		mParent = parentID;
	}

	void EntityHandle::setParent(int parentID)
	{
		mParent = parentID;
		mHandle->setParent(mID, parentID);
	}
	void EntityHandle::setStatic(bool isStatic)
	{
		mStatic = isStatic;
	}
	EntityHandle EntityHandle::getParent()
	{
		if (mParent == -1)
			return EntityHandle(-1, nullptr, -1);
		return mHandle->getParent(mID);
	}
}