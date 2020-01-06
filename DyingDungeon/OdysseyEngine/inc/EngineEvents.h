#pragma once
#include "Event.h"
#include <string>

namespace Odyssey
{
	class CommandReceiveEvent : public Event
	{
	public:
		CommandReceiveEvent()
		{
			priority = EventPriority::Immediate;
		}
	};

	class CommandFlushEvent : public Event
	{
	public:
		CommandFlushEvent()
		{
			priority = EventPriority::Immediate;
		}
	};

	class WindowResizeEvent : public Event
	{
	public:
		unsigned int top, bottom;
		unsigned int left, right;
		unsigned int width, height;

		WindowResizeEvent(unsigned int winLeft, unsigned int winRight, unsigned int winTop, unsigned int winBottom)
		{
			top = winTop;
			bottom = winBottom;
			left = winLeft;
			right = winRight;
			width = right - left;
			height = bottom - top;
		}
	};

	class UIElementResizeEvent : public Event
	{
	public:
		float xScale, yScale;

		UIElementResizeEvent(float xScaleFactor, float yScaleFactor) : xScale(xScaleFactor), yScale(yScaleFactor)
		{

		}
	};

	class SceneChangeEvent : public Event
	{
	public:
		std::string sceneName;
		SceneChangeEvent(std::string scene) : sceneName(scene)
		{
			priority = EventPriority::Deferred;
		}
		~SceneChangeEvent() = default;
	};

	class EngineShutdownEvent : public Event
	{
	public:
		EngineShutdownEvent()
		{

		}
	};
}