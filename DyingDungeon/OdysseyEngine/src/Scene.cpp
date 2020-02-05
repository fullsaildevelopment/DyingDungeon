#include "Camera.h"
#include "Component.h"
#include "Entity.h"
#include "UICanvas.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Light.h"
#include "AABB.h"
#include "AnimatorDX11.h"

namespace Odyssey
{
	void Scene::addLight(std::shared_ptr<Entity> light)
	{
		// Add the light to the light vector
		Light* lightPtr = light->getComponent<Light>();
		mSceneLights.push_back(lightPtr);

		if (lightPtr->getLightType() == LightType::Directional)
		{
			mShadowLight = lightPtr;
			mRenderPackage.shadowLight = mShadowLight;

			if (mSceneRadius == 0.0f)
			{
				mSceneRadius = 100.0f;
			}
		}

		mRenderPackage.sceneLights.push_back(lightPtr);
	}

	void Scene::addEntity(std::shared_ptr<Entity> entity)
	{
		// Add the entity to the entity vector
		mSceneEntities.push_back(entity);

		for (Component* component : entity->getComponents<Component>())
		{
			mComponentList.push_back(component);
			mComponentMap[entity.get()].push_back(component);

			if (component->isClassType(MeshRenderer::Type))
			{
				MeshRenderer* meshRenderer = static_cast<MeshRenderer*>(component);

				RenderObject renderObject;
				renderObject.aabb = entity->getComponent<AABB>();
				renderObject.meshRenderer = meshRenderer;
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
				canvasObject.entity = entity.get();

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

				mSceneLights.push_back(light);

				if (light->getLightType() == LightType::Directional)
				{
					mShadowLight = light;
					mRenderPackage.shadowLight = mShadowLight;

					if (mSceneRadius == 0.0f)
					{
						mSceneRadius = 100.0f;
					}
				}

				mRenderPackage.sceneLights.push_back(light);
			}
		}

		for (std::shared_ptr<Entity> child : entity->getChildren())
		{
			addEntity(child);
		}

		// Check if the entity has a camera component
		if (Camera* camera = entity->getComponent<Camera>())
		{
			// Set this as the main camera
			mMainCamera = entity;
			mRenderPackage.camera = camera;
		}
	}

	void Scene::removeEntity(Entity* entity)
	{
		// Remove from the entity list.

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

		for (std::shared_ptr<Entity> child : entity->getChildren())
		{
			removeEntity(child.get());
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
			return mSceneLights[index];
		}

		// Return nullptr by default
		return nullptr;
	}

	Entity* Odyssey::Scene::getMainCamera()
	{
		// Return the entity registered with the main camera
		return mMainCamera.get();
	}

	void Scene::setActive(bool active)
	{
		mActive = active;
	}

	void Scene::setSkybox(const char* filename)
	{
		std::shared_ptr<Texture> texture = mRenderDevice->createTexture(TextureType::Skybox, filename);
		std::shared_ptr<Mesh> mesh = mRenderDevice->createCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		std::shared_ptr<Material> material = mRenderDevice->createMaterial(TextureType::Skybox, texture);

		mSkybox = std::make_shared<Entity>();
		mSkybox->addComponent<MeshRenderer>(mesh, material);
		mSkybox->addComponent<Transform>();
		mSkybox->getComponent<Transform>()->setRotation(0, 45.0f, 0);

		mRenderPackage.skybox.meshRenderer = mSkybox->getComponent<MeshRenderer>();
		mRenderPackage.skybox.transform = mSkybox->getComponent<Transform>();
	}
}