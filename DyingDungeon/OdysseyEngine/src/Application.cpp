#include "Application.h"
#include <windows.h>
#include <windowsx.h>
#include "Scene.h"
#include "RenderWindow.h"
#include "DebugManager.h"
#include "InputManager.h"
#include "Entity.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "RenderPass.h"
#include "RenderPipeline.h"
#include "ThreadManager.h"
#include "EventManager.h"
#include "EngineEvents.h"
#include "RenderWindowDX11.h"
#include "Keycode.h"
#include "SceneDX11.h"

#include "ClearRenderTargetPass.h"
#include "SkyboxPass.h"
#include "ShadowPass.h"
#include "OpaquePass.h"
#include "TransparentPass.h"
#include "DebugPass.h"
#include "Sprite2DPass.h"

#define RENDER_WINDOW_CLASS_NAME L"RenderWindowClass"

namespace Odyssey
{
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
		mRenderPipeline = std::make_shared<RenderPipeline>();

		// Enable muli-threading by default
		mIsMultithreading = true;
		mIsShutdown = false;
		mIsRendering = true;
		mTimer.Restart();
		mTickInterval = 0.0;

		// Subscribe to the scene change event
		EventManager::getInstance().subscribe(this, &Application::onSceneChange);

		// Subscribe to the command receive event
		EventManager::getInstance().subscribe(this, &Application::onCommandReceive);

		// Subscribe to the engine shutdown event
		EventManager::getInstance().subscribe(this, &Application::onShutdown);

		EventManager::getInstance().subscribe(this, &Application::onShutdownApplication);

		EventManager::getInstance().subscribe(this, &Application::onUIScale);

		EventManager::getInstance().subscribe(this, &Application::onCreatePrefab);

		EventManager::getInstance().subscribe(this, &Application::onSpawnEntity);
		EventManager::getInstance().subscribe(this, &Application::onDestroyEntity);

