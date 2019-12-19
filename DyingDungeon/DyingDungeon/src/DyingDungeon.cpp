#include "DyingDungeon.h"
#include "RenderPass.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "Animator.h"
#include "Scene.h"
#include "GameObject.h"
#include "OpaquePass.h"
#include "SkyboxPass.h"
#include "ShadowPass.h"
#include "TransparentPass.h"
#include "ClearRenderTargetPass.h"
#include "DebugPass.h"
#include "Light.h"
#include "Component.h"
#include "FileManager.h"
#include "Transform.h"
#include "Application.h"
#include "RenderDevice.h"
#include "Material.h"
#include "Camera.h"
#include "RedAudioManager.h"
#include "UICanvas.h"
#include "Sprite2DPass.h"
#include "MenuManager.h"
#include "MainMenuController.h"
#include "Sprite2D.h"
#include "Rectangle2D.h"
#include "Text2D.h"

// Game Includes
#include "TowerManager.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"
#include "CameraController.h"

namespace
{
	// Rendering resources
	std::shared_ptr<Odyssey::RenderWindow> gMainWindow;
	std::shared_ptr<Odyssey::RenderTarget> gRenderTarget;
	// Scene resources
	std::shared_ptr<Odyssey::Scene> gGameScene;
	std::shared_ptr<Odyssey::Scene> gMainMenu;
	std::shared_ptr<Odyssey::GameObject> gMainCamera;
	//Game Objects
	std::shared_ptr<Odyssey::GameObject> gMenu;
	std::shared_ptr<Odyssey::GameObject> gGameMenu;
	std::shared_ptr<Odyssey::GameObject> gPaladin;
	std::shared_ptr<Odyssey::GameObject> gSkeleton;
	std::shared_ptr<Odyssey::GameObject> gCurrentTower;
	//Vectors
	std::vector<std::shared_ptr<Odyssey::GameObject>> gPlayerUnit;
	std::vector<std::shared_ptr<Odyssey::GameObject>> gEnemyUnit;
	// Light resources
	std::shared_ptr<Odyssey::Light> gDirLight;
	std::shared_ptr<Odyssey::Light> gLights[15];
}

// Forward declarations
int playGame();
void setupPipeline(Odyssey::RenderDevice* renderDevice, std::shared_ptr<Odyssey::Application> application);
void setupLighting();
void setupCamera();
void setupMainMenu(Odyssey::RenderDevice* renderDevice, Odyssey::Application* application);
void setupArena();
void setupGameInterface();
void setupPaladin();
void setupSkeleton();
void setupAudio();

//Tristen's Stuff
void setUpTowerManager();

int playGame()
{
	// Set up the application and create a render window
	std::shared_ptr<Odyssey::Application> application = std::make_shared<Odyssey::Application>();
	gMainWindow = application->createRenderWindow("Dying Dungeon", 1280, 720);

	MenuManager::GetInstance().initialize(application.get());

	// Get the render device
	Odyssey::RenderDevice* renderDevice = application->getRenderDevice();

	// Create the main scene
	gGameScene = renderDevice->createScene();

	// Set up the scene lighting
	setupLighting();

	// Set up the default rendering pipeline
	setupPipeline(renderDevice, application);

	// Set up camera
	setupCamera();

	setupMainMenu(renderDevice, application.get());

	// Load the arena scene
	setupArena();

	// Set up the paladin
	setupPaladin();

	// Set up the skeleton
	setupSkeleton();

	// Set up the game user interface
	setupGameInterface();

	// Set up the tower manager
	setUpTowerManager();

	// Set the active scene
	application->addScene("MainMenu", gMainMenu);
	application->addScene("Game", gGameScene);
	application->setActiveScene("MainMenu");

	// Play audio
	setupAudio();

	// Run the application
	return application->run();
}

