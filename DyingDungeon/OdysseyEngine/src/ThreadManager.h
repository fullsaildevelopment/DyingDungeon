#pragma once
#include "EngineIncludes.h"
#include "EngineEvents.h"
#include <thread>
#include <atomic>
#include "XTime.h"
#include "ReadWriteLock.h"

namespace Odyssey
{
	class SceneDX11;
	class RenderPipeline;

	class ThreadManager
	{
	public: // Singleton pattern
		/**
		 *	Get the singleton instance of the thread manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the thread manager.
		 */
		static ThreadManager& getInstance();
		~ThreadManager();

	private: // Singleton pattern
		ThreadManager();
	public:
		void executeSceneThread(std::shared_ptr<SceneDX11> activeScene);
		void changeActiveScene(std::shared_ptr<SceneDX11> activeScene);
		void onShutdown(EngineShutdownEvent* evnt);
		void setFrameLimit(double time);
	private:
		std::shared_ptr<SceneDX11> mActiveScene;
		std::shared_ptr<SceneDX11> mNextScene;
		std::thread sceneThread;
		bool sceneThreadActive;
		std::atomic<bool> mSceneChanged;
		std::atomic<bool> mShuttingDown;
		double mTickInterval;
		XTime mTimer;
		ReadWriteLock mLock;
	private:
		void updateScene();
	
	};
}