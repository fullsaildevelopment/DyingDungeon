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
		RenderWindowDX11(std::shared_ptr<RenderDevice> renderDevice, HWND& hWnd);
		~RenderWindowDX11();
	public:
		std::shared_ptr<RenderTarget> get3DRenderTarget();
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> get2DRenderTarget();
	public:
		Microsoft::WRL::ComPtr<ID3D11Resource> getBackBuffer();
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> getBackBuffer2D();
		void onResize(WindowResizeEvent* evnt);
		void present();
	private:
		void createRenderTargets();
		DXGI_RATIONAL queryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain2> mSwapChain;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> mBackBuffer;
		Microsoft::WRL::ComPtr<ID2D1Factory1> mFactory;
		std::shared_ptr<RenderTarget> m3DRenderTarget;
	};
}