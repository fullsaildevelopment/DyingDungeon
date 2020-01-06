#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderDevice;

	class ClearRenderTargetPass : public RenderPass
	{
	public:
		ClearRenderTargetPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> renderWindow, bool clearDepth);
		virtual void render(RenderArgs& args);
	private:
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
		bool mClearDepth;
		RenderDevice& mDevice;
	};
}