void setupPipeline(Odyssey::RenderDevice* renderDevice, std::shared_ptr<Odyssey::Application> application)
{
	// Create a clear render target pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ClearRenderTargetPass> rtvPass = renderDevice->createClearRTVPass(gMainWindow->getRenderTarget(), true);
	application->addRenderPass(rtvPass);

	// Create a skybox pass and add it to the render pipeline 
	std::shared_ptr<Odyssey::SkyboxPass> skyboxPass = renderDevice->createSkyboxPass("Skybox.dds", gMainWindow->getRenderTarget());
	application->addRenderPass(skyboxPass);

	// Create a shadow pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ShadowPass> shadowPass = renderDevice->createShadowPass(gDirLight, 4096, 4096);
	application->addRenderPass(shadowPass);

	// Create an opaque pass and add it to the render pipeline
	std::shared_ptr<Odyssey::OpaquePass> opaquePass = renderDevice->createOpaquePass(gMainWindow->getRenderTarget());
	application->addRenderPass(opaquePass);

	// Create a transparent pass and add it to the render pipeline
	std::shared_ptr<Odyssey::TransparentPass> transparentPass = renderDevice->createTransparentPass(gMainWindow->getRenderTarget());
	application->addRenderPass(transparentPass);

	std::shared_ptr<Odyssey::Sprite2DPass> spritePass = renderDevice->createSprite2DPass(gMainWindow);
	application->addRenderPass(spritePass);

	// Create a debugging pass and add it to the render pipeline
	//std::shared_ptr<Odyssey::DebugPass>debugPass = renderDevice->createDebugPass(gMainWindow->getRenderTarget());
	//application->addRenderPass(debugPass);
}

