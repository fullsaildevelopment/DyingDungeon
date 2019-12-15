#pragma once
#include "EngineIncludes.h"
#include "Camera.h"
#include "Frustum.h"
#include "Light.h"
#include "XTime.h"
#include "RenderPass.h"

namespace Odyssey
{
	class Animator;
	class Camera;
	class Light;
	class RenderTarget;
	class GameObject;
	class ParticleSystem;
	class RenderPass;
	class RenderDevice;

	class Scene
	{
	public:
		Scene() = default;
		void initialize();
		void addLight(std::shared_ptr<Light> light);
		void addSceneObject(std::shared_ptr<GameObject> sceneObject);
		void update();
		double getDeltaTime();
		std::shared_ptr<Light> getLight(int index);
		std::vector<std::shared_ptr<Light>> getSceneLights();
		std::vector<std::shared_ptr<GameObject>> getGameObjects();
	private:
		void updateScene();
	public:
		std::shared_ptr<GameObject> mMainCamera;
	private:
		std::vector<std::shared_ptr<GameObject>> mSceneObjectList;
		std::vector<std::shared_ptr<Light>> mSceneLights;
		std::vector<std::shared_ptr<GameObject>> mDebugList;
		XTime mXTimer;
		double mDeltaTime;
	};
}