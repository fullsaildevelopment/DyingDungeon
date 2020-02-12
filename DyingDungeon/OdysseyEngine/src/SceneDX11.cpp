#include "SceneDX11.h"
#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "UIElement.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "UICanvas.h"
#include "ParticleSystem.h"

namespace Odyssey
{
	SceneDX11::SceneDX11()
	{
		mSceneCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mSceneRadius = 0.0f;
		mShadowLight = nullptr;
		mRenderPackage.shadowLight = nullptr;
		setSkybox("Skybox.dds");
	}

	SceneDX11::SceneDX11(DirectX::XMFLOAT3 center, float radius)
	{
		mSceneCenter = center;
		mSceneRadius = radius;
		mShadowLight = nullptr;
		setSkybox("Skybox.dds");
	}

	void SceneDX11::initialize()
	{
		// Restart the timer
		mXTimer.Restart();

		// Iterate through each component of the entity
		for (int i = 0; i < mComponentList.size(); i++)
		{
			// Initialize the component
			mComponentList[i]->initialize();
		}

		for (Light* light : mRenderPackage.sceneLights)
		{
			if (light->getLightType() == LightType::Directional)
			{
				mShadowLight = light;
				mRenderPackage.shadowLight = mShadowLight;

				if (mSceneRadius == 0.0f)
				{
					mSceneRadius = 100.0f;
				}
			}

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

		for (int i = 0; i < mComponentList.size(); i++)
		{
			Component* component = mComponentList[i];
			if (component->isActive() && component->getEntity()->isActive())
			{
				component->update(mDeltaTime);
			}
		}
	}

	void SceneDX11::onDestroy()
	{
		for (int i = 0; i < mComponentList.size(); i++)
		{
			// Update the component
			mComponentList[i]->onDestroy();
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
		return mSkybox;
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