		EventManager::getInstance().subscribe(this, &Application::onChangeMouseCursor);
		EventManager::getInstance().subscribe(this, &Application::onEnableRendering);
	}

	Application::~Application()
	{
		if (mActiveScene)
		{
			mActiveScene = nullptr;
		}
	}

	void Application::onSceneChange(SceneChangeEvent* evnt)
	{
		// Set the new active scene
		if (mSceneMap.count(evnt->sceneName) > 0)
		{
			if (mIsMultithreading == false)
			{
				mActiveScene->onDestroy();
			}

			mActiveScene = mSceneMap[evnt->sceneName];

			// Check the active scene is set
			if (mActiveScene)
			{
				// Initialize the scene
				mActiveScene->initialize();

				// Notify the thread manager to restart the scene thread
				if (mIsMultithreading)
				{
					ThreadManager::getInstance().changeActiveScene(mActiveScene);
					mIsRendering = false;
				}
			}
		}
	}

	void Application::onCommandReceive(CommandReceiveEvent* command)
	{
		// Process event manager commands on the next frame
		mProcessCommands = true;
	}

	void Application::onEnableRendering(EnableRenderingEvent* evnt)
	{
		mIsRendering = true;
	}

	void Application::onShutdown(EngineShutdownEvent* evnt)
	{
		mIsShutdown = true;
		mIsRunning = false;
	}

	void Application::onShutdownApplication(ShutdownApplicationEvent* evnt)
	{
		EventManager::getInstance().publish(new EngineShutdownEvent());
	}

	void Application::onUIScale(UIScaleEvent* evnt)
	{
		mActiveWindow->getScreenScale(evnt->xScale, evnt->yScale);
	}

	void Application::onCreatePrefab(CreatePrefabEvent* evnt)
	{
		*(evnt->entity) = createPrefab();
	}

	void Application::onSpawnEntity(SpawnEntityEvent* evnt)
	{
		// Create a copy of the entity
		*(evnt->entity) = mActiveScene->spawnEntity(evnt->prefab, evnt->position, evnt->rotation);
	}

	void Application::onDestroyEntity(DestroyEntityEvent* evnt)
	{
		mActiveScene->destroyEntity(evnt->entity);
	}

	void Application::onChangeMouseCursor(ChangeMouseCursorEvent* evnt)
	{
		auto mCursor = LoadCursorFromFile(evnt->filename);
		SetCursor(mCursor);
		SetClassLongPtr(mActiveWindow->getWindowHandle(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(mCursor));
	}

	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_LBUTTONDOWN:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				EventManager::getInstance().publish(new MouseClickEvent(xPos, yPos));
				EventManager::getInstance().publish(new KeypressEvent(KeyCode::LButton));
				break;
			}
			case WM_LBUTTONUP:
			{
				EventManager::getInstance().publish(new KeyUpEvent(KeyCode::LButton));
				break;
			}
			case WM_KEYDOWN:
			{
				// Register the input as key down with the input manager
				EventManager::getInstance().publish(new KeypressEvent((KeyCode)wParam));
				break;
			}
			case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				EventManager::getInstance().publish(new MouseInputEvent(xPos, yPos));
				
				break;
			}
			case WM_SETCURSOR:
			{
				break;
			}
			case WM_KEYUP:
			{
				// Register the input as key up with the input manager
				EventManager::getInstance().publish(new KeyUpEvent((KeyCode)wParam));
				break;
			}
			case WM_SIZE:
			{
				// Get the new client rect
				RECT mainWinRect;
				GetClientRect(hwnd, &mainWinRect);

				// Register the window resize event with the new window bounds
				if ((mainWinRect.right - mainWinRect.left != 0) && (mainWinRect.bottom - mainWinRect.top != 0))
				{
					EventManager::getInstance().publish(new WindowResizeEvent(mainWinRect.left, mainWinRect.right, mainWinRect.top, mainWinRect.bottom));
				}
				
				break;
			}
			case WM_CLOSE:
			{
				EventManager::getInstance().publish(new EngineShutdownEvent());
				break;
			}
			case WM_PAINT:
			{
				PAINTSTRUCT paintStruct;
				HDC hDC;
				hDC = BeginPaint(hwnd, &paintStruct);
				EndPaint(hwnd, &paintStruct);
				break;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	RenderWindow* Application::createRenderWindow(const std::wstring& title, int windowWidth, int windowHeight)
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
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			mHandleInstance,
			this);

		// Assert the window was created successfully
		HRESULT hr = hWindow ? S_OK : E_FAIL;
		assert(!FAILED(hr));

		// Create a new RenderWindow associated with the window
		std::shared_ptr<RenderWindow> window = std::make_shared<RenderWindowDX11>(hWindow);

		// Store the window in the list of windows
		mWindows.emplace_back(std::static_pointer_cast<RenderWindowDX11>(window));

		// If there are no active windows, this becomes our new active window
		if (mActiveWindow == nullptr)
		{
			mActiveWindow = std::static_pointer_cast<RenderWindowDX11>(window);
		}

		// Update and show the window
		UpdateWindow(hWindow);
		ShowWindow(hWindow, SW_SHOWDEFAULT);

		// Return the RenderWindow
		return window.get();
	}

	Entity* Application::createPrefab()
	{
		std::shared_ptr<Entity> prefab = std::make_shared<Entity>();
		prefab->setStatic(false);
		mPrefabs.push_back(prefab);
		return prefab.get();
	}

	Scene* Application::createScene(std::string name)
	{
		mSceneMap[name] = std::make_shared<SceneDX11>();

		// Check if there is no active scene
		if (mActiveScene == nullptr)
		{
			mActiveScene = mSceneMap[name];
		}

		return std::static_pointer_cast<Scene>(mSceneMap[name]).get();
	}

	Scene* Application::createScene(std::string name, DirectX::XMFLOAT3 center, float radius)
	{
		mSceneMap[name] = std::make_shared<SceneDX11>(center, radius);

		// Check if there is no active scene
		if (mActiveScene == nullptr)
		{
			mActiveScene = mSceneMap[name];
		}

		return std::static_pointer_cast<Scene>(mSceneMap[name]).get();
	}

	void Application::addRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		// Push the render pass into the back of the list
		mRenderPipeline->addRenderPass(renderPass);
	}

	void Application::setMultithreading(bool active)
	{
		// TODO: Add functionality to shut down the scene thread and allow for single threaded access to the scene
		mIsMultithreading = active;
	}

	void Application::setFrameLimit(std::string threadName, unsigned int fps)
	{
		if (threadName == "Main")
		{
			mTimer.Restart();
			if (fps == 0)
			{
				mTickInterval = 0.0;
			}
			else
			{
				mTickInterval = 1.0 / static_cast<double>(fps);
			}
		}
		else if (threadName == "Render")
		{
			double interval = 0.0;
			if (fps != 0)
			{
				interval = 1.0 / static_cast<double>(fps);
			}

			ThreadManager::getInstance().setFrameLimit(interval);
		}
	}

	int Application::run()
	{
		// Set the running state
		mIsRunning = true;

		MSG msg;

		// Set up the rendering pipeline
		setupRenderPipeline();

		// Check if we are flagged for multithreading
		if (mIsMultithreading)
		{
			// Check that there is an active scene
			if (mActiveScene)
			{
				// Execute the scene thread with the active scene
				mActiveScene->initialize();
				ThreadManager::getInstance().executeSceneThread(mActiveScene);
			}
		}
		else
		{
			if (mActiveScene)
			{
				mActiveScene->initialize();
			}
		}

		//ThreadManager::getInstance().executeRenderThread(mRenderPipeline, mActiveScene);

		// Update loop
		while (mIsRunning)
		{
			// Handle windows messages
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
				mTimer.Signal();

				if (mIsShutdown)
				{
					continue;
				}

				// Check at the top of every frame if the event manager has commands to flush
				if (mProcessCommands)
				{
					// Fire the command flush event and set the process commands flag to false
					EventManager::getInstance().publish(new CommandFlushEvent());
					mProcessCommands = false;
				}

				InputManager::getInstance().sendMouseMove();

				if (mTimer.TotalTime() > mTickInterval)
				{
					mTimer.Restart();
					// Check if multithreading is disabled
					if (mIsMultithreading == false)
					{
						// Check for an active scene
						if (mActiveScene)
						{
							// Update the scene
							mActiveScene->update();
						}
					}

					// Check for an active scene
					if (mActiveScene && mIsRendering)
					{
						// Render the scene
						mRenderPipeline->render(mActiveWindow.get(), mActiveScene.get());
					}

					// Fire a thread tick event for the main thread. This is for profiling purposes only.
					EventManager::getInstance().publish(new ThreadTickEvent("Main Thread"));

					// Present the window
					mActiveWindow->present();
				}
				
			}
		}

		return static_cast<int>(msg.wParam);
	}

	void Application::stop()
	{
		// Exit the application gracefully
		PostQuitMessage(0);
	}

	void Application::setupRenderPipeline()
	{
		mRenderPipeline->addRenderPass(std::make_shared<SkyboxPass>());
		mRenderPipeline->addRenderPass(std::make_shared<ShadowPass>(4096, 4096));
		mRenderPipeline->addRenderPass(std::make_shared<OpaquePass>());
		mRenderPipeline->addRenderPass(std::make_shared<TransparentPass>());
		mRenderPipeline->addRenderPass(std::make_shared<Sprite2DPass>());
	}
}