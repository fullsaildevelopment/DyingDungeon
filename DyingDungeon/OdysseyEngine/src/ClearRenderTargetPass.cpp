#include "ClearRenderTargetPass.h"
#include "RenderTarget.h"
#include "SamplerState.h"
#include "RenderDevice.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	ClearRenderTargetPass::ClearRenderTargetPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> renderWindow, bool clearDepth)
		: mDevice(renderDevice)
	{
		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);
		mClearDepth = clearDepth;

		// Create and bind the sampler states
		std::shared_ptr<SamplerState> linear = renderDevice.createSamplerState(ComparisonFunc::COMPARISON_NEVER, D3D11_FILTER_ANISOTROPIC, 0);
		std::shared_ptr<SamplerState> shadow = renderDevice.createSamplerState(ComparisonFunc::COMPARISON_LESS, D3D11_FILTER_COMPARISON_ANISOTROPIC, 1);
		linear->bind();
		shadow->bind();
	}

	void ClearRenderTargetPass::render(RenderArgs& args)
	{
		// Bind the render target
		mRenderWindow->get3DRenderTarget()->bind();
		// Clear the render target
		mRenderWindow->get3DRenderTarget()->clearRenderView();
		// Check if depth should be cleared, if so, clear it
		if (mClearDepth)
		{
			mRenderWindow->get3DRenderTarget()->clearDepth();
		}
	}
}