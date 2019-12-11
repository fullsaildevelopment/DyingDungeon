#include "Application.h"
#include <windows.h>
#include "Scene.h"
#include "RenderWindow.h"
#include "DebugManager.h"

#define RENDER_WINDOW_CLASS_NAME L"RenderWindowClass"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
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
				updateInput();

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

	void Application::updateInput()
	{
		float delta = static_cast<float>(mActiveScene->getDeltaTime());
		float moveSpeed = 10.0f * delta;
		float rotationSpeed = 100.0f * delta;
		float xPosition = 0.0f;
		float yPosition = 0.0f;
		float zPosition = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;

		std::shared_ptr<Light> light = mActiveScene->getLight(4);

		if (GetAsyncKeyState(VK_NUMPAD4))
		{
			light->addPosition(-0.1f, 0, 0);
		}
		if (GetAsyncKeyState(VK_NUMPAD6))
		{
			light->addPosition(0.1f, 0, 0);
		}
		if (GetAsyncKeyState(VK_NUMPAD2))
		{
			light->addPosition(0.0f, 0, -0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD8))
		{
			light->addPosition(0.0f, 0, 0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD9))
		{
			light->addPosition(0.0f, 0.1f, 0.0f);
		}
		if (GetAsyncKeyState(VK_NUMPAD3))
		{
			light->addPosition(0.0f, -0.1f, 0.0f);
		}
		if (GetAsyncKeyState(VK_NUMPAD7))
		{
			light->addRange(0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			light->addRange(-0.1f);
		}
		if (GetAsyncKeyState('W'))
		{
			zPosition += moveSpeed;
		}
		if (GetAsyncKeyState('S'))
		{
			zPosition -= moveSpeed;
		}
		if (GetAsyncKeyState('D'))
		{
			xPosition += moveSpeed;
		}
		if (GetAsyncKeyState('A'))
		{
			xPosition -= moveSpeed;
		}

		if (GetAsyncKeyState(VK_SPACE))
		{
			yPosition += moveSpeed;
		}

		if (GetAsyncKeyState('X'))
		{
			yPosition -= moveSpeed;
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			yaw -= rotationSpeed;
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			yaw += rotationSpeed;
		}

		if (GetAsyncKeyState(VK_UP))
		{
			pitch -= rotationSpeed;
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			pitch += rotationSpeed;
		}

		mActiveScene->mMainCamera.updateCamera(xPosition, yPosition, zPosition, pitch, yaw, 0.0f);
	}
}