void setupLighting()
{
	// Set up a directional light
	gDirLight = std::make_shared<Odyssey::Light>();
	gDirLight->setLightType(Odyssey::LightType::Directional);
	gDirLight->setPosition(0, 0, 0);
	gDirLight->setDirection(0.75f, -0.45f, -0.055f);
	gDirLight->setColor(0.4f, 0.5f, 0.7f);
	gDirLight->setIntensity(1.0f);
	gDirLight->setRange(0.0f);
	gDirLight->setSpotAngle(0.0f);

	// First Level Arena Lights
	// Arena ambient
	gLights[0] = std::make_shared<Odyssey::Light>();
	gLights[0]->setLightType(Odyssey::LightType::Point);
	gLights[0]->setPosition(0.0, 10.0f, 0.0f);
	gLights[0]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[0]->setColor(0.5f, 0.5f, 0.5f);
	gLights[0]->setIntensity(2.25f);
	gLights[0]->setRange(30.0f);
	gLights[0]->setSpotAngle(0.0f);

	// World-Space Left Pillar 1
	gLights[1] = std::make_shared<Odyssey::Light>();
	gLights[1]->setLightType(Odyssey::LightType::Point);
	gLights[1]->setPosition(-5.45f, 4.75f, 14.4f);
	gLights[1]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[1]->setColor(0.8f, 0.5f, 0.4f);
	gLights[1]->setIntensity(2.0f);
	gLights[1]->setRange(5.0f);
	gLights[1]->setSpotAngle(0.0f);

	// World-Space Left Pillar 2
	gLights[2] = std::make_shared<Odyssey::Light>();
	gLights[2]->setLightType(Odyssey::LightType::Point);
	gLights[2]->setPosition(-5.45f, 4.75f, 7.5f);
	gLights[2]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[2]->setColor(0.8f, 0.5f, 0.4f);
	gLights[2]->setIntensity(2.0f);
	gLights[2]->setRange(5.0f);
	gLights[2]->setSpotAngle(0.0f);

	// World-Space Left Pillar 3
	gLights[3] = std::make_shared<Odyssey::Light>();
	gLights[3]->setLightType(Odyssey::LightType::Point);
	gLights[3]->setPosition(-5.45f, 4.75f, -6.22f);
	gLights[3]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[3]->setColor(0.8f, 0.5f, 0.4f);
	gLights[3]->setIntensity(2.0f);
	gLights[3]->setRange(5.0f);
	gLights[3]->setSpotAngle(0.0f);

	// World-Space Left Pillar 4
	gLights[4] = std::make_shared<Odyssey::Light>();
	gLights[4]->setLightType(Odyssey::LightType::Point);
	gLights[4]->setPosition(-5.45f, 4.75f, -13.22f);
	gLights[4]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[4]->setColor(0.8f, 0.5f, 0.4f);
	gLights[4]->setIntensity(2.0f);
	gLights[4]->setRange(5.0f);
	gLights[4]->setSpotAngle(0.0f);

	// World-Space Right Pillar 1
	gLights[5] = std::make_shared<Odyssey::Light>();
	gLights[5]->setLightType(Odyssey::LightType::Point);
	gLights[5]->setPosition(5.45f, 4.75f, 14.4f);
	gLights[5]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[5]->setColor(0.8f, 0.5f, 0.4f);
	gLights[5]->setIntensity(2.0f);
	gLights[5]->setRange(5.0f);
	gLights[5]->setSpotAngle(0.0f);

	// World-Space Right Pillar 2
	gLights[6] = std::make_shared<Odyssey::Light>();
	gLights[6]->setLightType(Odyssey::LightType::Point);
	gLights[6]->setPosition(5.45f, 4.75f, 7.5f);
	gLights[6]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[6]->setColor(0.8f, 0.5f, 0.4f);
	gLights[6]->setIntensity(2.0f);
	gLights[6]->setRange(5.0f);
	gLights[6]->setSpotAngle(0.0f);

	// World-Space Right Pillar 3
	gLights[7] = std::make_shared<Odyssey::Light>();
	gLights[7]->setLightType(Odyssey::LightType::Point);
	gLights[7]->setPosition(5.45f, 4.75f, -13.22f);
	gLights[7]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[7]->setColor(0.8f, 0.5f, 0.4f);
	gLights[7]->setIntensity(2.0f);
	gLights[7]->setRange(5.0f);
	gLights[7]->setSpotAngle(0.0f);

	// World-Space Left Door Light 1
	gLights[8] = std::make_shared<Odyssey::Light>();
	gLights[8]->setLightType(Odyssey::LightType::Point);
	gLights[8]->setPosition(-12.0f, 4.75f, -6.7f);
	gLights[8]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[8]->setColor(0.8f, 0.5f, 0.4f);
	gLights[8]->setIntensity(2.0f);
	gLights[8]->setRange(5.0f);
	gLights[8]->setSpotAngle(0.0f);

	// World-Space Left Door Light 2
	gLights[9] = std::make_shared<Odyssey::Light>();
	gLights[9]->setLightType(Odyssey::LightType::Point);
	gLights[9]->setPosition(-12.0f, 4.75f, 1.2f);
	gLights[9]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[9]->setColor(0.8f, 0.5f, 0.4f);
	gLights[9]->setIntensity(2.0f);
	gLights[9]->setRange(5.0f);
	gLights[9]->setSpotAngle(0.0f);

	// World-Space Right Door Light 1
	gLights[10] = std::make_shared<Odyssey::Light>();
	gLights[10]->setLightType(Odyssey::LightType::Point);
	gLights[10]->setPosition(12.74f, 5.0f, -2.85f);
	gLights[10]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[10]->setColor(0.8f, 0.5f, 0.4f);
	gLights[10]->setIntensity(2.0f);
	gLights[10]->setRange(5.0f);
	gLights[10]->setSpotAngle(0.0f);

	// World-Space Right Door Light 2
	gLights[11] = std::make_shared<Odyssey::Light>();
	gLights[11]->setLightType(Odyssey::LightType::Point);
	gLights[11]->setPosition(12.74f, 5.0f, 4.25f);
	gLights[11]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[11]->setColor(0.8f, 0.5f, 0.4f);
	gLights[11]->setIntensity(2.0f);
	gLights[11]->setRange(5.0f);
	gLights[11]->setSpotAngle(0.0f);

	// Library-Area Candle Light
	gLights[12] = std::make_shared<Odyssey::Light>();
	gLights[12]->setLightType(Odyssey::LightType::Point);
	gLights[12]->setPosition(-1.25f, 12.5f, -35.0f);
	gLights[12]->setDirection(0.0f, 0.0f, 0.0f);
	gLights[12]->setColor(0.8f, 0.5f, 0.4f);
	gLights[12]->setIntensity(2.0f);
	gLights[12]->setRange(12.5f);

	gGameScene->addLight(gDirLight);
	gGameScene->addLight(gLights[0]);
	gGameScene->addLight(gLights[1]);
	gGameScene->addLight(gLights[2]);
	gGameScene->addLight(gLights[3]);
	gGameScene->addLight(gLights[4]);
	gGameScene->addLight(gLights[5]);
	gGameScene->addLight(gLights[6]);
	gGameScene->addLight(gLights[7]);
	gGameScene->addLight(gLights[8]);
	gGameScene->addLight(gLights[9]);
	gGameScene->addLight(gLights[10]);
	gGameScene->addLight(gLights[11]);
	gGameScene->addLight(gLights[12]);
}

void setupCamera()
{
	gMainCamera = std::make_shared<Odyssey::GameObject>();
	gMainCamera->addComponent<Odyssey::Transform>();
	gMainCamera->getComponent<Odyssey::Transform>()->setPosition(7.44f, 6.13f, 4.03f);
	gMainCamera->getComponent<Odyssey::Transform>()->setRotation(23.5f, -129.55f, 0.0f);
	gMainCamera->addComponent<Odyssey::Camera>();
	gMainCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gMainCamera->addComponent<CameraController>();
	gGameScene->addGameObject(gMainCamera);
}

