#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "SceneHandle.h"
#include "ComponentHandle.h"

namespace Odyssey
{
	class SceneHandle;

	class EntityHandle
	{
	public:
		EntityHandle(int id, SceneHandle* handle, int parentID = -1);
		void setParent(int parentID);
		void setStatic(bool isStatic);
		EntityHandle getParent();

	public:
		bool mStatic;
		int mParent;
		int mID;
		SceneHandle* mHandle;

	public:
		template<class ComponentType, typename... Args>
		void addComponent(Args&&... params)
		{
			mHandle->addComponent<ComponentType>(mID, params...);
		}

		template<class ComponentType>
		ComponentHandle<ComponentType> getComponent()
		{
			return mHandle->getComponent<ComponentType>(mID);
		}

		template<class ComponentType>
		std::vector<ComponentHandle<ComponentType>> getComponents()
		{
			return mHandle->getComponents<ComponentType>(mID);
		}

		template<class ComponentType>
		ComponentHandle<ComponentType> getRootComponent()
		{
			return mHandle->getRootComponent<ComponentType>(mID);
		}

		template<class ComponentType>
		bool removeComponent()
		{
			return mHandle->removeComponent<ComponentType>(mID);
		}

		template<class ComponentType>
		bool removeComponent(ComponentHandle<ComponentType> handle)
		{
			return mHandle->removeComponent<ComponentType>(mID, handle);
		}

		template<class ComponentType>
		int removeComponents()
		{
			return mHandle->removeComponents<ComponentType>(mID);
		}
	};
}