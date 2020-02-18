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
		// Create the entity with this scene
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(this);

		// Add it to the list of entities
		mSceneEntities.push_back(entity);

		// Return the created entity
		return entity.get();
	}

	void Scene::addComponent(Component* component)
	{
		// Get the component's entity
		Entity* entity = component->getEntity();

		// Add the component to the list and map
		mComponentList.push_back(component);

		// Check if this component is an animator
		if (component->isClassType(Animator::Type))
		{
			// Animators will check their child render objects and update their animator stats
			for (Entity* child : entity->getChildren())
			{
				mLock.lock(LockState::Write);
				// Check if we have added this entity as a render object before
				for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
				{
					if (mRenderPackage.renderObjects[i].entity == child)
					{
						// Update the contents of the child's render object
						mRenderPackage.renderObjects[i].animator = child->getRootComponent<AnimatorDX11>();
					}
				}
				mLock.unlock(LockState::Write);
			}
		}

		// Check if this entity is renderable
		if (entity->getComponent<MeshRenderer>())
		{
			// Create a render object
			RenderObject* renderObject = nullptr;

			mLock.lock(LockState::Write);
			// Iterate through the render objects and look for the matching entity
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				// Check if this entity already has a render object
				if (mRenderPackage.renderObjects[i].entity == entity)
				{
					// Get the reference to the stored render object
					renderObject = &mRenderPackage.renderObjects[i];
					break;
				}
			}

			// Check if we did not find a matching render object
			if (renderObject == nullptr)
			{
				// Create an empty render object and assign it to as current render object
				mRenderPackage.renderObjects.push_back(RenderObject());
				renderObject = &mRenderPackage.renderObjects[mRenderPackage.renderObjects.size() - 1];

				// Set the new render object's entity
				renderObject->entity = entity;
			}

			// Update the render object's components
			renderObject->aabb = entity->getComponent<AABB>();
			renderObject->animator = entity->getRootComponent<AnimatorDX11>();
			renderObject->meshRenderer = entity->getComponent<MeshRenderer>();
			renderObject->transform = entity->getComponent<Transform>();

			//// Check if this is an animator component
			//if (component->isClassType(Animator::Type))
			//{
			//	// Update the render object's components
			//	renderObject->aabb = entity->getComponent<AABB>();
			//	renderObject->animator = entity->getComponent<AnimatorDX11>();
			//	renderObject->meshRenderer = entity->getComponent<MeshRenderer>();
			//	renderObject->transform = entity->getComponent<Transform>();
			//}

			//if (component->isClassType(AABB::Type))
			//{
			//	// Update the render object's components
			//	renderObject->aabb = static_cast<AABB*>(component);
			//	renderObject->animator = entity->getComponent<AnimatorDX11>();
			//	renderObject->meshRenderer = entity->getComponent<MeshRenderer>();
			//	renderObject->transform = entity->getComponent<Transform>();
			//}

			//if (component->isClassType(MeshRenderer::Type))
			//{
			//	// Update the render object's components
			//	renderObject->aabb = entity->getComponent<AABB>();
			//	renderObject->animator = entity->getComponent<AnimatorDX11>();
			//	renderObject->meshRenderer = static_cast<MeshRenderer*>(component);
			//	renderObject->transform = entity->getComponent<Transform>();
			//}

			//if (component->isClassType(Transform::Type))
			//{
			//	// Update the render object's components
			//	renderObject->aabb = entity->getComponent<AABB>();
			//	renderObject->animator = entity->getComponent<AnimatorDX11>();
			//	renderObject->meshRenderer = entity->getComponent<MeshRenderer>();
			//	renderObject->transform = static_cast<Transform*>(component);
			//}
			mLock.unlock(LockState::Write);
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

			mLock.lock(LockState::Write);
			mRenderPackage.canvasObjects.push_back(canvasObject);
			mLock.unlock(LockState::Write);
		}

		if (component->isClassType(ParticleSystem::Type))
		{
			ParticleSystem* particleSystem = static_cast<ParticleSystem*>(component);

			VFXObject vfxObject;
			vfxObject.system = particleSystem;
			vfxObject.transform = entity->getComponent<Transform>();

			mLock.lock(LockState::Write);
			mRenderPackage.vfxObjects.push_back(vfxObject);
			mLock.unlock(LockState::Write);
		}

		if (component->isClassType(Light::Type))
		{
			Light* light = static_cast<Light*>(component);

			mLock.lock(LockState::Write);
			mRenderPackage.sceneLights.push_back(light);
			mLock.unlock(LockState::Write);
		}

		// Check if the entity has a camera component
		if (Camera* camera = entity->getComponent<Camera>())
		{
			// Set this as the main camera
			mMainCamera = entity;

			mLock.lock(LockState::Write);
			mRenderPackage.camera = camera;
			mLock.unlock(LockState::Write);
		}
	}

	void Scene::addElement(UIElement* element)
	{
		mLock.lock(LockState::Write);
		for (int i = 0; i < mRenderPackage.canvasObjects.size(); i++)
		{
			if (element->getCanvas() == mRenderPackage.canvasObjects[i].canvas)
			{
				mRenderPackage.canvasObjects[i].elements.push_back(element);
			}
		}
		mLock.unlock(LockState::Write);
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

		// Check if the component is a mesh renderer
		if (component->isClassType(MeshRenderer::Type))
		{
			MeshRenderer* meshRenderer = static_cast<MeshRenderer*>(component);

			mLock.lock(LockState::Write);
			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (meshRenderer == mRenderPackage.renderObjects[i].meshRenderer)
				{
					mRenderPackage.renderObjects.erase(mRenderPackage.renderObjects.begin() + i);
					break;
				}
			}
			mLock.unlock(LockState::Write);
		}

		// Check if the component is a transform
		if (component->isClassType(Transform::Type))
		{
			Transform* transform = static_cast<Transform*>(component);

			mLock.lock(LockState::Write);
			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (transform == mRenderPackage.renderObjects[i].transform)
				{
					mRenderPackage.renderObjects.erase(mRenderPackage.renderObjects.begin() + i);
					break;
				}
			}
			mLock.unlock(LockState::Write);
		}

		// Check if the component is a transform
		if (component->isClassType(AnimatorDX11::Type))
		{
			AnimatorDX11* animator = static_cast<AnimatorDX11*>(component);

			mLock.lock(LockState::Write);
			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (animator == mRenderPackage.renderObjects[i].animator)
				{
					mRenderPackage.renderObjects[i].animator = nullptr;
					break;
				}
			}
			mLock.unlock(LockState::Write);
		}

		// Check if the component is a transform
		if (component->isClassType(AABB::Type))
		{
			AABB* aabb = static_cast<AABB*>(component);

			mLock.lock(LockState::Write);
			// Iterate through render objects and compare the pointer, if they match remove that render object.
			for (int i = 0; i < mRenderPackage.renderObjects.size(); i++)
			{
				if (aabb == mRenderPackage.renderObjects[i].aabb)
				{
					mRenderPackage.renderObjects[i].aabb = nullptr;
					break;
				}
			}
			mLock.unlock(LockState::Write);
		}

		if (component->isClassType(UICanvas::Type))
		{
			mLock.lock(LockState::Write);
			// Iterate through the canvas objects and compare the entity pointer, if they match remove that object.
			for (int i = 0; i < mRenderPackage.canvasObjects.size(); i++)
			{
				if (mRenderPackage.canvasObjects[i].entity == entity)
				{
					mRenderPackage.canvasObjects.erase(mRenderPackage.canvasObjects.begin() + i);
					break;
				}
			}
			mLock.unlock(LockState::Write);
		}

		// Iterate through vfx objects and compare the pointer, if they match remove that system.
		if (component->isClassType(ParticleSystem::Type))
		{
			ParticleSystem* particleSystem = static_cast<ParticleSystem*>(component);

			mLock.lock(LockState::Write);
			for (int i = 0; i < mRenderPackage.vfxObjects.size(); i++)
			{
				if (particleSystem == mRenderPackage.vfxObjects[i].system)
				{
					mRenderPackage.vfxObjects.erase(mRenderPackage.vfxObjects.begin() + i);
					break;
				}
			}
			mLock.unlock(LockState::Write);
		}

		if (component->isClassType(Light::Type))
		{
			Light* light = static_cast<Light*>(component);

			mLock.lock(LockState::Write);
			auto iter2 = std::find(mRenderPackage.sceneLights.begin(), mRenderPackage.sceneLights.end(), light);

			if (mRenderPackage.shadowLight == light)
			{
				mRenderPackage.shadowLight = nullptr;
			}

			if (iter2 != mRenderPackage.sceneLights.end())
			{
				mRenderPackage.sceneLights.erase(iter2);
			}
			mLock.unlock(LockState::Write);
		}

		// Check if the entity has a camera component
		if (component->isClassType(Camera::Type))
		{
			Camera* camera = static_cast<Camera*>(component);

			mLock.lock(LockState::Write);
			if (camera == mRenderPackage.camera)
			{
				mRenderPackage.camera = nullptr;
			}
			mLock.unlock(LockState::Write);
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
		Light* light = nullptr;

		mLock.lock(LockState::Read);
		if (mRenderPackage.sceneLights.size() > index)
		{
			// Get the light at the parameter index
			light = mRenderPackage.sceneLights[index];
		}

		mLock.unlock(LockState::Read);
		// Return nullptr by default
		return light;
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

		mSkybox = createEntity();
		mSkybox->setScene(nullptr);
		mSkybox->addComponent<MeshRenderer>(mesh, material);
		mSkybox->addComponent<Transform>();
		mSkybox->getComponent<Transform>()->setRotation(0, 45.0f, 0);

		mLock.lock(LockState::Write);
		mRenderPackage.skybox.meshRenderer = mSkybox->getComponent<MeshRenderer>();
		mRenderPackage.skybox.transform = mSkybox->getComponent<Transform>();
		mLock.unlock(LockState::Write);
	}
}