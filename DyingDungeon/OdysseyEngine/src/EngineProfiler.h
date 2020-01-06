#pragma once
#include "XTime.h"
#include <string>
#include "Event.h"
#include <map>

namespace Odyssey
{
	class ThreadTickEvent : public Event
	{
	public:
		std::string threadName;
		ThreadTickEvent(std::string name) : threadName(name) { }
	};

	class RenderEvent : public Event
	{
	public:
		UINT totalObjects;
		UINT rendered;
		UINT culled;
		RenderEvent(UINT numObjects, UINT numRendered, UINT numCulled) : totalObjects(numObjects), rendered(numRendered), culled(numCulled) { }
	};

	class EngineProfiler
	{
	private: // Class structs
		struct ThreadStats
		{
			std::string threadName;
			XTime timer;
			double lastFrame;
			double currentFrame;
			UINT framesPerSecond;
			UINT frameCounter;
		};
		struct RenderStats
		{
			XTime timer;
			UINT objectsInScene;
			UINT numRendered;
			UINT numCulled;
			double renderTime;
		};
		
	public: // Rule of 3
		EngineProfiler();

	public: // Events
		void onThreadTick(ThreadTickEvent* evnt);
		void onOpaqueRender(RenderEvent* evnt);

	private: // Members
		std::map<std::string, ThreadStats> statsMap;
		RenderStats opaqueStats;
		bool mOutputEnabled;
	};
}