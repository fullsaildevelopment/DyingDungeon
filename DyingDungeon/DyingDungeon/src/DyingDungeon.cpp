#include "DyingDungeon.h"
#include "RenderPass.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "Animator.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderPipelineManager.h"
#include "OpaquePass.h"
#include "SkyboxPass.h"
#include "ShadowPass.h"
#include "TransparentPass.h"
#include "ClearRenderTargetPass.h"
#include "DebugPass.h"
#include "Light.h"
#include "MaterialManager.h"
#include "ShaderManager.h"
#include "Component.h"
#include "FileManager.h"

// Game Includes
#include "ExampleComponent.h"

namespace
{
    // Rendering resources
    std::unique_ptr<Odyssey::RenderWindow> gMainWindow;
    std::shared_ptr<Odyssey::RenderTarget> gRenderTarget;
    // Scene resources
    std::unique_ptr<Odyssey::Scene> gMainScene;
	std::shared_ptr<Odyssey::GameObject> gArena;
    // Light resources
    std::shared_ptr<Odyssey::Light> gDirLight;
    std::shared_ptr<Odyssey::Light> gPointLight[10];
}

// Forward declarations
void initialize(HWND& hWnd);
void setupDefaults(HWND& hWnd);
void setupPipeline();
void setupLighting();
void setupArena();
void update();
void updateInput();

void initialize(HWND& hWnd)
{
	// Set up the following default rendering resources:
	// Render window, render target, viewport, render state, sampler state, main scene
	setupDefaults(hWnd);

	// Set up the scene lighting
	setupLighting();

	// Load the arena scene
	setupArena();

	// Set up the default rendering pipeline
	setupPipeline();

	// Set the initial view and projection matrix
	gMainScene->mMainCamera.setPosition(0, 0, 0);
	gMainScene->mMainCamera.setProjectionValues(60.0f, gMainWindow->getAspectRatio(), 0.1f, 100.0f);

	gMainScene->initialize();
}

void setupDefaults(HWND& hWnd)
{
	// Create a render window from the current window
	gMainWindow = std::make_unique<Odyssey::RenderWindow>(hWnd);

	// Create a render target from the main window
	gRenderTarget = std::make_shared<Odyssey::RenderTarget>(gMainWindow->mMainWindow.width, gMainWindow->mMainWindow.height, true, *gMainWindow);

	// Create the scene
	gMainScene = std::make_unique<Odyssey::Scene>();
}

void setupPipeline()
{
	// Create a clear render target pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ClearRenderTargetPass> rtvPass;
	rtvPass = std::make_shared<Odyssey::ClearRenderTargetPass>(gRenderTarget, true);
	Odyssey::RenderPipelineManager::getInstance().addPass(rtvPass);

	// Create a skybox pass and add it to the render pipeline
	std::shared_ptr<Odyssey::SkyboxPass> skyboxPass;
	skyboxPass = std::make_shared<Odyssey::SkyboxPass>("Skybox.dds", gRenderTarget);
	Odyssey::RenderPipelineManager::getInstance().addPass(skyboxPass);

	// Create a shadow pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ShadowPass> shadowPass;
	shadowPass = std::make_shared<Odyssey::ShadowPass>(gDirLight, 4096, 4096);
	Odyssey::RenderPipelineManager::getInstance().addPass(shadowPass);

	// Create an opaque pass and add it to the render pipeline
	std::shared_ptr<Odyssey::OpaquePass> opaquePass;
	opaquePass = std::make_shared<Odyssey::OpaquePass>(gRenderTarget);
	Odyssey::RenderPipelineManager::getInstance().addPass(opaquePass);

	// Create a transparent pass and add it to the render pipeline
	//std::shared_ptr<Odyssey::TransparentPass> transparentPass;
	//transparentPass = std::make_shared<Odyssey::TransparentPass>(gRenderTarget);
	//Odyssey::RenderPipelineManager::getInstance().addPass(transparentPass);

	// Create a debugging pass and add it to the render pipeline
	std::shared_ptr<Odyssey::DebugPass>debugPass;
	debugPass = std::make_shared<Odyssey::DebugPass>(gRenderTarget);
	Odyssey::RenderPipelineManager::getInstance().addPass(debugPass);
}

void setupLighting()
{
	// Set up a directional light
	gDirLight = std::make_shared<Odyssey::Light>();
	gDirLight->mLightType = Odyssey::LightType::Directional;
	gDirLight->mWorldPosition = DirectX::XMFLOAT4(0, 0, 0, 1);
	gDirLight->mWorldDirection = DirectX::XMFLOAT4(0.0f, -0.5f, -0.5f, 0.0f);
	gDirLight->mColor = DirectX::XMFLOAT4(0.4f, 0.5f, 0.6f, 1.0f);
	gDirLight->mIntensity = 1.0f;
	gDirLight->mRange = 0.0f;
	gDirLight->mSpotAngle = 0.0f;

	gPointLight[0] = std::make_shared<Odyssey::Light>();
	gPointLight[0]->mLightType = Odyssey::LightType::Point;
	gPointLight[0]->mWorldPosition = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	gPointLight[0]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gPointLight[0]->mColor = DirectX::XMFLOAT4(0.7f, 0.6f, 0.5f, 1.0f);
	gPointLight[0]->mIntensity = 1.0f;
	gPointLight[0]->mRange = 100.0f;
	gPointLight[0]->mSpotAngle = 0.0f;

	gMainScene->addLight(gDirLight);
	gMainScene->addLight(gPointLight[0]);
}

void setupArena()
{
	gArena = std::make_shared<Odyssey::GameObject>();
	Odyssey::FileManager::getInstance().importModel(gArena, "assets/models/TestArena.dxm");
	gArena->addComponent<ExampleComponent>();
	gMainScene->addSceneObject(gArena);
}

void update()
{
	// Update user input
	updateInput();

	// Render the scene
	gMainScene->update();

	// Present the main window
	gMainWindow->present();
}

void updateInput()
{
	float delta = static_cast<float>(gMainScene->getDeltaTime());
	float moveSpeed = 10.0f * delta;
	float rotationSpeed = 100.0f * delta;
	float xPosition = 0.0f;
	float yPosition = 0.0f;
	float zPosition = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

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

	gMainScene->mMainCamera.updateCamera(xPosition, yPosition, zPosition, pitch, yaw, 0.0f);
}

#pragma region WINDOWS CODE
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DYINGDUNGEON, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DYINGDUNGEON));

	MSG msg;

	// Main message loop:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		// Update
		update();
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DYINGDUNGEON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DYINGDUNGEON);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Initialize
	initialize(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
#pragma endregion