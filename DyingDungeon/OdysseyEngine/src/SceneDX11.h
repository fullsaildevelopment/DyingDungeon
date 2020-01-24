#pragma once
#include "Scene.h"
#include <vector>
#include <memory>
#include "EngineEvents.h"
#include "ReadWriteLock.h"

namespace Odyssey
{
	class Entity;
	class Component;
	class UICanvas;
	class MeshRenderer;
	class Light;
	class RenderDevice;

	class SceneDX11 : public Scene
	{
	public:
		SceneDX11(std::shared_ptr<RenderDevice> renderDevice);
		~SceneDX11() = default;

		void onComponentAdd(ComponentAddEvent* evnt);
		void onComponentRemove(ComponentRemoveEvent* evnt);
	public:
		/**
		 *	Initialize the components of each entity contained within the scene.
		 *	@param[in] void
		 *	@return void
		 */
		void initialize();

		/**
		 *	Update the components of each entity contained within the scene.
		 *	@param[in] void
		 *	@return void
		 */
		void update();

		/**
		 *	Get all lights within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all lights within the scene.
		 */
		std::vector<std::shared_ptr<Light>> getSceneLights();

		/**
		 *	Get all entities within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all entities within the scene.
		 */
		std::vector<std::shared_ptr<Entity>> getEntities();

		/**
		 *	Get all UI canvases within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all UI canvases within the scene.
		 */
		std::vector<UICanvas*> getCanvasList();

		/**
		 *	Get all mesh renderers within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all mesh renderers within the scene.
		 */
		std::vector<MeshRenderer*> getRenderList();

		/**
		 *	Get all mesh renderers within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all mesh renderers within the scene.
		 */
		std::vector<Component*> getComponentList();

		/**
		 *	Get all mesh renderers within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all mesh renderers within the scene.
		 */
		std::vector<UIElement*> getElementList();

		std::vector<ParticleSystem*> getSystemList();

		Entity* getSkybox();

		std::shared_ptr<Light> getShadowLight();

		DirectX::XMFLOAT3 getSceneCenter();

		float getSceneRadius();
	private:
		ReadWriteLock mLock;
	};
}