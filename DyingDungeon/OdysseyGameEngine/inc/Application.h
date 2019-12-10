#pragma once
#include "EngineIncludes.h"
#include "RenderDevice.h"

namespace Odyssey
{
	class RenderWindow;
	class Scene;

	class Application
	{
	public:
		Application();
		std::shared_ptr<RenderWindow> createRenderWindow(const std::string& title, int windowWidth, int windowHeight);
		void setActiveScene(std::shared_ptr<Scene> scene);
		int update();
		void stop();
		RenderDevice* getRenderDevice();
		HINSTANCE GetModuleHandle() const;
	private:
		void updateInput();
	private:
		std::unique_ptr<RenderDevice> mRenderDevice;
		std::vector<std::shared_ptr<Scene>> mScenes;
		std::shared_ptr<Scene> mActiveScene;
		std::shared_ptr<RenderWindow> mActiveWindow;
		HINSTANCE mHandleInstance;
		bool mIsRunning;
	};
}