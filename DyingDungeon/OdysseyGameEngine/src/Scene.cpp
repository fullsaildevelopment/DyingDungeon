#include "Scene.h"
#include "BufferManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "RenderPass.h"
#include "RenderPipelineManager.h"
#include "ParticleSystem.h"
#include "Buffer.h"
#include "Component.h"

namespace Odyssey
{
	Scene::Scene()
	{
		mLightingBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(SceneLighting), nullptr);
		mShaderMatrixBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(ShaderMatrix), nullptr);
	}

	void Scene::addLight(std::shared_ptr<Light> light)
	{
		mSceneLights.push_back(light);
	}

	void Scene::addSceneObject(std::shared_ptr<GameObject> sceneObject)
	{
		mSceneObjectList.push_back(sceneObject);
	}

	void Scene::initialize()
	{
		mXTimer.Restart();
		for (std::shared_ptr<GameObject> sceneObject : mSceneObjectList)
		{
			for (auto* component : sceneObject->getComponents<Component>())
			{
				component->initialize(sceneObject.get());
			}

			for (auto child : sceneObject->getChildren())
			{
				for (auto component : child->getComponents<Component>())
				{
					component->initialize(child.get());
				}
			}
		}

		// Update the render args lists
		renderArgs.shaderMatrixBuffer = mShaderMatrixBuffer;
		renderArgs.camera = &mMainCamera;
		renderArgs.lightList = mSceneLights;
		renderArgs.renderList = mSceneObjectList;
	}

	void Scene::update()
	{
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Updates the lighting buffer
		updateLightingBuffer();

		// Update the scene
		updateScene();

		// Render the scene
		RenderPipelineManager::getInstance().render(renderArgs);
	}

	double Scene::getDeltaTime()
	{
		return mDeltaTime;
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

	void Scene::updateLightingBuffer()
	{
		// Generate a list of lights on a per-object basis
		SceneLighting sceneLighting;
		sceneLighting.numLights = 0;

		// Set the camera's position for specular highlighting
		sceneLighting.camPos = DirectX::XMFLOAT3(mMainCamera.getViewMatrix().m[3][0], mMainCamera.getViewMatrix().m[3][1], mMainCamera.getViewMatrix().m[3][2]);

		// Iterate through the entire scene light list
		for (int i = 0; i < mSceneLights.size(); i++)
		{
			// Directional and spot lights are automatically added to the light list
			sceneLighting.sceneLights[sceneLighting.numLights] = *(mSceneLights[i]);
			sceneLighting.numLights++;
		}

		// Set the lighting constant buffer
		mLightingBuffer->updateData(&sceneLighting);
		mLightingBuffer->bind(1, ShaderType::PixelShader);
	}
}