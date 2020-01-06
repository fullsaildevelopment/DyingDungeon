#include "ThreadManager.h"
#include "SceneDX11.h"
#include <thread>
#include "RenderPipeline.h"
#include "XTime.h"
#include "EventManager.h"
#include "EngineProfiler.h"

namespace Odyssey
{
	ThreadManager& ThreadManager::getInstance()
	{
		// TODO: insert return statement here
		static ThreadManager instance;
		return instance;
	}

	ThreadManager::~ThreadManager()
	{
		if (sceneThread.joinable())
		{
			mSceneChanged = true;
			sceneThread.join();
		}
	}

	void ThreadManager::executeSceneThread(std::shared_ptr<SceneDX11> activeScene)
	{
		sceneThread = std::thread(&ThreadManager::updateScene, this, activeScene);
	}

	void ThreadManager::changeActiveScene(std::shared_ptr<SceneDX11> activeScene)
	{
		if (sceneThread.joinable())
		{
			mSceneChanged = true;
			sceneThread.join();
			mSceneChanged = false;
			executeSceneThread(activeScene);
		}
	}

	void ThreadManager::updateScene(std::shared_ptr<SceneDX11> activeScene)
	{
		while (mSceneChanged == false)
		{
			// TODO: FIX THIS
			//EventManager::getInstance().publish(new ThreadTickEvent("Scene Thread"));

			activeScene->update();
		}
	}
}