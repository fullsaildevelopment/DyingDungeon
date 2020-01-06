#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderWindow.h"

namespace Odyssey
{
	class RenderDevice;
	class RenderTarget;

	class RenderWindowDX11 : public RenderWindow
	{
	public:
		RenderWindowDX11(RenderDevice& renderDevice, HWND& hWnd);
		~RenderWindowDX11();
	public:
		std::shared_ptr<RenderTarget> get3DRenderTarget();
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> get2DRenderTarget();
	public:
		Microsoft::WRL::ComPtr<ID3D11Resource> getBackBuffer();
		void onResize(WindowResizeEvent* evnt);
		void present();
	private:
		void createRenderTargets();
		DXGI_RATIONAL queryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain2> mSwapChain;
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> m2DRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1Factory> mFactory;

		std::shared_ptr<RenderTarget> m3DRenderTarget;
		RenderDevice& mRenderDevice;
	};
}