#include "RenderWindow.h"
#include "RenderDevice.h"
#include "RenderTarget.h"

namespace Odyssey
{
	static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);

	RenderWindow::RenderWindow(RenderDevice& renderDevice, HWND& hWnd)
	{
		// Get the window properties
		RECT mainWinRect;
		GetClientRect(hWnd, &mainWinRect);
		mMainWindow.setBounds(mainWinRect.left, mainWinRect.right, mainWinRect.top, mainWinRect.bottom);

		Microsoft::WRL::ComPtr<IDXGIFactory2> factory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory2), &factory);

		assert(!FAILED(hr));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = mMainWindow.width;
		swapChainDesc.Height = mMainWindow.height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = false;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SampleDesc = { 1,0 };
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc = {};

		swapChainFullScreenDesc.RefreshRate = QueryRefreshRate(mMainWindow.width, mMainWindow.height, true);
		swapChainFullScreenDesc.Windowed = true;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
		hr = factory->CreateSwapChainForHwnd(renderDevice.getDevice().Get(), hWnd,
			&swapChainDesc, &swapChainFullScreenDesc, nullptr, swapChain.GetAddressOf());
		assert(!FAILED(hr));

		hr = swapChain.Get()->QueryInterface<IDXGISwapChain2>(mSwapChain.GetAddressOf());
		assert(!FAILED(hr));

		mRenderTarget = renderDevice.createRenderTarget(mMainWindow.width, mMainWindow.height, true, this);

		mWindowHandle = std::make_shared<HWND>(hWnd);
	}

	void RenderWindow::present()
	{
		mSwapChain->Present(0, 0);
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> RenderWindow::getBackBuffer()
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(backBuffer.Get()), (void**)backBuffer.GetAddressOf());
		return backBuffer;
	}

	float RenderWindow::getAspectRatio()
	{
		return static_cast<float>(mMainWindow.width) / static_cast<float>(mMainWindow.height);
	}

	std::shared_ptr<RenderTarget> RenderWindow::getRenderTarget()
	{
		return mRenderTarget;
	}

	HWND* RenderWindow::getWindowHandle()
	{
		// TODO: insert return statement here
		return mWindowHandle.get();
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain2> RenderWindow::getSwapChain()
	{
		return mSwapChain;
	}

	RenderWindow::~RenderWindow()
	{
		mSwapChain->SetFullscreenState(false, nullptr);
	}

	// This function was inspired by:
	// http://www.rastertek.com/dx11tut03.html
	static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
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
