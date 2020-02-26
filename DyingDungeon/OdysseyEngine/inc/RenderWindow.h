#pragma once
#include "EngineEvents.h"
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderWindow
	{
	public:
		HWND getWindowHandle();
		float getAspectRatio();
		UINT getWidth();
		UINT getHeight();
		void getPosition(UINT& x, UINT& y);
		void getScreenScale(float* x, float* y);
	protected:
		std::shared_ptr<HWND> mWindowHandle;
		bool mIsFullScreen;
		struct WindowProperties
		{
			UINT left, right;
			UINT top, bottom;
			UINT width, height;
			float aspectRatio;
			float screenScaleX, screenScaleY;

			WindowProperties()
			{
				left = right = top = bottom = width = height = 0;
				aspectRatio = 0.0f;
				screenScaleX = screenScaleY = 1.0f;
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
		};
		WindowProperties mProperties;
		WindowProperties mBaseProperties;
	};
}