void setupMainMenu(Odyssey::RenderDevice* renderDevice, Odyssey::Application* application)
{
	gMainMenu = renderDevice->createScene();
	gMenu = std::make_shared<Odyssey::GameObject>();
	gMenu->addComponent<Odyssey::Transform>();
	gMenu->addComponent<Odyssey::UICanvas>();
	gMenu->addComponent<Odyssey::Camera>();
	gMenu->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	UINT width = static_cast<UINT>(gMainWindow->mMainWindow.width);
	UINT height = static_cast<UINT>(gMainWindow->mMainWindow.height);
	gMenu->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/MainMenu.png", width, height);
	gMenu->addComponent<MainMenuController>(application);
	gMainMenu->addGameObject(gMenu);
}

void setupArena()
{
	Odyssey::FileManager::getInstance().importScene(gGameScene, "assets/models/TestArena.dxm");
}

void setupGameInterface()
{
	// Create the menu object and add the UI Canvas component
	gGameMenu = std::make_shared<Odyssey::GameObject>();
	gGameMenu->addComponent<Odyssey::UICanvas>(); // The player's icons and health bars
	gGameMenu->addComponent<Odyssey::UICanvas>(); // The rewards screen


	// Get a reference to the UI canvas and get the width/height of the screen
	Odyssey::UICanvas* canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[0];
	float width = static_cast<float>(gMainWindow->mMainWindow.width);
	float height = static_cast<float>(gMainWindow->mMainWindow.height);

	// Construct the UI
	// Battle for Flavortown Title
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2((width / 3.0f) + 50.0f, 15.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"Battle for Flavortown", 40.0f, 500.0f, 50.0f);

	// Player Team - Character 1
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(25.0f, 45.0f), L"assets/images/Guy.png", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 95.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"1", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	Character* pal = gPaladin->getComponent<Character>();
	pal->pHealthBar = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 120.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	pal->pManaBar = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 137.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);
	
	// Buff Icon Locations
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(125.0f, 45.0f), L"assets/images/AttackUp.png", 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(125.0f, 60.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), L"1", 10.0f, 25.0f, 25.0f);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(155.0f, 45.0f), L"assets/images/SpeedUp.png", 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(155.0f, 60.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), L"2", 10.0f, 25.0f, 25.0f);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(185.0f, 45.0f), L"assets/images/DefenseUp.jpg", 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(185.0f, 60.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), L"3", 10.0f, 25.0f, 25.0f);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(125.0f, 95.0f), L"assets/images/SpeedDown.png", 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(125.0f, 110.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), L"1", 10.0f, 25.0f, 25.0f);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(155.0f, 95.0f), L"assets/images/Bleed.png", 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(155.0f, 110.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), L"2", 10.0f, 25.0f, 25.0f);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(185.0f, 95.0f), L"assets/images/Poison.png", 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(185.0f, 110.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), L"3", 10.0f, 25.0f, 25.0f);

	// Player Team - Character 2
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(25.0f, 180.0f), L"assets/images/Guy.png", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 230.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"2", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 255.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 275.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Player Team - Character 3
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(25.0f, 315.0f), L"assets/images/Guy.png", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 365.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"3", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 390.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 410.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Player Team - Character 4
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(25.0f, 455.0f), L"assets/images/Guy.png", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 505.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"4", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 530.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(25.0f, 550.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Enemy Team - Character 1
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(width - 100.0f, 45.0f), L"assets/images/Gordon.jpg", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(width - 125.0f, 95.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"5", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	Character* skel = gSkeleton->getComponent<Character>();
	skel->pHealthBar = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 120.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	skel->pManaBar = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 140.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Enemy Team - Character 2
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(width - 100.0f, 180.0f), L"assets/images/Gordon.jpg", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(width - 125.0f, 230.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"6", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 255.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 275.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Enemy Team - Character 3
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(width - 100.0f, 315.0f), L"assets/images/Gordon.jpg", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(width - 125.0f, 365.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"7", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 390.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 410.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Enemy Team - Character 4
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(width - 100.0f, 455.0f), L"assets/images/Gordon.jpg", 75, 75);
	// Turn Order Text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(width - 125.0f, 505.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"8", 24.0f, 50.0f, 50.0f);
	// Health and Mana bars
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 530.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 75.0f, 15.0f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(width - 100.0f, 550.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 75.0f, 15.0f);

	// Results Menu
	canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[1];
	UINT rewardsImageWidth = 1280;
	UINT rewardsImageHeight = 720;
	float rewardsImageX = width / 2.0f - (static_cast<float>(rewardsImageWidth) / 2.0f);
	float rewardsImageY = height / 2.0f - (static_cast<float>(rewardsImageHeight) / 2.0f);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX, rewardsImageY), L"assets/images/ResultsMenu.png", rewardsImageWidth, rewardsImageHeight);
	canvas->setActive(false); // The rewards screen won't show up at the start

	gGameScene->addGameObject(gGameMenu);
}

