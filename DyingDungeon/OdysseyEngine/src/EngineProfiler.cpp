#include "EngineProfiler.h"
#include "EventManager.h"
#include <iostream>
#include "InputManager.h"

namespace Odyssey
{
	EngineProfiler::EngineProfiler()
	{
		// Subscribe to the thread tick event
		EventManager::getInstance().subscribe(this, &EngineProfiler::onThreadTick);

		// Subscribe to the opaque render event
		EventManager::getInstance().subscribe(this, &EngineProfiler::onOpaqueRender);

		mOutputStats = true;
	}

	void EngineProfiler::onThreadTick(ThreadTickEvent* evnt)
	{
		if (statsMap.count(evnt->threadName) == 0)
		{
			statsMap[evnt->threadName] = ThreadStats();
			statsMap[evnt->threadName].threadName = evnt->threadName;
		}
		
		ThreadStats& stats = statsMap[evnt->threadName];
		stats.timer.Signal();
		stats.frameCounter++;
		stats.lastFrame = stats.currentFrame;
		stats.currentFrame = stats.timer.TotalTime();
		
		double time = stats.timer.TotalTime();
		if (time >= 1.0)
		{
			stats.timer.Restart();
			stats.framesPerSecond = stats.frameCounter;
			stats.frameCounter = 0;

			if (mOutputStats)
			{
				std::cout << "========================" << std::endl;
				std::cout << stats.threadName << " FPS: " << stats.framesPerSecond << std::endl;
				std::cout << "========================" << std::endl << std::endl;
			}
		}
	}

	void EngineProfiler::onOpaqueRender(RenderEvent* evnt)
	{
		opaqueStats.timer.Signal();
		
		opaqueStats.objectsInScene = evnt->totalObjects;
		opaqueStats.numRendered = evnt->rendered;
		opaqueStats.numCulled = evnt->culled;
		opaqueStats.renderTime = opaqueStats.timer.Delta() * 1000.0;
		
		double time = opaqueStats.timer.TotalTime();
		
		if (time >= 1.0)
		{
			opaqueStats.timer.Restart();
			if (mOutputStats && false)
			{
				std::cout << "========================" << std::endl;
				std::cout << "Total Render time: " << opaqueStats.renderTime << " ms" << std::endl;
				std::cout << "Render FPS: " << statsMap["Main Thread"].framesPerSecond << " fps" << std::endl;
				std::cout << "Objects in scene: " << opaqueStats.objectsInScene << std::endl;
				std::cout << "Objects rendered: " << opaqueStats.numRendered << std::endl;
				std::cout << "Objects culled: " << opaqueStats.numCulled << std::endl;
				std::cout << "========================" << std::endl << std::endl;
			}
		}
	}
}