#include "RenderWindow.h"
#include "RenderTarget.h"
#include "EventManager.h"

namespace Odyssey
{
	HWND RenderWindow::getWindowHandle()
	{
		return *mWindowHandle;
	}

	float RenderWindow::getAspectRatio()
	{
		return static_cast<float>(mProperties.width) / static_cast<float>(mProperties.height);
	}

	UINT RenderWindow::getWidth()
	{
		return mProperties.width;
	}

	UINT RenderWindow::getHeight()
	{
		return mProperties.height;
	}

	void RenderWindow::getPosition(UINT& x, UINT& y)
	{
		x = mProperties.left;
		y = mProperties.top;
	}
	void RenderWindow::getScreenScale(float* x, float* y)
	{
		*x = mProperties.screenScaleX;
		*y = mProperties.screenScaleY;
	}
}
