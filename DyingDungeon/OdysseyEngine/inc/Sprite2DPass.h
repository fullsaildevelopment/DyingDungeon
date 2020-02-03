#pragma once
#include "EngineIncludes.h"
#include "EngineEvents.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderDevice;

	class Sprite2DPass : public RenderPass
	{
	public:
		Sprite2DPass(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<RenderWindow> targetWindow);
		virtual ~Sprite2DPass() = default;
	public:
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
		void onDebugEngine(DebugEngine* evnt);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> mContext;
		std::shared_ptr<UICanvas> mDebugCanvas;
	};
}