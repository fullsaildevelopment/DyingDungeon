#pragma once
#include "XTime.h"
#include <memory>
#include <vector>
#include "EngineIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	// Forward declarations
	class Component;
	class Light;
	class Entity;
	class MeshRenderer;
	class ParticleSystem;
	class UICanvas;
	class UIElement;

	class Scene
	{
	public: // Rule of 3
		Scene() = default;
		~Scene() = default;

	public: // Interface
		Entity* createEntity();

		Entity* createEntity(Entity* copyEntity);

		void addComponent(Component* component);

		void removeComponent(Component* component);

		void removeEntity(Entity* entity);
		/**
		 *	Get the delta time between frames.
		 *	@param[in] void
		 *	@return double The delta time between frames.
		 */
		double getDeltaTime();

		/**
		 *	Get the light at the specified index.
		 *	@param[in] index The index of the desired light.
		 *	@return Light* The light at the specified index.
		 */
		Light* getLight(int index);

		/**
		 *	Get the entity holding the main camera component.
		 *	@param[in] void
		 *	@return shared_ptr<Light> The light at the specified index.
		 */
		Entity* getMainCamera();

		void setActive(bool active);

		void setSkybox(const char* filename);

	protected: // Members
		Entity* mMainCamera;
		std::vector<std::shared_ptr<Entity>> mSceneEntities;
		std::vector<Component*> mComponentList;
		std::shared_ptr<Entity> mSkybox;
		Light* mShadowLight;
		DirectX::XMFLOAT3 mSceneCenter;
		float mSceneRadius;
		XTime mXTimer;
		double mDeltaTime;
		bool mActive;
		RenderPackage mRenderPackage;
		std::map<Entity*, std::vector<Component*>> mComponentMap;
	};
}