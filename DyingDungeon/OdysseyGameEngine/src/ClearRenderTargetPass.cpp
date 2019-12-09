#include "ClearRenderTargetPass.h"
#include "RenderTarget.h"

namespace Odyssey
{
	ClearRenderTargetPass::ClearRenderTargetPass(RenderDevice& renderDevice, std::shared_ptr<RenderTarget> renderTarget, bool clearDepth)
		: mDevice(renderDevice)
	{
		mRenderTarget = renderTarget;
		mClearDepth = clearDepth;
	}

	void ClearRenderTargetPass::render(RenderArgs& args)
	{
		// Bind the render target
		mRenderTarget->bind();
		// Clear the render target
		mRenderTarget->clearRenderView();
		// Check if depth should be cleared, if so, clear it
		if (mClearDepth)
		{
			mRenderTarget->clearDepth();
		}
	}
}