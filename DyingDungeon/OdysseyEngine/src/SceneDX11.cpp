#include "SceneDX11.h"
#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "UIElement.h"
#include "EventManager.h"
#include "RenderDevice.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

namespace Odyssey
{
	SceneDX11::SceneDX11(std::shared_ptr<RenderDevice> renderDevice)
	{
		mSceneCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mSceneRadius = 0.0f;
		mRenderDevice = renderDevice;
		mShadowLight = nullptr;
		mRenderPackage.shadowLight = nullptr;
		setSkybox("Skybox.dds");
	}

	SceneDX11::SceneDX11(std::shared_ptr<RenderDevice> renderDevice, DirectX::XMFLOAT3 center, float radius)
	{
		mSceneCenter = center;
		mSceneRadius = radius;
		mRenderDevice = renderDevice;
		mShadowLight = nullptr;
		setSkybox("Skybox.dds");
	}

	void SceneDX11::initialize()
	{
		// Restart the timer
		mXTimer.Restart();

		// Iterate through each component of the entity
		for (Component* component : mComponentList)
		{
			// Initialize the component
			component->initialize();
		}

		for (Light* light : mRenderPackage.sceneLights)
		{
			light->initialize();
		}
	}

	void SceneDX11::update()
	{
		// Signal the timer
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Maybe an optimization for creation/destruction?
		//for (std::pair<std::shared_ptr<Entity>, std::vector<Component*>> pair : mComponentMap)
		//{
		//	if (pair.first->isActive())
		//	{
		//		for (Component* component : pair.second)
		//		{
		//			if (component->isActive())
		//			{
		//				component->update(mDeltaTime);
		//			}
		//		}
		//	}
		//}
		for (auto* component : mComponentList)
		{
			if (component->isActive() && component->getEntity()->isActive())
			{
				// Update the component
				component->update(mDeltaTime);
			}
		}
	}

	void SceneDX11::onDestroy()
	{
		for (auto* component : mComponentList)
		{
			// Update the component
			component->onDestroy();
		}
	}

	std::vector<std::shared_ptr<Entity>> SceneDX11::getEntities()
	{
		// Return the scene entity vector
		return mSceneEntities;
	}

	std::vector<Component*> SceneDX11::getComponentList()
	{
		return mComponentList;
	}

	Entity* SceneDX11::getSkybox()
	{
		if (mSkybox == nullptr)
		{
			std::shared_ptr<Texture> texture = mRenderDevice->createTexture(TextureType::Skybox, "Skybox.dds");
			std::shared_ptr<Mesh> mesh = mRenderDevice->createCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
			std::shared_ptr<Material> material = mRenderDevice->createMaterial(TextureType::Skybox, texture);

			mSkybox = std::make_shared<Entity>();
			mSkybox->addComponent<MeshRenderer>(mesh, material);
			mSkybox->addComponent<Transform>();
			mSkybox->getComponent<Transform>()->setRotation(0, 45.0f, 0);
		}
		return mSkybox.get();
	}

	DirectX::XMFLOAT3 SceneDX11::getSceneCenter()
	{
		return mSceneCenter;
	}

	float SceneDX11::getSceneRadius()
	{
		return mSceneRadius;
	}

	void SceneDX11::getRenderPackage(RenderPackage& renderPackage)
	{
		mRenderPackage.sceneCenter = mSceneCenter;
		mRenderPackage.sceneRadius = mSceneRadius;
		mRenderPackage.cameraPosition = mMainCamera->getComponent<Transform>()->getPosition();
		mRenderPackage.frustum = mMainCamera->getComponent<Camera>()->getFrustum();
		renderPackage = mRenderPackage;
	}
}