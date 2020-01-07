#pragma once
#include "XTime.h"
#include <memory>
#include <vector>

namespace Odyssey
{
	// Forward declarations
	class Component;
	class Light;
	class Entity;
	class MeshRenderer;
	class UICanvas;
	class UIElement;

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
		void addLight(std::shared_ptr<Light> light);

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

	protected: // Members
		std::shared_ptr<Entity> mMainCamera;
		std::vector<std::shared_ptr<Entity>> mSceneEntities;
		std::vector<std::shared_ptr<Light>> mSceneLights;
		std::vector<UICanvas*> mSceneCanvas;
		std::vector<MeshRenderer*> mRenderList;
		std::vector<UIElement*> mElementList;
		std::vector<Component*> mComponentList;
		XTime mXTimer;
		double mDeltaTime;
	};
}