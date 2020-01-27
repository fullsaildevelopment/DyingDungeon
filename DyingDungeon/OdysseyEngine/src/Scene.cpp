#include "Camera.h"
#include "Component.h"
#include "Entity.h"
#include "UICanvas.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "RenderDevice.h"

namespace Odyssey
{
	void Scene::addLight(std::shared_ptr<Light> light)
	{
		// Add the light to the light vector
		mSceneLights.push_back(light);
	}

	void Scene::addEntity(std::shared_ptr<Entity> entity)
	{
		// Add the entity to the entity vector
		mSceneEntities.push_back(entity);

		for (Component* component : entity->getComponents<Component>())
		{
			mComponentList.push_back(component);

			if (component->isClassType(MeshRenderer::Type))
			{
				mRenderList.push_back(static_cast<MeshRenderer*>(component));
			}

			if (component->isClassType(UICanvas::Type))
			{
				// Add it to the vector of UI canvas objects
				mSceneCanvas.push_back(static_cast<UICanvas*>(component));

				for (UIElement* element : static_cast<UICanvas*>(component)->getElements<UIElement>())
				{
					mElementList.push_back(element);
				}
			}

			if (component->isClassType(ParticleSystem::Type))
			{
				mSystemList.push_back(static_cast<ParticleSystem*>(component));
			}
		}

		for (std::shared_ptr<Entity> child : entity->getChildren())
		{
			addEntity(child);
		}

		// Check if the entity has a camera component
		if (entity->getComponent<Camera>())
		{
			// Set this as the main camera
			mMainCamera = entity;
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
			return mSceneLights[index].get();
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

	void Scene::setShadowStats(std::shared_ptr<Light> shadowLight, DirectX::XMFLOAT3 sceneCenter, float sceneRadius)
	{
		mShadowLight = shadowLight;
		mSceneCenter = sceneCenter;
		mSceneRadius = sceneRadius;
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
	}
}