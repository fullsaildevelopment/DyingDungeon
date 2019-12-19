#include "Sprite2DPass.h"
#include "RenderDevice.h"
#include "RenderWindow.h"
#include "UICanvas.h"

namespace Odyssey
{
	Sprite2DPass::Sprite2DPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> targetWindow)
	{
		auto options = D2D1_FACTORY_OPTIONS();
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, mFactory.GetAddressOf());
		
		
		Microsoft::WRL::ComPtr<IDXGISurface1> backBufferSurface;

		hr = targetWindow->getSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBufferSurface.GetAddressOf()));
	
		float dpi = 96.0f;

		auto properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi, dpi
		);

		mFactory->CreateDxgiSurfaceRenderTarget(backBufferSurface.Get(), &properties, mRenderTarget.GetAddressOf());
	}

	void Sprite2DPass::preRender(RenderArgs& args)
	{
		mRenderTarget->BeginDraw();
	}

	void Sprite2DPass::render(RenderArgs& args)
	{
		for (UICanvas* canvas : args.canvasList)
		{
			for (auto element : canvas->getElements<UIElement>())
			{
				element->draw(mRenderTarget);
			}
		}
		mRenderTarget->EndDraw();
	}
}