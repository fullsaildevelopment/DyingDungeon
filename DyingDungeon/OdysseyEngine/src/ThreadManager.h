#pragma once
#include "EngineIncludes.h"
#include <thread>
#include <atomic>
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
		ThreadManager() = default;
	public:
		void executeSceneThread(std::shared_ptr<SceneDX11> activeScene);
		void changeActiveScene(std::shared_ptr<SceneDX11> activeScene);
	private:
		std::thread sceneThread;
		bool sceneThreadActive;
		std::atomic<bool> mSceneChanged;
	private:
		void updateScene(std::shared_ptr<SceneDX11> activeScene);
	
	};
}