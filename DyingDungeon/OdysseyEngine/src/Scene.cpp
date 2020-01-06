#include "Camera.h"
#include "Component.h"
#include "Entity.h"
#include "UICanvas.h"
#include "Scene.h"
#include "MeshRenderer.h"

namespace Odyssey
{
	void Scene::addLight(std::shared_ptr<Light> light)
	{
		// Add the light to the light vector
		mSceneLights.push_back(light);
	}

	void Scene::addEntity(std::shared_ptr<Entity> entity)
	{
		// Add the entity to the entity vector
		mSceneEntities.push_back(entity);

		for (Component* component : entity->getComponents<Component>())
		{
			mComponentList.push_back(component);
		}

		for (MeshRenderer* meshRenderer : entity->getComponents<MeshRenderer>())
		{
			mRenderList.push_back(meshRenderer);
		}

		for (std::shared_ptr<Entity> child : entity->getChildren())
		{
			for (Component* component : child->getComponents<Component>())
			{
				mComponentList.push_back(component);
			}

			for (MeshRenderer* childMesh : child->getComponents<MeshRenderer>())
			{
				mRenderList.push_back(childMesh);
			}
		}

		// Check if the entity has a camera component
		if (entity->getComponent<Camera>())
		{
			// Set this as the main camera
			mMainCamera = entity;
		}

		// Check if the entity has a UI canvas component
		for (UICanvas* canvas : entity->getComponents<UICanvas>())
		{
			// Add it to the vector of UI canvas objects
			mSceneCanvas.push_back(canvas);

			for (UIElement* element : canvas->getElements<UIElement>())
			{
				mElementList.push_back(element);
			}
		}
	}

	double Scene::getDeltaTime()
	{
		// Return delta time
		return mDeltaTime;
	}

	Light* Scene::getLight(int index)
	{
		// Check the index is within the size of the light vector
		if (mSceneLights.size() > index)
		{
			// Get the light at the parameter index
			return mSceneLights[index].get();
		}

		// Return nullptr by default
		return nullptr;
	}

	Entity* Odyssey::Scene::getMainCamera()
	{
		// Return the entity registered with the main camera
		return mMainCamera.get();
	}
}