void setupPaladin()
{
	gPaladin = std::make_shared<Odyssey::GameObject>();
	gPaladin->addComponent<Odyssey::Transform>();
	gPaladin->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gPaladin->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.6f, 3.0f);
	gPaladin->getComponent<Odyssey::Transform>()->setRotation(0.0f, 180.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(gPaladin, "assets/models/Paladin.dxm", true);
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("AttackUp", "assets/animations/Paladin/Paladin_AttackUp.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("BasicAttack", "assets/animations/Paladin/Paladin_BasicAttack.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("BigAttack", "assets/animations/Paladin/Paladin_BigAttack.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Defense", "assets/animations/Paladin/Paladin_Defense.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Heal", "assets/animations/Paladin/Paladin_Heal.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Shield", "assets/animations/Paladin/Paladin_Shield.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Paladin/Paladin_Idle.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Stun", "assets/animations/Paladin/Paladin_Kick.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Dead", "assets/animations/Paladin/Paladin_Death.dxanim");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Hit", "assets/animations/Paladin/Paladin_Hit.dxanim");
	gPaladin->addComponent<HeroComponent>(HEROID::Paladin);
	gGameScene->addGameObject(gPaladin);
}

void setupSkeleton()
{
	gSkeleton = std::make_shared<Odyssey::GameObject>();
	gSkeleton->addComponent<Odyssey::Transform>();
	gSkeleton->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gSkeleton->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.5f, -10.0f);
	gSkeleton->getComponent<Odyssey::Transform>()->setRotation(0.0f, 180, 0);
	Odyssey::FileManager::getInstance().importModel(gSkeleton, "assets/models/Skeleton.dxm", false);
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Skeleton/Skeleton_Idle.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("BasicAttackButBetter", "assets/animations/Skeleton/Skeleton_BasicAttack.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("Dead", "assets/animations/Skeleton/Skeleton_Death.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("FwdKick", "assets/animations/Skeleton/Skeleton_FwdKick.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("Hit", "assets/animations/Skeleton/Skeleton_Hit.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("SpinKick", "assets/animations/Skeleton/Skeleton_SpinKick.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->setDebugEnabled(true);
	gSkeleton->addComponent<EnemyComponent>(ENEMYID::Skeleton);
	gGameScene->addGameObject(gSkeleton);
}

void setupAudio()
{
	//SFX
	RedAudioManager::Instance()->AddAudio("assets/audio/bone_punch.mp3", "SkeletonAttack");
	RedAudioManager::Instance()->AddAudio("assets/audio/sword_slash.mp3", "PaladinAttack");
	RedAudioManager::Instance()->AddAudio("assets/audio/armor_hit.mp3", "PaladinHit");
	RedAudioManager::Instance()->AddAudio("assets/audio/losing.mp3", "Loss");
	//Background Sound
	RedAudioManager::Instance()->AddAudio("assets/audio/battle_music.mp3", "BackgroundBattle");
	RedAudioManager::Instance()->AddAudio("assets/audio/menu_music.mp3", "BackgroundMenu");
	//Play Initial Loop
	RedAudioManager::Instance()->Loop("BackgroundMenu");
}

void setUpTowerManager()
{
	gPlayerUnit.push_back(gPaladin);
	gEnemyUnit.push_back(gSkeleton);
	gCurrentTower = std::make_shared<Odyssey::GameObject>();
	gCurrentTower->addComponent<TowerManager>(gPlayerUnit, gEnemyUnit, 5);
	gCurrentTower->getComponent<TowerManager>()->UI = gGameMenu->getComponents<Odyssey::UICanvas>()[0];
	gCurrentTower->getComponent<TowerManager>()->Rewards = gGameMenu->getComponents<Odyssey::UICanvas>()[1];
	gCurrentTower->getComponent<TowerManager>()->TurnOrderNumbers.push_back(gGameMenu->getComponents<Odyssey::UICanvas>()[0]->getElements<Odyssey::Text2D>()[1]);
	gCurrentTower->getComponent<TowerManager>()->TurnOrderNumbers.push_back(gGameMenu->getComponents<Odyssey::UICanvas>()[0]->getElements<Odyssey::Text2D>()[11]);
	gGameScene->addGameObject(gCurrentTower);
}

int main()
{
	return playGame();
}