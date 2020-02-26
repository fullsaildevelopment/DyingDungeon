#include "ThreadManager.h"
#include "SceneDX11.h"
#include <thread>
#include "RenderPipeline.h"
#include "XTime.h"
#include "EventManager.h"
#include "EngineProfiler.h"
#include "EventManager.h"

namespace Odyssey
{
	ThreadManager& ThreadManager::getInstance()
	{
		// TODO: insert return statement here
		static ThreadManager instance;
		return instance;
	}

	ThreadManager::ThreadManager()
	{
		mShuttingDown = false;
		mSceneChanged = false;
		mTimer.Restart();
		mTickInterval = 0.0;
		EventManager::getInstance().subscribe(this, &ThreadManager::onShutdown);
	}

	ThreadManager::~ThreadManager()
	{
		if (sceneThread.joinable())
		{
			mSceneChanged = true;
			mShuttingDown = true;
			sceneThread.join();
		}
	}

	void ThreadManager::executeSceneThread(std::shared_ptr<SceneDX11> activeScene)
	{
		mLock.lock(LockState::Write);
		mActiveScene = activeScene;
		mLock.unlock(LockState::Write);
		sceneThread = std::thread(&ThreadManager::updateScene, this);
	}

	void ThreadManager::changeActiveScene(std::shared_ptr<SceneDX11> activeScene)
	{
		mLock.lock(LockState::Write);
		mNextScene = activeScene;
		mSceneChanged = true;
		mLock.unlock(LockState::Write);
	}

	void ThreadManager::onShutdown(EngineShutdownEvent* evnt)
	{
		mShuttingDown = true;
	}

	void ThreadManager::setFrameLimit(double time)
	{
		mTickInterval = time;
		mTimer.Restart();
	}

	void ThreadManager::updateScene()
	{
		while (mShuttingDown == false)
		{
			// Check if there is a scene change
			mLock.lock(LockState::Write);
			if (mSceneChanged)
			{
				mActiveScene = mNextScene;
				mNextScene = nullptr;
				mSceneChanged = false;
			}
			mLock.unlock(LockState::Write);

			mTimer.Signal();

			if (mTimer.TotalTime() > mTickInterval)
			{
				mTimer.Restart();

				EventManager::getInstance().publish(new ThreadTickEvent("Scene Thread"));

				mActiveScene->update();
			}
		}
	}
}