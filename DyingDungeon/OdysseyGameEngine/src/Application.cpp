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
#include "RenderPass.h"
#include "RenderPipeline.h"

#define RENDER_WINDOW_CLASS_NAME L"RenderWindowClass"

namespace Odyssey
{
	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_KEYDOWN:
			{
				// Register the input as key down with the input manager
				Odyssey::InputManager::getInstance().registerKeyDown(static_cast<int>(wParam));
			}
			break;
			case WM_KEYUP:
			{
				// Register the input as key up with the input manager
				Odyssey::InputManager::getInstance().registerKeyUp(static_cast<int>(wParam));
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

	Application::Application()
	{
		// Get the module handle assocaited with the application
		mHandleInstance = ::GetModuleHandle(NULL);

		// Create a default blank window for rendering
		WNDCLASS renderWindowClass = { };
		renderWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		renderWindowClass.lpfnWndProc = Application::WndProc;
		renderWindowClass.cbClsExtra = 0;
		renderWindowClass.cbWndExtra = 0;
		renderWindowClass.hInstance = mHandleInstance;
		renderWindowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		renderWindowClass.hbrBackground = 0;
		renderWindowClass.lpszMenuName = NULL;
		renderWindowClass.lpszClassName = RENDER_WINDOW_CLASS_NAME;

		// Register the window
		RegisterClass(&renderWindowClass);

		// Create the RenderDevice for this application
		mRenderDevice = std::make_unique<RenderDevice>(*this);
		mRenderPipeline = std::make_unique<RenderPipeline>(*(mRenderDevice.get()));

		// Initialize the debug renderer
		DebugManager::getInstance().initialize(*mRenderDevice);
	}

	Application::~Application()
	{
		if (mActiveScene)
		{
			mActiveScene = nullptr;
		}
	}

	std::shared_ptr<RenderWindow> Application::createRenderWindow(const std::string& title, int windowWidth, int windowHeight)
	{
		// Get the width and height of the screen
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Initialize a window rect
		RECT windowRect = { 0, 0, windowWidth, windowHeight };

		// Adjust the window rect
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		// Calculate a new window width and height
		windowWidth = windowRect.right - windowRect.left;
		windowHeight = windowRect.bottom - windowRect.top;

		// Calculate the window's x and y
		int windowX = (screenWidth - windowWidth) / 2;
		int windowY = (screenHeight - windowHeight) / 2;

		// Create the window
		HWND hWindow = CreateWindowEx(NULL,
			RENDER_WINDOW_CLASS_NAME,
			L"Render Window",
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			mHandleInstance,
			this);

		HRESULT hr = hWindow ? S_OK : E_FAIL;

		// Create a new RenderWindow associated with the window
		std::shared_ptr<RenderWindow> window = std::make_shared<RenderWindow>(*(mRenderDevice.get()), hWindow);

		// Store the window in the list of windows
		mWindows.emplace_back(window);

		// If there are no active windows, this becomes our new active window
		if (mActiveWindow == nullptr)
		{
			mActiveWindow = window;
		}

		// Update and show the window
		UpdateWindow(hWindow);
		ShowWindow(hWindow, SW_SHOWDEFAULT);

		// Return the RenderWindow
		return window;
	}

	void Application::addScene(std::string name, std::shared_ptr<Scene> scene)
	{
		mSceneMap[name] = scene;
	}

	void Application::setActiveScene(std::string scene)
	{
		// Set the new active scene
		if (mActiveScene)
		{
			mSceneMap[scene]->initialize();
		}
		mActiveScene = mSceneMap[scene];
	}

	void Application::addRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		// Push the render pass into the back of the list
		mRenderPipeline->addRenderPass(renderPass);
	}

	int Application::run()
	{
		// Set the running state
		mIsRunning = true;

		// Initialize the active scene and it's contained components
		if (mActiveScene)
			mActiveScene->initialize();

		MSG msg;

		// Update loop
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
			// else
			{
				// Update the scene
				if (mActiveScene)
					mActiveScene->update();

				static DWORD frameCount = 0; ++frameCount;
				static DWORD framesPast = frameCount;
				static DWORD prevCount = GetTickCount();

				if (GetTickCount64() - prevCount > 1000)
				{
					char buffer[256];
					sprintf_s(buffer, "FPS: %d", frameCount - framesPast);
					SetWindowTextA(*(mActiveWindow->getWindowHandle()), buffer);
					framesPast = frameCount;
					prevCount = GetTickCount64();
				}

				if (InputManager::getInstance().getKeyPress(VK_F4))
				{
					mRenderPipeline->getRenderPass(6)->setEnabled(!mRenderPipeline->getRenderPass(6)->isEnabled());
				}

				// Render the scene
				mRenderPipeline->render(mActiveScene);

				// Present the window
				mActiveWindow->present();
			}
		}

		return static_cast<int>(msg.wParam);
	}

	void Application::stop()
	{
		// Exit the application gracefully
		PostQuitMessage(0);
	}

	RenderDevice* Application::getRenderDevice()
	{
		// Return a raw pointer to the RenderDevice
		return mRenderDevice.get();
	}

	HINSTANCE Application::GetModuleHandle() const
	{
		// Return the module handle
		return mHandleInstance;
	}
}
