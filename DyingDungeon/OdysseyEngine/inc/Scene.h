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
	class RenderDevice;

	class Scene
	{
	public: // Rule of 3
		Scene() = default;
		~Scene() = default;

	public: // Interface
		/**
		 *	Add a light object to the scene.
		 *	@param[in] light The light object to add.
		 *	@return void
		 */
		void addLight(std::shared_ptr<Entity> light);

		/**
		 *	Add an entity object to the scene
		 *	@param[in] entity The entity object to add
		 *	@return void
		 */
		void addEntity(std::shared_ptr<Entity> entity);

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
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<Entity> mMainCamera;
		std::vector<std::shared_ptr<Entity>> mSceneEntities;
		std::vector<Light*> mSceneLights;
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