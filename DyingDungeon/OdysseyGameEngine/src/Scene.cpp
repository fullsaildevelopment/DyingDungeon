#include "Scene.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "RenderPass.h"
#include "ParticleSystem.h"
#include "Buffer.h"
#include "Component.h"
#include "RenderDevice.h"

namespace Odyssey
{
	void Scene::addLight(std::shared_ptr<Light> light)
	{
		mSceneLights.push_back(light);
	}

	void Scene::addSceneObject(std::shared_ptr<GameObject> sceneObject)
	{
		mSceneObjectList.push_back(sceneObject);

		if (sceneObject->getComponent<Camera>())
		{
			mMainCamera = sceneObject;
		}
	}

	void Scene::initialize()
	{
		mXTimer.Restart();
		for (std::shared_ptr<GameObject> sceneObject : mSceneObjectList)
		{
			for (auto* component : sceneObject->getComponents<Component>())
			{
				component->initialize();
			}

			for (auto child : sceneObject->getChildren())
			{
				for (auto component : child->getComponents<Component>())
				{
					component->initialize();
				}
			}
		}
	}

	void Scene::update()
	{
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Update the scene
		updateScene();
	}

	double Scene::getDeltaTime()
	{
		return mDeltaTime;
	}

	std::shared_ptr<Light> Scene::getLight(int index)
	{
		return mSceneLights[index];
	}

	std::vector<std::shared_ptr<Light>> Scene::getSceneLights()
	{
		return mSceneLights;
	}

	std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects()
	{
		return mSceneObjectList;
	}

	void Scene::updateScene()
	{
		// Generate a render list
		for (std::shared_ptr<GameObject> renderObject : mSceneObjectList)
		{
			for (auto& component : renderObject->getComponents<Component>())
			{
				component->update(mDeltaTime);
			}
		}
	}
}