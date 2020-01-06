#include "SceneDX11.h"
#include "Entity.h"
#include "Component.h"

namespace Odyssey
{
	void SceneDX11::initialize()
	{
		// Restart the timer
		mXTimer.Restart();

		// Iterate through the entity vector
		for (std::shared_ptr<Entity> entity : mSceneEntities)
		{
			// Iterate through each component of the entity
			for (auto* component : entity->getComponents<Component>())
			{
				// Initialize the component
				component->initialize();
			}

			// Iterate through each child entity
			for (auto child : entity->getChildren())
			{
				// Iterate through each component of the child
				for (auto component : child->getComponents<Component>())
				{
					// Initialize the component
					component->initialize();
				}
			}
		}
	}

	void SceneDX11::update()
	{
		// Signal the timer
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Iterate through the entity vector
		for (std::shared_ptr<Entity> entity : mSceneEntities)
		{
			// Iterate through each component of the entity
			for (auto& component : entity->getComponents<Component>())
			{
				// Update the component
				component->update(mDeltaTime);
			}
		}
	}

	std::vector<std::shared_ptr<Light>> SceneDX11::getSceneLights()
	{
		// Return the scene light vector
		return mSceneLights;
	}

	std::vector<std::shared_ptr<Entity>> SceneDX11::getEntities()
	{
		// Return the scene entity vector
		return mSceneEntities;
	}

	std::vector<UICanvas*> SceneDX11::getCanvasList()
	{
		// Return the scene canvas vector
		return mSceneCanvas;
	}
}