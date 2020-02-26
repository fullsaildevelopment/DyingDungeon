#pragma once
#include "Event.h"
#include "Keycode.h"
#include <memory>
#include <string>
#include <DirectXMath.h>

namespace Odyssey
{
	class MouseInputEvent : public Event
	{
	public:
		int mouseX, mouseY;

		MouseInputEvent(int xPos, int yPos) : mouseX(xPos), mouseY(yPos)
		{
			priority = EventPriority::Immediate;
		}
	};

	class MouseMoveEvent : public Event
	{
	public:
		int mouseX, mouseY;

		MouseMoveEvent(int xPos, int yPos) : mouseX(xPos), mouseY(yPos)
		{
			priority = EventPriority::Deferred;
		}
	};

	class KeypressEvent : public Event
	{
	public:
		KeyCode keyCode;

		KeypressEvent(KeyCode code) : keyCode(code)
		{
			priority = EventPriority::Immediate;
		}
	};

	class KeyUpEvent : public Event
	{
	public:
		KeyCode keyCode;
		KeyUpEvent(KeyCode code) : keyCode(code)
		{
			priority = EventPriority::Immediate;
		}
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
			priority = EventPriority::Immediate;
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
			priority = EventPriority::Immediate;
		}
	};

	class ShutdownApplicationEvent : public Event
	{
	public:
		ShutdownApplicationEvent()
		{
			priority = EventPriority::Immediate;
		}

	};

	class EngineShutdownEvent : public Event
	{
	public:
		EngineShutdownEvent()
		{
			priority = EventPriority::Immediate;
		}
	};

	class MouseClickEvent : public Event
	{
	public:
		int xPosition;
		int yPosition;

		MouseClickEvent(int xPos, int yPos) : xPosition(xPos), yPosition(yPos)
		{
			priority = EventPriority::Immediate;
		}
	};

	class UICanvas;
	class DebugEngine : public Event
	{
	public:
		std::shared_ptr<UICanvas> canvas;

		DebugEngine(std::shared_ptr<UICanvas> debugCanvas) : canvas(debugCanvas)
		{
			priority = EventPriority::Immediate;
		}
	};

	class UIScaleEvent : public Event
	{
	public:
		float* xScale;
		float* yScale;

		UIScaleEvent(float* screenX, float* screenY) : xScale(screenX), yScale(screenY)
		{
			priority = EventPriority::Immediate;
		}
	};

	class Entity;

	class SpawnEntityEvent : public Event
	{
	public:
		Entity* prefab;
		Entity** entity;
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR rotation;

		SpawnEntityEvent(Entity* spawnPrefab, Entity** outEntity, DirectX::XMVECTOR spawnPosition, DirectX::XMVECTOR spawnRotation) 
			: prefab(spawnPrefab), entity(outEntity), position(spawnPosition), rotation(spawnRotation)
		{
			priority = EventPriority::Immediate;
		}
	};

	class DestroyEntityEvent : public Event
	{
	public:
		Entity* entity;

		DestroyEntityEvent(Entity* spawnEntity) : entity(spawnEntity)
		{
			priority = EventPriority::Immediate;
		}
	};

	class CreatePrefabEvent : public Event
	{
	public:
		Entity** entity;

		CreatePrefabEvent(Entity** prefabEntity) : entity(prefabEntity)
		{
			priority = EventPriority::Immediate;
		}
	};

	class ChangeMouseCursorEvent : public Event
	{
	public:
		const wchar_t* filename;

		ChangeMouseCursorEvent(const wchar_t* file) : filename(file)
		{
			priority = EventPriority::Immediate;
		}
	};
}