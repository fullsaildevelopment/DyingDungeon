#include "Scene.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "RenderPass.h"
#include "RenderPipelineManager.h"
#include "ParticleSystem.h"
#include "Buffer.h"
#include "Component.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Scene::Scene(RenderDevice& renderDevice)
		: mDevice(renderDevice)
	{
		mPerFrameBuffer = mDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(PerFrameBuffer)), nullptr);

		mPerObjectBuffer = mDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(PerObjectBuffer)), nullptr);
	}

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

		// Update the render args lists
		renderArgs.perFrameBuffer = mPerFrameBuffer.get();
		renderArgs.perObjectBuffer = mPerObjectBuffer.get();
		renderArgs.camera = mMainCamera.get();
		renderArgs.lightList = mSceneLights;
		renderArgs.renderList = mSceneObjectList;
	}

	void Scene::update()
	{
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Update the scene
		updateScene();

		// Render the scene
		RenderPipelineManager::getInstance().render(renderArgs);
	}

	double Scene::getDeltaTime()
	{
		return mDeltaTime;
	}

	std::shared_ptr<Light> Scene::getLight(int index)
	{
		return mSceneLights[index];
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