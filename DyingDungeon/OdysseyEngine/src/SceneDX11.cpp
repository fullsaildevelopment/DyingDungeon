#include "SceneDX11.h"
#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "UIElement.h"
#include "EventManager.h"
#include "RenderDevice.h"
#include "Texture.h"
#include "Transform.h"

namespace Odyssey
{
	SceneDX11::SceneDX11(std::shared_ptr<RenderDevice> renderDevice)
	{
		EventManager::getInstance().subscribe(this, &SceneDX11::onComponentAdd);
		EventManager::getInstance().subscribe(this, &SceneDX11::onComponentRemove);

		mRenderDevice = renderDevice;
		mSkybox = nullptr;
		mShadowLight = nullptr;
	}

	void SceneDX11::onComponentAdd(ComponentAddEvent* evnt)
	{ 
		mLock.lock(LockState::Write);

		for (std::shared_ptr<Entity> entity : mSceneEntities)
		{
			if (entity.get() == evnt->component->getEntity())
			{
				mComponentList.push_back(evnt->component);
				mLock.unlock(LockState::Write);
				return;
			}
		}
		mLock.unlock(LockState::Write);
	}

	void SceneDX11::onComponentRemove(ComponentRemoveEvent* evnt)
	{
		mLock.lock(LockState::Write);
		for (int i = 0; i < mSceneEntities.size(); i++)
		{
			if (mSceneEntities[i].get() == evnt->component->getEntity())
			{
				for (int j = 0; j < mComponentList.size(); j++)
				{
					if (mComponentList[j] == evnt->component)
					{
						mComponentList.erase(mComponentList.begin() + j);
						mLock.unlock(LockState::Write);
						return;
					}
				}
			}
		}
		mLock.unlock(LockState::Write);
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
	}

	void SceneDX11::update()
	{
		// Signal the timer
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Iterate through each component of the entity
		for (auto* component : mComponentList)
		{
			if (component->isActive() && component->getEntity()->isActive())
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
	std::vector<MeshRenderer*> SceneDX11::getRenderList()
	{
		return mRenderList;
	}
	std::vector<Component*> SceneDX11::getComponentList()
	{
		return mComponentList;
	}
	std::vector<UIElement*> SceneDX11::getElementList()
	{
		return mElementList;
	}
	std::vector<ParticleSystem*> SceneDX11::getSystemList()
	{
		return mSystemList;
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

	std::shared_ptr<Light> SceneDX11::getShadowLight()
	{
		return mShadowLight;
	}

	DirectX::XMFLOAT3 SceneDX11::getSceneCenter()
	{
		return mSceneCenter;
	}

	float SceneDX11::getSceneRadius()
	{
		return mSceneRadius;
	}
}