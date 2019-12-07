#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderWindow
	{
	public:
		RenderWindow(HWND& hWnd);
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
	public:
		void present();
		Microsoft::WRL::ComPtr<ID3D11Resource> getBackBuffer();
		float getAspectRatio();
		~RenderWindow() { }
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	};
}