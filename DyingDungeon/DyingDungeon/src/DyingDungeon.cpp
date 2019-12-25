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

#include <stdio.h>
#include <DbgHelp.h>
#include <time.h>

// Game Includes
#include "TowerManager.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"
#include "CameraController.h"

#pragma comment(lib, "dbghelp.lib")

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
	Odyssey::Text2D::TextProperties gDefaultText;
}

// Forward declarations
int playGame();
void setupPipeline(Odyssey::RenderDevice* renderDevice, std::shared_ptr<Odyssey::Application> application);
void setupLighting();
void setupCamera();
void setupMainMenu(Odyssey::RenderDevice* renderDevice, Odyssey::Application* application);
void setupArena();
void setupGameInterface();
void setup4PlayerInterface();
void setupPaladin();
void setupSkeleton();
void setupAudio();
LONG WINAPI DumpOutput(struct _EXCEPTION_POINTERS* in_error);
// Factories
void createCharacterPortrait(float anchorX, float anchorY, const wchar_t* image, Odyssey::UICanvas* canvas, Character* owner);
void createBuffIcon(float anchorX, float anchorY, int slot, int buildDirection, const wchar_t* image, float width, float height, Odyssey::UICanvas* canvas, Character* owner);

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

	gDefaultText.bold = false;
	gDefaultText.italic = false;
	gDefaultText.fontSize = 25.0f;
	gDefaultText.textAlignment = Odyssey::Text2D::TextAlignment::Left;
	gDefaultText.paragraphAlignment = Odyssey::Text2D::ParagraphAlignment::Left;
	gDefaultText.fontName = L"Verdana";

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
	setup4PlayerInterface();

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
	std::shared_ptr<Odyssey::DebugPass>debugPass = renderDevice->createDebugPass(gMainWindow->getRenderTarget());
	debugPass->setEnabled(false);
	application->addRenderPass(debugPass);
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
	// Create the game menu object
	gGameMenu = std::make_shared<Odyssey::GameObject>();

	// Add a UI Canvas for the 1v1 combat
	gGameMenu->addComponent<Odyssey::UICanvas>(); // The player's icons and health bars

	// Add a UI Canvas for the rewards screen
	gGameMenu->addComponent<Odyssey::UICanvas>(); // The rewards screen

	// Get a reference to the UI canvas and get the width/height of the screen
	Odyssey::UICanvas* canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[0];

	// Get the width and height of the window
	float width = static_cast<float>(gMainWindow->mMainWindow.width);
	float height = static_cast<float>(gMainWindow->mMainWindow.height);

	// Battle for Flavortown Title
	Odyssey::Text2D::TextProperties properties = gDefaultText;
	properties.fontSize = 40.0f;
	properties.textAlignment = Odyssey::Text2D::TextAlignment::Center;
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(0.0f, height / 75.0f), DirectX::XMFLOAT4(1, 0, 0, 1), L"Battle for Flavortown", properties, width, 0.0f);

	float anchorX = width / 75.0f;
	float anchorY = height / 25.0f;

	// Set up 4 player characters
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Guy.png", canvas, gPaladin->getComponent<Character>());

	anchorX = width - anchorX - (width / 25.0f);
	anchorY = height / 25.0f;
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Gordon.jpg", canvas, gSkeleton->getComponent<Character>());

	// Buff Icon Locations
	float iconSize = width / 50;

	anchorX = width / 75.0f + iconSize * 2.5f;
	anchorY = height / 25.0f;

	createBuffIcon(anchorX, anchorY, 1, 1, L"assets/images/AttackUp.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 2, 1, L"assets/images/SpeedUp.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 3, 1, L"assets/images/DefenseUp.jpg", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 4, 1, L"assets/images/SpeedDown.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 5, 1, L"assets/images/Bleed.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 6, 1, L"assets/images/Poison.png", iconSize, iconSize, canvas, nullptr);

	anchorX = width - (width / 25.0f) - width / 75.0f;
	anchorX -= iconSize * 1.5f;
	createBuffIcon(anchorX, anchorY, 1, -1, L"assets/images/AttackUp.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 2, -1, L"assets/images/DefenseUp.jpg", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 3, -1, L"assets/images/SpeedUp.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 4, -1, L"assets/images/SpeedDown.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 5, -1, L"assets/images/Poison.png", iconSize, iconSize, canvas, nullptr);
	createBuffIcon(anchorX, anchorY, 6, -1, L"assets/images/Bleed.png", iconSize, iconSize, canvas, nullptr);

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

void setup4PlayerInterface()
{
	// Add a UI Canvas for 4v4 HUD
	gGameMenu->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[2];

	// Get the width and height of the window
	float width = static_cast<float>(gMainWindow->mMainWindow.width);
	float height = static_cast<float>(gMainWindow->mMainWindow.height);

	float anchorX = width / 75.0f;
	float anchorY = (height / 25.0f) + (height / 7.0f);

	// Set up the other 3 player characters
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Guy.png", canvas, nullptr);
	anchorY += (height / 7.0f);
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Guy.png", canvas, nullptr);
	anchorY += (height / 7.0f);
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Guy.png", canvas, nullptr);

	// Move the anchor points to accomodate the right side
	anchorX = width - anchorX - (width / 25.0f);
	anchorY = (height / 25.0f) + (height / 7.0f);

	// Set up the other 3 enemy characters
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Gordon.jpg", canvas, nullptr);
	anchorY += (height / 7.0f);
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Gordon.jpg", canvas, nullptr);
	anchorY += (height / 7.0f);
	createCharacterPortrait(anchorX, anchorY, L"assets/images/Gordon.jpg", canvas, nullptr);

	canvas->setActive(false);
}

void createCharacterPortrait(float anchorX, float anchorY, const wchar_t* image, Odyssey::UICanvas* canvas, Character* owner)
{
	// Get the width and height of the window
	float width = static_cast<float>(gMainWindow->mMainWindow.width);
	float height = static_cast<float>(gMainWindow->mMainWindow.height);
	Odyssey::Text2D::TextProperties properties = gDefaultText;
	properties.fontSize = height / 50.0f;
	properties.bold = true;

	// Player Team - Character 1
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(anchorX, anchorY), image, (UINT)width / 25, (UINT)width / 25);

	// Turn Order Text
	float offsetX = width / 25.0f;
	float offsetY = height / 23.0f;
	if (owner && owner->IsHero()==false)
	{
		offsetX = -properties.fontSize / 1.5f;
	}
	properties.textAlignment = Odyssey::Text2D::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::Text2D::ParagraphAlignment::Left;
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(anchorX, anchorY), DirectX::XMFLOAT4(1, 0.84f, 0, 1), L"1", properties, width / 25, width / 25);
	// Health and Mana bars
	Character* pal = gPaladin->getComponent<Character>();
	if (owner)
	{
		owner->pHealthBar = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + height / 14.25f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), width / 25.0f, height / 70.0f);
		owner->pManaBar = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + height / 11.75f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), width / 25.0f, height / 70.0f);
	}
	else
	{
		canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + height / 14.25f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), width / 25.0f, height / 70.0f);
		canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + height / 11.75f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), width / 25.0f, height / 70.0f);
	}
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
	RedAudioManager::Instance().AddAudio("assets/audio/bone_punch.mp3", "SkeletonAttack");
	RedAudioManager::Instance().AddAudio("assets/audio/sword_slash.mp3", "PaladinAttack");
	RedAudioManager::Instance().AddAudio("assets/audio/armor_hit.mp3", "PaladinHit");
	RedAudioManager::Instance().AddAudio("assets/audio/losing.mp3", "Loss");
	//Background Sound
	RedAudioManager::Instance().AddAudio("assets/audio/battle_music.mp3", "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/menu_music.mp3", "BackgroundMenu");
	//Play Initial Loop
	RedAudioManager::Instance().Loop("BackgroundMenu");
}

