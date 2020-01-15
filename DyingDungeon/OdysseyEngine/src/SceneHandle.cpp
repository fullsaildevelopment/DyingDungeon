#include "SceneHandle.h"
#include "EntityHandle.h"

namespace Odyssey
{
	SceneHandle::SceneHandle()
	{
		mMapStack.push(0);
	}

	EntityHandle SceneHandle::createEntity()
	{
		// Get the next free ID
		int id = mMapStack.top();
		mMapStack.pop();

		// Create the entity
		EntityHandle entity(id, this);

		// Push the next index onto the ID stack
		mMapStack.push(mComponentMap.size() - 1);

		// Set the entity's parent to none by default
		mSceneGraph[id] = -1;

		return entity;
	}

	void SceneHandle::destroyEntity(EntityHandle entity)
	{
		mComponentMap.erase(entity.mID);
		mMapStack.push(entity.mID);
	}
	void SceneHandle::setParent(int entityID, int parentID)
	{
		mSceneGraph[entityID] = parentID;
	}

	EntityHandle SceneHandle::getParent(int entityID)
	{
		// Get the parent's ID
		int pID = mSceneGraph[entityID];

		// If the parent is not -1, get the parent's parent
		int parent = (pID != -1) ? mSceneGraph[pID] : -1;

		// Return an entity handle for the parent
		return EntityHandle(parent, this, parent);
	}
}