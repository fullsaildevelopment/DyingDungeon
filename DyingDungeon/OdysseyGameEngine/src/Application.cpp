#include "Application.h"
#include <windows.h>
#include "Scene.h"
#include "RenderWindow.h"
#include "DebugManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"

#define RENDER_WINDOW_CLASS_NAME L"RenderWindowClass"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
		Odyssey::InputManager::getInstance().registerInput(static_cast<char>(wParam), true);
	}
	break;
	case WM_KEYUP:
	{
		Odyssey::InputManager::getInstance().unregisterInput(static_cast<char>(wParam));
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;

		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

namespace Odyssey
{
	Application::Application()
	{
		mHandleInstance = ::GetModuleHandle(NULL);

		WNDCLASS renderWindowClass = { };

		renderWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		renderWindowClass.lpfnWndProc = &WndProc;
		renderWindowClass.cbClsExtra = 0;
		renderWindowClass.cbWndExtra = 0;
		renderWindowClass.hInstance = mHandleInstance;
		renderWindowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		renderWindowClass.hbrBackground = 0;
		renderWindowClass.lpszMenuName = NULL;
		renderWindowClass.lpszClassName = RENDER_WINDOW_CLASS_NAME;

		RegisterClass(&renderWindowClass);

		mRenderDevice = std::make_unique<RenderDevice>(*this);

		DebugManager::getInstance().initialize(*mRenderDevice);
	}

	std::shared_ptr<RenderWindow> Application::createRenderWindow(const std::string& title, int windowWidth, int windowHeight)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		RECT windowRect = { 0, 0, windowWidth, windowHeight };

		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		windowWidth = windowRect.right - windowRect.left;
		windowHeight = windowRect.bottom - windowRect.top;

		int windowX = (screenWidth - windowWidth) / 2;
		int windowY = (screenHeight - windowHeight) / 2;

		HWND hWindow = CreateWindowEx(NULL,
			RENDER_WINDOW_CLASS_NAME,
			L"Render Window",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			mHandleInstance,
			NULL);

		std::shared_ptr<RenderWindow> window = std::make_shared<RenderWindow>(*(mRenderDevice.get()), hWindow);

		if (mActiveWindow == nullptr)
		{
			mActiveWindow = window;
		}
		UpdateWindow(hWindow);
		ShowWindow(hWindow, SW_SHOWDEFAULT);
		return window;
	}

	void Application::setActiveScene(std::shared_ptr<Scene> scene)
	{
		mActiveScene = scene;
	}

	int Application::update()
	{
		mIsRunning = true;
		if (mActiveScene)
			mActiveScene->initialize();

		MSG msg;

		while (mIsRunning)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					return 0;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			{
				if (mActiveScene)
					mActiveScene->update();

				mActiveWindow->present();
			}
		}

		return static_cast<int>(msg.wParam);
	}

	void Application::stop()
	{
		PostQuitMessage(0);
	}

	RenderDevice* Application::getRenderDevice()
	{
		return mRenderDevice.get();
	}

	HINSTANCE Application::GetModuleHandle() const
	{
		return mHandleInstance;
	}
}
