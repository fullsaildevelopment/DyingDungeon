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
	class RenderTarget;
	class GameObject;
	class ParticleSystem;
	class RenderPass;
	class RenderDevice;

	struct SceneLighting
	{
		Light sceneLights[24];
		///////////////////////////// 16 x 10 bytes
		unsigned int numLights;
		DirectX::XMFLOAT3 camPos;
		///////////////////////////// 16 bytes
	};
	class Scene
	{
	public:
		Scene();
		Scene(RenderDevice& renderDevice);
		void initialize();
		void addLight(std::shared_ptr<Light> light);
		void addSceneObject(std::shared_ptr<GameObject> sceneObject);
		void update();
		double getDeltaTime();
	private:
		void updateScene();
		void updateLightingBuffer();
	public:
		Camera mMainCamera;
	private:
		std::vector<std::shared_ptr<GameObject>> mSceneObjectList;
		std::vector<std::shared_ptr<Light>> mSceneLights;
		std::vector<std::shared_ptr<GameObject>> mDebugList;
		std::shared_ptr<Buffer> mLightingBuffer;
		std::shared_ptr<Buffer> mShaderMatrixBuffer;
		RenderArgs renderArgs;
		XTime mXTimer;
		double mDeltaTime;
		RenderDevice& mDevice;
	};
}