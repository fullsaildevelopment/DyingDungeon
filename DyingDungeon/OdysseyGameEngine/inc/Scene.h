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
	class Buffer;
	class Camera;
	class RenderWindow;
	class BufferManager;
	class Light;
	class RenderTarget;
	class GameObject;
	class ParticleSystem;
	class RenderPass;
	class RenderDevice;

	class Scene
	{
	public:
		Scene(RenderDevice& renderDevice);
		void initialize();
		void addLight(std::shared_ptr<Light> light);
		void addSceneObject(std::shared_ptr<GameObject> sceneObject);
		void update();
		double getDeltaTime();
		std::shared_ptr<Light> getLight(int index);
	private:
		void updateScene();
	public:
		std::shared_ptr<GameObject> mMainCamera;
	private:
		std::vector<std::shared_ptr<GameObject>> mSceneObjectList;
		std::vector<std::shared_ptr<Light>> mSceneLights;
		std::vector<std::shared_ptr<GameObject>> mDebugList;
		std::shared_ptr<Buffer> mPerFrameBuffer;
		std::shared_ptr<Buffer> mPerObjectBuffer;
		RenderArgs renderArgs;
		XTime mXTimer;
		double mDeltaTime;
		RenderDevice& mDevice;
	};
}