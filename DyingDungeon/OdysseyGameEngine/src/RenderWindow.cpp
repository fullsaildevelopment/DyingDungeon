#include "RenderWindow.h"
#include "RenderManager.h"

namespace Odyssey
{
	RenderWindow::RenderWindow(HWND& hWnd)
	{
		// Get the window properties
		RECT mainWinRect;
		GetClientRect(hWnd, &mainWinRect);
		mMainWindow.setBounds(mainWinRect.left, mainWinRect.right,
			mainWinRect.top, mainWinRect.bottom);

		// Initialize the render manager
		RenderManager::getInstance().initialize(hWnd, mSwapChain);
	}

	void RenderWindow::present()
	{
		mSwapChain->Present(1, 0);
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
}