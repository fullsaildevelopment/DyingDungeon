#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class Buffer;
	class Camera;
	class Light;
	class GameObject;
	class RenderWindow;
	class UICanvas;

	struct RenderArgs
	{
		GameObject* camera;
		PerFrameBuffer perFrame;
		Buffer* perFrameBuffer;
		PerObjectBuffer perObject;
		Buffer* perObjectBuffer;
		std::vector<std::shared_ptr<GameObject>> renderList;
		std::vector<std::shared_ptr<GameObject>> transparentList;
		std::vector<std::shared_ptr<Light>> lightList;
		std::vector<UICanvas*> canvasList;
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