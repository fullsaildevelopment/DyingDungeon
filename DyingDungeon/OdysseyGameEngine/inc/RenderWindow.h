#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderDevice;
	class RenderTarget;
	class RenderWindow
	{
	public:
		RenderWindow(RenderDevice& renderDevice, HWND& hWnd);
	public:
		void present();
		Microsoft::WRL::ComPtr<ID3D11Resource> getBackBuffer();
		float getAspectRatio();
		std::shared_ptr<RenderTarget> getRenderTarget();
		~RenderWindow() { }
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain2> mSwapChain;
		std::shared_ptr<RenderTarget> mRenderTarget;
		HWND& mWindowHandle;
	public:
		struct Window
		{
			UINT left, right;
			UINT top, bottom;
			UINT width, height;
			float aspectRatio;

			Window()
			{
				left = right = top = bottom = width = height = 0;
				aspectRatio = 0.0f;
			}

			void setBounds(UINT winLeft, UINT winRight, UINT winTop, UINT winBottom)
			{
				left = winLeft;
				right = winRight;
				top = winTop;
				bottom = winBottom;
				width = winRight - winLeft;
				height = winBottom - winTop;
				aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			}
		} mMainWindow;
	};
}