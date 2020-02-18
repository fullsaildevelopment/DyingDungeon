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

	Entity* SceneDX11::spawnEntity(Entity* spawnPrefab, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
	{
		// Create the entity from the prefab copy and set the scene
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(*spawnPrefab);
		if (entity->getComponent<Transform>())
		{
			entity->getComponent<Transform>()->setPosition(DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position));
			entity->getComponent<Transform>()->setRotation(DirectX::XMVectorGetX(rotation), DirectX::XMVectorGetY(rotation), DirectX::XMVectorGetZ(rotation));
		}

		// Set the entity's scene and add it to the list of entities
		entity->setScene(this);
		mSceneEntities.push_back(entity);

		// Initialize the entity's components
		for (Component* component : entity->getComponents<Component>())
		{
			component->initialize();
		}

		// Iterate through the prefab's child objects and spawn them
		for (Entity* prefabChild : spawnPrefab->getChildren())
		{
			// Copy the child entity
			std::shared_ptr<Entity> child = std::make_shared<Entity>(*prefabChild);
			child->setScene(this);
			child->setParent(entity.get());

			for (Component* childComponent : child->getComponents<Component>())
			{
				childComponent->initialize();
			}
			mSceneEntities.push_back(child);
			entity->addChild(child.get());
		}

		return entity.get();
	}

	void SceneDX11::destroyEntity(Entity* entity)
	{
		// Remove from the entity list.
		if (entity == nullptr)
			return;

		// Deactivate the entity and add it to the destroy list
		entity->setActive(false);
		entity->setVisible(false);
		mDestroyList.push_back(entity);

		// Get the MR
		if (MeshRenderer* meshRenderer = entity->getComponent<MeshRenderer>())
		{
			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (meshRenderer == mRenderPackage.renderObjects[i].meshRenderer)
				{
					mRenderPackage.renderObjects.erase(mRenderPackage.renderObjects.begin() + i);
					break;
				}
			}
		}

		// Iterate through vfx objects and compare the pointer, if they match remove that system.
		if (ParticleSystem* particleSystem = entity->getComponent<ParticleSystem>())
		{
			for (int i = 0; i < mRenderPackage.vfxObjects.size(); i++)
			{
				if (particleSystem == mRenderPackage.vfxObjects[i].system)
				{
					mRenderPackage.vfxObjects.erase(mRenderPackage.vfxObjects.begin() + i);
					break;
				}
			}
		}

		// Iterate through the canvas objects and compare the entity pointer, if they match remove that object.
		for (int i = 0; i < mRenderPackage.canvasObjects.size(); i++)
		{
			if (entity == mRenderPackage.canvasObjects[i].entity)
			{
				mRenderPackage.canvasObjects.erase(mRenderPackage.canvasObjects.begin() + i);
				break;
			}
		}

		for (Entity* child : entity->getChildren())
		{
			destroyEntity(child);
		}
	}

	void SceneDX11::update()
	{
		// Signal the timer
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Flush the destroy list
		flushDestroyList();

		// Update all components in the scene
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

	RenderPackage SceneDX11::getRenderPackage()
	{
		RenderPackage returnPackage;
		mLock.lock(LockState::Write);
		mRenderPackage.sceneCenter = mSceneCenter;
		mRenderPackage.sceneRadius = mSceneRadius;
		mRenderPackage.cameraPosition = mMainCamera->getComponent<Transform>()->getPosition();
		mRenderPackage.frustum = mMainCamera->getComponent<Camera>()->getFrustum();
		returnPackage = mRenderPackage;
		mLock.unlock(LockState::Write);
		return returnPackage;
	}
	void SceneDX11::flushDestroyList()
	{
		for (Entity* entity : mDestroyList)
		{
			for (Component* component : entity->getComponents<Component>())
			{
				removeComponent(component);
			}
		}
		mDestroyList.clear();
	}
}