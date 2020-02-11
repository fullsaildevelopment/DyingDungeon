#include "Camera.h"
#include "Component.h"
#include "Entity.h"
#include "UICanvas.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Light.h"
#include "AABB.h"
#include "AnimatorDX11.h"

namespace Odyssey
{
	Entity* Scene::createEntity()
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(this);
		mSceneEntities.push_back(entity);
		return entity.get();
	}

	Entity* Scene::createEntity(Entity* copyEntity)
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(*copyEntity);
		entity->setScene(this);
		mSceneEntities.push_back(entity);
		return entity.get();
	}

	void Scene::addComponent(Component* component)
	{
		Entity* entity = component->getEntity();
		mComponentList.push_back(component);
		mComponentMap[entity].push_back(component);

		if (component->isClassType(AABB::Type))
		{
			RenderObject renderObject;
			renderObject.aabb = entity->getComponent<AABB>();
			renderObject.meshRenderer = entity->getComponent<MeshRenderer>();
			renderObject.transform = entity->getComponent<Transform>();
			renderObject.animator = entity->getRootComponent<AnimatorDX11>();
			mRenderPackage.renderObjects.push_back(renderObject);
		}

		if (component->isClassType(UICanvas::Type))
		{
			// Add it to the vector of UI canvas objects
			UICanvas* canvas = static_cast<UICanvas*>(component);

			CanvasObject canvasObject;
			canvasObject.canvas = canvas;
			canvasObject.entity = entity;

			for (UIElement* element : static_cast<UICanvas*>(component)->getElements<UIElement>())
			{
				canvasObject.elements.push_back(element);
			}

			mRenderPackage.canvasObjects.push_back(canvasObject);
		}

		if (component->isClassType(ParticleSystem::Type))
		{
			ParticleSystem* particleSystem = static_cast<ParticleSystem*>(component);

			VFXObject vfxObject;
			vfxObject.system = particleSystem;
			vfxObject.transform = entity->getComponent<Transform>();
			mRenderPackage.vfxObjects.push_back(vfxObject);
		}

		if (component->isClassType(Light::Type))
		{
			Light* light = static_cast<Light*>(component);

			mRenderPackage.sceneLights.push_back(light);
		}

		// Check if the entity has a camera component
		if (Camera* camera = entity->getComponent<Camera>())
		{
			// Set this as the main camera
			mMainCamera = entity;
			mRenderPackage.camera = camera;
		}
	}

	void Scene::removeComponent(Component* component)
	{
		Entity* entity = component->getEntity();

		// Remove the component from the component list if it exists
		auto iter = std::find(mComponentList.begin(), mComponentList.end(), component);
		if (iter != mComponentList.end())
		{
			mComponentList.erase(iter);
		}

		// Remove the component from the component map if it exists
		iter = std::find(mComponentMap[entity].begin(), mComponentMap[entity].end(), component);
		if (iter != mComponentMap[entity].end())
		{
			mComponentMap[entity].erase(iter);
		}

		// Check if the component is a mesh renderer
		if (component->isClassType(MeshRenderer::Type))
		{
			MeshRenderer* meshRenderer = static_cast<MeshRenderer*>(component);

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

		// Check if the component is a transform
		if (component->isClassType(Transform::Type))
		{
			Transform* transform = static_cast<Transform*>(component);

			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (transform == mRenderPackage.renderObjects[i].transform)
				{
					mRenderPackage.renderObjects.erase(mRenderPackage.renderObjects.begin() + i);
					break;
				}
			}
		}

		// Check if the component is a transform
		if (component->isClassType(AnimatorDX11::Type))
		{
			AnimatorDX11* animator = static_cast<AnimatorDX11*>(component);

			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (animator == mRenderPackage.renderObjects[i].animator)
				{
					mRenderPackage.renderObjects[i].animator = nullptr;
					break;
				}
			}
		}

		// Check if the component is a transform
		if (component->isClassType(AABB::Type))
		{
			AABB* aabb = static_cast<AABB*>(component);

			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (aabb == mRenderPackage.renderObjects[i].aabb)
				{
					mRenderPackage.renderObjects[i].aabb = nullptr;
					break;
				}
			}
		}

		if (component->isClassType(UICanvas::Type))
		{
			// Iterate through the canvas objects and compare the entity pointer, if they match remove that object.
			for (int i = 0; i < mRenderPackage.canvasObjects.size(); i++)
			{
				if (mRenderPackage.canvasObjects[i].entity == entity)
				{
					mRenderPackage.canvasObjects.erase(mRenderPackage.canvasObjects.begin() + i);
					break;
				}
			}
		}

		// Iterate through vfx objects and compare the pointer, if they match remove that system.
		if (component->isClassType(ParticleSystem::Type))
		{
			ParticleSystem* particleSystem = static_cast<ParticleSystem*>(component);

			for (int i = 0; i < mRenderPackage.vfxObjects.size(); i++)
			{
				if (particleSystem == mRenderPackage.vfxObjects[i].system)
				{
					mRenderPackage.vfxObjects.erase(mRenderPackage.vfxObjects.begin() + i);
					break;
				}
			}
		}

		if (component->isClassType(Light::Type))
		{
			Light* light = static_cast<Light*>(component);

			auto iter2 = std::find(mRenderPackage.sceneLights.begin(), mRenderPackage.sceneLights.end(), light);

			if (mRenderPackage.shadowLight == light)
			{
				mRenderPackage.shadowLight = nullptr;
			}

			if (iter2 != mRenderPackage.sceneLights.end())
			{
				mRenderPackage.sceneLights.erase(iter2);
			}
		}

		// Check if the entity has a camera component
		if (component->isClassType(Camera::Type))
		{
			Camera* camera = static_cast<Camera*>(component);
			if (camera == mRenderPackage.camera)
			{
				mRenderPackage.camera = nullptr;
			}
		}
	}

	void Scene::removeEntity(Entity* entity)
	{
		// Remove from the entity list.
		if (entity == nullptr)
			return;

		// Get the MR
		MeshRenderer* meshRenderer = entity->getComponent<MeshRenderer>();

		if (meshRenderer)
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

		// Get the PS
		ParticleSystem* particleSystem = entity->getComponent<ParticleSystem>();

		// Iterate through vfx objects and compare the pointer, if they match remove that system.
		if (particleSystem)
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
			removeEntity(child);
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
		if (mRenderPackage.sceneLights.size() > index)
		{
			// Get the light at the parameter index
			return mRenderPackage.sceneLights[index];
		}

		// Return nullptr by default
		return nullptr;
	}

	Entity* Odyssey::Scene::getMainCamera()
	{
		// Return the entity registered with the main camera
		return mMainCamera;
	}

	void Scene::setActive(bool active)
	{
		mActive = active;
	}

	void Scene::setSkybox(const char* filename)
	{
		int texture = RenderManager::getInstance().createTexture(TextureType::Skybox, filename);
		int mesh = RenderManager::getInstance().createCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		int material = RenderManager::getInstance().createMaterial(TextureType::Skybox, texture);

		mSkybox = std::make_shared<Entity>();
		mSkybox->addComponent<MeshRenderer>(mesh, material);
		mSkybox->addComponent<Transform>();
		mSkybox->getComponent<Transform>()->setRotation(0, 45.0f, 0);

		mRenderPackage.skybox.meshRenderer = mSkybox->getComponent<MeshRenderer>();
		mRenderPackage.skybox.transform = mSkybox->getComponent<Transform>();
	}
}