void createBuffIcon(float anchorX, float anchorY, int slot, int buildDirection, const wchar_t* image, float width, float height, Odyssey::UICanvas* canvas, Character* owner)
{
	Odyssey::Text2D::TextProperties properties = gDefaultText;
	properties.fontSize = gMainWindow->mMainWindow.height / 65.0f;
	properties.bold = true;
	float iconStepX = width * 1.25f;
	float iconStepY = height * 1.25f;
	slot -= 1;
	int number = (slot % 3) + 1;
	float xPos = anchorX + buildDirection * (static_cast<float>(slot % 3) * iconStepX);
	float yPos = anchorY + (static_cast<float>(slot / 3) * iconStepY);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xPos, yPos), image, static_cast<UINT>(width), static_cast<UINT>(height));
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(xPos, yPos + height / 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), std::to_wstring(number), properties, width, height);
}

void setUpTowerManager()
{
	gPlayerUnit.push_back(gPaladin);
	gEnemyUnit.push_back(gSkeleton);
	gCurrentTower = std::make_shared<Odyssey::GameObject>();
	gCurrentTower->addComponent<TowerManager>(gPlayerUnit, gEnemyUnit, 5);
	gCurrentTower->getComponent<TowerManager>()->UI = gGameMenu->getComponents<Odyssey::UICanvas>()[0];
	gCurrentTower->getComponent<TowerManager>()->Rewards = gGameMenu->getComponents<Odyssey::UICanvas>()[1];
	gCurrentTower->getComponent<TowerManager>()->addHUD = gGameMenu->getComponents<Odyssey::UICanvas>()[2];
	gCurrentTower->getComponent<TowerManager>()->TurnOrderNumbers.push_back(gGameMenu->getComponents<Odyssey::UICanvas>()[0]->getElements<Odyssey::Text2D>()[1]);
	gCurrentTower->getComponent<TowerManager>()->TurnOrderNumbers.push_back(gGameMenu->getComponents<Odyssey::UICanvas>()[0]->getElements<Odyssey::Text2D>()[2]);
	gGameScene->addGameObject(gCurrentTower);
}

LONG WINAPI DumpOutput(struct _EXCEPTION_POINTERS* in_error)
{
	struct tm newTime;
	time_t locTime;
	wchar_t path[100] = { 0 };

	locTime = time(&locTime);
	localtime_s(&newTime, &locTime);
	//../crash_log/
	wcsftime(path, sizeof(path), L"crash logs/%A_%b%d_%I%M%p.dmp", &newTime);

	//fstream crashlog(path, ios::trunc | ios::out);

	HANDLE hFile = ::CreateFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = in_error;
		ExInfo.ClientPointers = FALSE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
		::CloseHandle(hFile);
	}

	return 0;
}

int main()
{
	SetUnhandledExceptionFilter(DumpOutput);
	//DumpFile Test
	/*int test = 120;
	int tester = 12;
	if (test == tester) {

	}
	else {
		test = 0;
	}
	int newer = tester / test;*/
	return playGame();
}