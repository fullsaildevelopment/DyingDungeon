#include "ClearRenderTargetPass.h"
#include "RenderTarget.h"
#include "SamplerState.h"
#include "RenderManager.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	ClearRenderTargetPass::ClearRenderTargetPass()
	{
		
	}

	void ClearRenderTargetPass::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, RenderWindowDX11* activeWindow, RenderArgs& args, RenderPackage& renderPackage)
	{
		// Bind the render target
		activeWindow->get3DRenderTarget()->bind(context);

		// Clear the render target
		activeWindow->get3DRenderTarget()->clearRenderView(context);
		activeWindow->get3DRenderTarget()->clearDepth(context);
	}
}