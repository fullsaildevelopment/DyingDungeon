#pragma once
#include "EngineIncludes.h"
#include "ReadWriteLock.h"
#include "Component.h"
#include <stack>
#include "ComponentHandle.h"

namespace Odyssey
{
	class Component;
	class EntityHandle;

	class SceneHandle
	{
	public:
		SceneHandle();
	private:
		std::map<int, std::vector<std::shared_ptr<Component>>> mComponentMap;
		std::stack<int> mMapStack;
		// Map of <child, parent>
		std::map<int, int> mSceneGraph;
		ReadWriteLock mLock;
	public:
		EntityHandle createEntity();
		void destroyEntity(EntityHandle entity);
		void setParent(int entityID, int parentID);
		EntityHandle getParent(int entityID);
	public:
		template<class ComponentType, typename... Args>
		void addComponent(int entityID, Args&&... params)
		{
			mComponentMap[entityID].emplace_back(std::make_shared<ComponentType>(std::forward<Args>(params)...));
		}

		template<class ComponentType>
		ComponentHandle<ComponentType> getComponent(int entityID)
		{
			mLock.lock(LockState::Read);
			for (std::shared_ptr<Component> component : mComponentMap[entityID])
			{
				if (component->isClassType(ComponentType::Type))
				{
					ComponentHandle<ComponentType> handle(entityID, static_cast<ComponentType*>(component.get()), this);
					mLock.unlock(LockState::Read);
					return handle;
				}
			}
			mLock.unlock(LockState::Read);
			return ComponentHandle<ComponentType>();
		}

		template<class ComponentType>
		std::vector<ComponentHandle<ComponentType>> getComponents(int entityID)
		{
			std::vector<ComponentHandle<ComponentType>> componentsOfType;

			mLock.lock(LockState::Read);

			for (std::shared_ptr<Component> component : mComponentMap[entityID])
			{
				if (component->isClassType(ComponentType::Type))
				{
					componentsOfType.emplace_back(ComponentHandle<ComponentType>(entityID, static_cast<ComponentType*>(component.get()), this));
				}
			}

			mLock.unlock(LockState::Read);
			return componentsOfType;
		}

		template<class ComponentType>
		ComponentHandle<ComponentType> getRootComponent(int entityID)
		{
			mLock.lock(LockState::Read);
			
			int current = mSceneGraph[entityID];

			while (current != -1)
			{
				current = mSceneGraph[current];
			}

			if (ComponentHandle<ComponentType> handle = getComponent<ComponentType>(current))
			{
				return handle;
			}
			else
			{
				return getComponent<ComponentType>(entityID);
			}
		}

		template<class ComponentType>
		bool removeComponent(int entityID)
		{
			mLock.lock(LockState::Write);
			if (mComponentMap[entityID].size() == 0)
			{
				return false;
			}

			for (int i = 0; i < mComponentMap[entityID].size(); i++)
			{
				if (mComponentMap[entityID][i]->isClassType(ComponentType::Type))
				{
					mComponentMap[entityID].erase(mComponentMap[entityID].begin() + i);
					mLock.unlock(LockState::Write);
					return true;
				}
			}
			mLock.unlock(LockState::Write);
			return false;
		}

		template<class ComponentType>
		bool removeComponent(int entityID, ComponentHandle<ComponentType> searchHandle)
		{
			if (mComponentMap[entityID].empty())
			{
				return false;
			}

			for (int i = 0; i < mComponentMap[entityID].size(); i++)
			{
				if (mComponentMap[entityID][i]->isClassType(ComponentType::Type) &&
					mComponentMap[entityID][i].get() == searchHandle.mComponent)
				{
					mComponentMap[entityID].erase(mComponentMap[searchHandle.mEntityID].begin() + i);
					return true;
				}
			}

			return false;
		}

		template<class ComponentType>
		int removeComponents(int entityID)
		{
			if (mComponentMap[entityID].empty())
			{
				return 0;
			}

			int numRemoved = 0;

			for (int i = 0; i < mComponentMap[entityID].size(); i++)
			{
				if (mComponentMap[entityID][i]->isClassType(ComponentType::Type))
				{
					mComponentMap[entityID].erase(mComponentMap[entityID].begin() + i);
					i--;
					numRemoved++;
				}
			}

			return numRemoved;
		}
	};
}