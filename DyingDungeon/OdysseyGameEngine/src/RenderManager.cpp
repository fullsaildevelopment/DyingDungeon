#include "RenderManager.h"

namespace Odyssey
{
	RenderManager& RenderManager::getInstance()
	{
		static RenderManager instance;

		return instance;
	}

	void RenderManager::initialize(HWND& hWnd, Microsoft::WRL::ComPtr<IDXGISwapChain>& outSwapChain)
	{
		RECT mainWinRect;
		GetClientRect(hWnd, &mainWinRect);

		// Setup the swap chain description
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL dx11 = D3D_FEATURE_LEVEL_11_0;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Width = static_cast<UINT>(mainWinRect.right - mainWinRect.left);
		swapChainDesc.BufferDesc.Height = static_cast<UINT>(mainWinRect.bottom - mainWinRect.top);
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Create the swapchain, device and context
		HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &dx11, 1, D3D11_SDK_VERSION,
			&swapChainDesc, &mSwapChain, mDevice.GetAddressOf(), 0, mDeviceContext.GetAddressOf());

		assert(!FAILED(hr));

		// Set the out swap chain parameter to the created swap chain
		outSwapChain = mSwapChain;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> RenderManager::getDevice()
	{
		return mDevice;
	}
}