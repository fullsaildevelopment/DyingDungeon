#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class SceneHandle;

	template<typename ComponentType>
	class ComponentHandle
	{
	public:
		int mEntityID;
		ComponentType* mComponent;
		SceneHandle* mScene;

		ComponentHandle()
		{
			mEntityID = -1;
			mComponent = nullptr;
			mScene = nullptr;
		}

		ComponentHandle(unsigned int id, ComponentType* component, SceneHandle* sceneHandle)
		{
			mEntityID = id;
			mComponent = component;
			mScene = sceneHandle;
		}

		ComponentType* operator->() const
		{
			return mComponent;
		}

		void destroy()
		{

		}
	};
}