#include "RenderWindowDX11.h"
#include "RenderDevice.h"
#include "EventManager.h"

namespace Odyssey
{
	RenderWindowDX11::RenderWindowDX11(std::shared_ptr<RenderDevice> renderDevice, HWND& hWnd)
	{
		mRenderDevice = renderDevice;

		// Get the window properties
		RECT mainWinRect;
		GetClientRect(hWnd, &mainWinRect);
		mProperties.setBounds(mainWinRect.left, mainWinRect.right, mainWinRect.top, mainWinRect.bottom);

		Microsoft::WRL::ComPtr<IDXGIFactory2> factory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory2), &factory);

		assert(!FAILED(hr));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = mProperties.width;
		swapChainDesc.Height = mProperties.height;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.Stereo = false;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SampleDesc = { 1,0 };
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc = {};

		swapChainFullScreenDesc.RefreshRate = queryRefreshRate(mProperties.width, mProperties.height, false);
		swapChainFullScreenDesc.Windowed = true;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;

		hr = factory->CreateSwapChainForHwnd(renderDevice->getDevice().Get(), hWnd,
			&swapChainDesc, &swapChainFullScreenDesc, nullptr, swapChain.GetAddressOf());
		assert(!FAILED(hr));

		hr = swapChain.Get()->QueryInterface<IDXGISwapChain2>(mSwapChain.GetAddressOf());
		assert(!FAILED(hr));

		mWindowHandle = std::make_shared<HWND>(hWnd);

		mFactory = renderDevice->get2DFactory();

		createRenderTargets();

		// Subscribe to the resize event
		EventManager::getInstance().subscribe(this, &RenderWindowDX11::onResize);
	}

	RenderWindowDX11::~RenderWindowDX11()
	{
		mSwapChain->SetFullscreenState(false, nullptr);
	}

	std::shared_ptr<RenderTarget> RenderWindowDX11::get3DRenderTarget()
	{
		return m3DRenderTarget;
	}

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> RenderWindowDX11::get2DRenderTarget()
	{
		return mBackBuffer;
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> RenderWindowDX11::getBackBuffer()
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(backBuffer.Get()), (void**)backBuffer.GetAddressOf());
		return backBuffer;
	}

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> RenderWindowDX11::getBackBuffer2D()
	{
		return mBackBuffer;
	}

	void RenderWindowDX11::onResize(WindowResizeEvent* evnt)
	{
		// Reset the 2D and 3D render targets
		mBackBuffer.Reset();
		m3DRenderTarget.reset();

		// Clear the output merger of any render targets and flush
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		mRenderDevice->getDevice()->GetImmediateContext(context.GetAddressOf());
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		context->Flush();
		mRenderDevice->getDevice2DContext()->SetTarget(nullptr);

		// Resize the swapchain buffers
		mSwapChain->ResizeBuffers(0, evnt->width, evnt->height, DXGI_FORMAT_UNKNOWN, 0);

		mProperties.screenScaleX = (float)evnt->width / (float)mProperties.width;
		mProperties.screenScaleY = (float)evnt->height / (float)mProperties.height;

		EventManager::getInstance().publish(new UIElementResizeEvent(mProperties.screenScaleX, mProperties.screenScaleY));

		// Recreate the render targets
		createRenderTargets();
	}

	void RenderWindowDX11::present()
	{
		mSwapChain->Present(0, 0);
	}

	void RenderWindowDX11::createRenderTargets()
	{
		// Generate 3D render target
		RECT mainWinRect;
		GetClientRect(*mWindowHandle, &mainWinRect);
		mProperties.setBounds(mainWinRect.left, mainWinRect.right, mainWinRect.top, mainWinRect.bottom);

		m3DRenderTarget = mRenderDevice->createRenderTarget(mProperties.width, mProperties.height, true, this);

		// Generate 2D render target
		Microsoft::WRL::ComPtr<IDXGISurface1> backBufferSurface;

		HRESULT hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(backBufferSurface.GetAddressOf()));

		float dpi = 96.0f;

		auto properties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpi, dpi);

		mRenderDevice->getDevice2DContext()->CreateBitmapFromDxgiSurface(backBufferSurface.Get(), &properties, mBackBuffer.GetAddressOf());
		mRenderDevice->getDevice2DContext()->SetTarget(mBackBuffer.Get());
	}

	DXGI_RATIONAL RenderWindowDX11::queryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
	{
		DXGI_RATIONAL refreshRate = { 0, 1 };

		if (vsync)
		{
			Microsoft::WRL::ComPtr<IDXGIFactory> factory;
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
			Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;
			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			assert(!FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), &factory)));

			assert(!FAILED(factory->EnumAdapters(0, &adapter)));

			assert(!FAILED(adapter->EnumOutputs(0, &adapterOutput)));

			UINT numDisplayModes = 0;
			assert(!FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL)));

			displayModeList = new DXGI_MODE_DESC[numDisplayModes];
			assert(displayModeList);

			assert(!FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList)));

			// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
			for (UINT i = 0; i < numDisplayModes; ++i)
			{
				if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
				{
					refreshRate = displayModeList[i].RefreshRate;
				}
			}

			delete[] displayModeList;
		}

		return refreshRate;
	}
}