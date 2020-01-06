#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class Buffer;
	class Camera;
	class Light;
	class Entity;
	class MeshRenderer;
	class RenderWindow;
	class UICanvas;
	class UIElement;

	struct RenderArgs
	{
		Entity* camera;
		PerFrameBuffer perFrame;
		Buffer* perFrameBuffer;
		PerObjectBuffer perObject;
		Buffer* perObjectBuffer;
		std::vector<std::shared_ptr<Entity>> entityList;
		std::vector<std::shared_ptr<Entity>> transparentList;
		std::vector<std::shared_ptr<Light>> lightList;
		std::vector<UIElement*> elementList;
		std::vector<MeshRenderer*> renderList;
	};

	class RenderPass
	{
	public:
		RenderPass();
		virtual ~RenderPass();
		virtual void setEnabled(bool enable);
		virtual bool isEnabled() const;
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args) = 0;
		virtual void postRender(RenderArgs& args);
	protected:
		virtual void updatePerFrameBuffer(PerFrameBuffer& perFrame, Buffer* frameBuffer);
		virtual void updatePerObjectBuffer(PerObjectBuffer& perObject, Buffer* objectBuffer);
	private:
		bool mEnabled;
	};
}