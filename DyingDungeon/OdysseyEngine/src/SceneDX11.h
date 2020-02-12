#pragma once
#include "Scene.h"
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "EngineEvents.h"
#include "ReadWriteLock.h"

namespace Odyssey
{
	class Entity;
	class Component;
	class UICanvas;
	class MeshRenderer;
	class Light;

	class SceneDX11 : public Scene
	{
	public:
		SceneDX11();
		SceneDX11(DirectX::XMFLOAT3 center, float radius);
		~SceneDX11() = default;
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

		void onDestroy();
		/**
		 *	Get all entities within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all entities within the scene.
		 */
		std::vector<std::shared_ptr<Entity>> getEntities();

		/**
		 *	Get all mesh renderers within the scene.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Light>> A vector with all mesh renderers within the scene.
		 */
		std::vector<Component*> getComponentList();

		Entity* getSkybox();

		DirectX::XMFLOAT3 getSceneCenter();

		float getSceneRadius();

		void getRenderPackage(RenderPackage& renderPackage);
	};
}