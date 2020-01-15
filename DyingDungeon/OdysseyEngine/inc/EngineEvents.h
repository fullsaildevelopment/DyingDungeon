#pragma once
#include "Event.h"
#include "Keycode.h"
#include <memory>
#include <string>

namespace Odyssey
{
	class KeypressEvent : public Event
	{
	public:
		KeyCode keyCode;
		KeypressEvent(KeyCode code) : keyCode(code) { }
	};

	class KeyUpEvent : public Event
	{
	public:
		KeyCode keyCode;
		KeyUpEvent(KeyCode code) : keyCode(code) { }
	};

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

	class UIElementResizeEvent : public Event
	{
	public:
		float xScale, yScale;

		UIElementResizeEvent(float xScaleFactor, float yScaleFactor) : xScale(xScaleFactor), yScale(yScaleFactor)
		{

		}
	};

	class EngineShutdownEvent : public Event
	{
	public:
		EngineShutdownEvent()
		{

		}
	};

	class MouseClickEvent : public Event
	{
	public:
		int xPosition;
		int yPosition;

		MouseClickEvent(int xPos, int yPos) : xPosition(xPos), yPosition(yPos)
		{

		}
	};

	class UICanvas;
	class DebugEngine : public Event
	{
	public:
		std::shared_ptr<UICanvas> canvas;

		DebugEngine(std::shared_ptr<UICanvas> debugCanvas) : canvas(debugCanvas)
		{

		}
	};

	class Component;
	class ComponentAddEvent : public Event
	{
	public:
		Component* component;

		ComponentAddEvent(Component* componentAdded) : component(componentAdded)
		{

		}
	};

	class ComponentRemoveEvent : public Event
	{
	public:
		Component* component;

		ComponentRemoveEvent(Component* componentRemoved) : component(componentRemoved)
		{

		}
	};
}