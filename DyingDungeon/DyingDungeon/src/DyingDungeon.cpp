#include "DyingDungeon.h"

// General includes
#include <stdio.h>
#include <DbgHelp.h>
#include <time.h>

//Game UI Manager
#include "GameUIManager.h"

// Game Includes
#include "TowerManager.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"
#include "CameraController.h"
#include "CharacterFactory.h"
#include "RedAudioManager.h"
#include "MainMenuController.h"
#include "TowerSelectController.h"
#include "TeamSelectionController.h"
#include "StatTracker.h"
#include "ParticleMover.h"
#include "SkillHoverComponent.h"

// Engine includes
#include "OdysseyEngine.h"
#pragma comment(lib, "dbghelp.lib")
#include "Material.h"

// Memory leak includes
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

enum MenuComponent
{
	eMainMenu,
	eTowerSelector,
	eTeamSelector
};

namespace
{
	// Rendering resources
	std::shared_ptr<Odyssey::RenderWindow> gMainWindow;
	std::shared_ptr<Odyssey::RenderTarget> gRenderTarget;
	Odyssey::RenderDevice* gRenderDevice;
	// Scene resources
	std::shared_ptr<Odyssey::Scene> gGameScene;
	std::shared_ptr<Odyssey::Scene> gMainMenu;
	std::shared_ptr<Odyssey::Scene> gTowerSelectScene;
	std::shared_ptr<Odyssey::Scene> gTeamSelectScene;
	std::shared_ptr<Odyssey::Entity> gMainCamera;
	//Game Objects
	std::shared_ptr<Odyssey::Entity> gMenu;
	std::shared_ptr<Odyssey::Entity> gTowerSelectMenu;
	std::shared_ptr<Odyssey::Entity> gTeamSelectMenu;
	std::shared_ptr<Odyssey::Entity> gGameMenu;
	std::shared_ptr<Odyssey::Entity> gPaladin;
	std::shared_ptr<Odyssey::Entity> gSkeleton;
	std::shared_ptr<Odyssey::Entity> gCurrentTower;
	//Vectors
	std::vector<std::shared_ptr<Odyssey::Entity>> gPlayerUnit;
	std::vector<std::shared_ptr<Odyssey::Entity>> gEnemyUnit;
	// Light resources
	std::shared_ptr<Odyssey::Light> gDirLight;
	std::shared_ptr<Odyssey::Light> gLights[15];
	Odyssey::TextProperties gDefaultText;
	// Particle systems
	std::shared_ptr<Odyssey::Entity> gFireBall;
	std::shared_ptr<Odyssey::Entity> gFireStorm;
}

// Forward declarations
int playGame();
void setupPipeline(Odyssey::RenderDevice* renderDevice, std::shared_ptr<Odyssey::Application> application);
void setupLighting();
void setupCamera();
void setupMenu(Odyssey::RenderDevice* renderDevice, Odyssey::Application* application, std::shared_ptr<Odyssey::Scene>& _sceneObject, std::shared_ptr<Odyssey::Entity>& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent);
void setupMainMenu(Odyssey::Application* application);
void setupTeamSelectMenu(Odyssey::Application* application);
void setupArena();
void setupGameInterface();
void setupAudio();
LONG WINAPI DumpOutput(struct _EXCEPTION_POINTERS* in_error);
// Factories
void createCharacterHealthPopup(float anchorX, float anchorY, Odyssey::UICanvas* canvas, Character* owner);
void createBuffIcon(UINT anchorX, UINT anchorY, int slot, int buildDirection, const wchar_t* image, UINT width, UINT height, Odyssey::UICanvas* canvas, Character* owner);

// BUILD 2 STUFF
void setupFire();
void setupSkillHover(Odyssey::UICanvas* canvas, std::wstring character, std::wstring skillName, std::wstring icon, std::wstring manaCost, std::wstring skillType, std::wstring numTargets, std::wstring skillValue, std::wstring description);
void setupPaladinSkills(std::shared_ptr<Odyssey::Entity> character, float xAnchor, float yAnchor);
void setupMageSkills(std::shared_ptr<Odyssey::Entity> character, float xAnchor, float yAnchor);

//Tristen's Stuff
void setUpTowerManager();

int playGame()
{
	// TODO: BREAKPOINT FOR YOUR DUMBASS MEMORY LEAKS
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(231);
#endif // _DEBUG

	// Set up the application and create a render window
	std::shared_ptr<Odyssey::Application> application = std::make_shared<Odyssey::Application>();
	gMainWindow = application->createRenderWindow(L"Dying Dungeon", 1280, 720);
	
	// Get the render device
	gRenderDevice = application->getRenderDevice();

	gDefaultText.bold = false;
	gDefaultText.italic = false;
	gDefaultText.fontSize = 25.0f;
	gDefaultText.textAlignment = Odyssey::TextAlignment::Left;
	gDefaultText.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	gDefaultText.fontName = L"Constantia";

	// Create the main scene
	gGameScene = gRenderDevice->createScene();

	// Set up the scene lighting
	setupLighting();

	// Set up the default rendering pipeline
	setupPipeline(gRenderDevice, application);

	// Set up camera
	setupCamera();

	// Assign the width and height for the UI Manager
	GameUIManager::getInstance().SetScreenWidthAndHeight(gMainWindow->getWidth(), gMainWindow->getHeight());

	// Set up the main menu
	setupMainMenu(application.get());

	// Set up the tower selection screen
	setupMenu(gRenderDevice, application.get(), gTowerSelectScene, gTowerSelectMenu, L"assets/images/TowerSelectionBackground.png", "TowerSelection", MenuComponent::eTowerSelector);
	// Create the tower selection menu
	GameUIManager::getInstance().CreateTowerSelectMenuCanvas(gTowerSelectScene);

	// Set up the team selection screen
	setupTeamSelectMenu(application.get());

	// Create the battle log for the game
	GameUIManager::getInstance().CreateBattleLog(gGameScene);

	// Load the arena scene
	setupArena();

	// BUILD 2
	setupFire();

	// Set up the game user interface
	setupGameInterface();

	// Set up the tower manager
	setUpTowerManager();

	// Add the gGameMenu to the gGameScene after I have added all the elements
	gGameScene->addEntity(gGameMenu);

	// Create Pause Menu
	GameUIManager::getInstance().CreatePauseMenuCanvas(gGameScene);

	// Add the game scene to the application
	application->addScene("Game", gGameScene);
	application->setMultithreading(true);

	// Play audio
	setupAudio();

	// Run the application
	return application->run();
}

void setupPipeline(Odyssey::RenderDevice* renderDevice, std::shared_ptr<Odyssey::Application> application)
{
	// Create a clear render target pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ClearRenderTargetPass> rtvPass = renderDevice->createClearRTVPass(gMainWindow, true);
	application->addRenderPass(rtvPass);

	// Create a skybox pass and add it to the render pipeline 
	std::shared_ptr<Odyssey::SkyboxPass> skyboxPass = renderDevice->createSkyboxPass("Skybox.dds", gMainWindow);
	application->addRenderPass(skyboxPass);

	// Create a shadow pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ShadowPass> shadowPass = renderDevice->createShadowPass(gDirLight, 4096, 4096);
	application->addRenderPass(shadowPass);

	// Create an opaque pass and add it to the render pipeline
	std::shared_ptr<Odyssey::OpaquePass> opaquePass = renderDevice->createOpaquePass(gMainWindow);
	application->addRenderPass(opaquePass);

	// Create a transparent pass and add it to the render pipeline
	std::shared_ptr<Odyssey::TransparentPass> transparentPass = renderDevice->createTransparentPass(gMainWindow);
	application->addRenderPass(transparentPass);

	std::shared_ptr<Odyssey::Sprite2DPass> spritePass = renderDevice->createSprite2DPass(gMainWindow);
	application->addRenderPass(spritePass);
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
	gMainCamera = std::make_shared<Odyssey::Entity>();
	gMainCamera->addComponent<Odyssey::Transform>();
	gMainCamera->getComponent<Odyssey::Transform>()->setPosition(3.432f, 7.053f, 14.602f);
	gMainCamera->getComponent<Odyssey::Transform>()->setRotation(25.189f, -160.439f, 0.0f);
	gMainCamera->addComponent<Odyssey::Camera>();
	gMainCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gMainCamera->addComponent<CameraController>();
	gGameScene->addEntity(gMainCamera);
}

void setupMenu(Odyssey::RenderDevice* renderDevice, Odyssey::Application* application, std::shared_ptr<Odyssey::Scene>& _sceneObject, std::shared_ptr<Odyssey::Entity>& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent)
{
	_sceneObject = renderDevice->createScene();
	_entityToAdd = std::make_shared<Odyssey::Entity>();
	_entityToAdd->addComponent<Odyssey::Transform>();
	_entityToAdd->addComponent<Odyssey::UICanvas>();
	_entityToAdd->addComponent<Odyssey::Camera>();
	_entityToAdd->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	UINT width = gMainWindow->getWidth();
	UINT height = gMainWindow->getHeight();

	// Only add image if there is one
	if (_imageName != L"")
	{
		_entityToAdd->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), _imageName, width, height);
	}

	switch (_menuComponent)
	{
	case eMainMenu:
		_entityToAdd->addComponent<MainMenuController>(application);
		break;
	case eTowerSelector:
		_entityToAdd->addComponent<TowerSelectController>(application);
		break;
	case eTeamSelector:
	{
		_entityToAdd->addComponent<TeamSelectionController>(application);
		// TODO: REFACTOR THIS LATER
		_entityToAdd->getComponent<TeamSelectionController>()->setupHovers();
		break;
	}
	default:
		break;
	}
	
	_sceneObject->addEntity(_entityToAdd);

	// Add the scene to the application
	application->addScene(_menuName, _sceneObject);
}

void setupMainMenu(Odyssey::Application* application)
{
	// Create Menu
	setupMenu(gRenderDevice, application, gMainMenu, gMenu, L"", "MainMenu", MenuComponent::eMainMenu);

	// Set up a directional light
	std::shared_ptr<Odyssey::Light> dirLight = std::make_shared<Odyssey::Light>();
	dirLight->setLightType(Odyssey::LightType::Directional);
	dirLight->setPosition(0, 0, 0);
	dirLight->setDirection(0.75f, -0.45f, -0.055f);
	dirLight->setColor(0.4f, 0.5f, 0.7f);
	dirLight->setIntensity(1.0f);
	dirLight->setRange(0.0f);
	dirLight->setSpotAngle(0.0f);
	gMainMenu->addLight(dirLight);

	// Set up an ambient light
	std::shared_ptr<Odyssey::Light> ambientLight = std::make_shared<Odyssey::Light>();
	ambientLight->setLightType(Odyssey::LightType::Point);
	ambientLight->setPosition(0.0, 10.0f, 0.0f);
	ambientLight->setDirection(0.0f, 0.0f, 0.0f);
	ambientLight->setColor(0.5f, 0.5f, 0.5f);
	ambientLight->setIntensity(10.0f);
	ambientLight->setRange(30.0f);
	ambientLight->setSpotAngle(0.0f);
	gMainMenu->addLight(ambientLight);

	// Create a paladin and add him to the main menu scene
	std::shared_ptr<CharacterFactory> charFactory = std::make_shared<CharacterFactory>();
	// Set the render device for the particles
	charFactory->mRenderRefrence = gRenderDevice;
	std::shared_ptr<Odyssey::Entity> characterToAdd;
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(2.0f, -2.5f, 6.0f, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Paladin, "Main Menu Paladin", charPosition, charRotation, gMainMenu);
	gMainMenu->addEntity(characterToAdd);

	// Create the UI
	GameUIManager::getInstance().CreateMainMenuCanvas(gMainMenu);
}

void setupTeamSelectMenu(Odyssey::Application* application)
{
	// Set up the team selection screen
	setupMenu(gRenderDevice, application, gTeamSelectScene, gTeamSelectMenu, L"", "TeamSelection", MenuComponent::eTeamSelector);

	// Set up a directional light
	std::shared_ptr<Odyssey::Light> dirLight = std::make_shared<Odyssey::Light>();
	dirLight->setLightType(Odyssey::LightType::Directional);
	dirLight->setPosition(0, 0, 0);
	dirLight->setDirection(0.75f, -0.45f, -0.055f);
	dirLight->setColor(0.4f, 0.5f, 0.7f);
	dirLight->setIntensity(1.0f);
	dirLight->setRange(0.0f);
	dirLight->setSpotAngle(0.0f);
	gTeamSelectScene->addLight(dirLight);

	// Set up an ambient light
	std::shared_ptr<Odyssey::Light> ambientLight = std::make_shared<Odyssey::Light>();
	ambientLight->setLightType(Odyssey::LightType::Point);
	ambientLight->setPosition(0.0, 0.0f, 2.0f);
	ambientLight->setDirection(0.0f, 0.0f, 0.0f);
	ambientLight->setColor(0.5f, 0.5f, 0.5f);
	ambientLight->setIntensity(5.0f);
	ambientLight->setRange(30.0f);
	ambientLight->setSpotAngle(0.0f);
	gTeamSelectScene->addLight(ambientLight);

	// Create a paladin and add him to the team select scene
	std::shared_ptr<CharacterFactory> charFactory = std::make_shared<CharacterFactory>();
	// Set the render device for the particles
	charFactory->mRenderRefrence = gRenderDevice;
	std::shared_ptr<Odyssey::Entity> characterToAdd;
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(-1.0f, -2.0f, 6.0f, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Paladin, "Team Select Paladin", charPosition, charRotation, gTeamSelectScene);
	characterToAdd->getComponent<Odyssey::Transform>()->setScale(0.015f, 0.015f, 0.015f);
	gTeamSelectScene->addEntity(characterToAdd);

	// Create a mage and add him to the team select scene
	charPosition = DirectX::XMVectorSet(3.0f, -2.0f, 6.0f, 1.0f);
	charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Mage, "Team Select Mage", charPosition, charRotation, gTeamSelectScene);
	characterToAdd->getComponent<Odyssey::Transform>()->setScale(0.015f, 0.015f, 0.015f);
	gTeamSelectScene->addEntity(characterToAdd);

	// Create the UI
	GameUIManager::getInstance().CreateTeamSelectMenuCanvas(gTeamSelectScene);
}

void setupArena()
{
	Odyssey::FileManager::getInstance().importScene(gGameScene, "assets/models/TestArena.dxm");
}

void setupGameInterface()
{
	// Create the game menu object
	gGameMenu = std::make_shared<Odyssey::Entity>();

	// Add a UI Canvas for the 1v1 combat
	gGameMenu->addComponent<Odyssey::UICanvas>(); // The player's icons and health bars

	// Add a UI Canvas for the rewards screen
	gGameMenu->addComponent<Odyssey::UICanvas>(); // The rewards screen

	// Get a reference to the UI canvas and get the width/height of the screen
	Odyssey::UICanvas* canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[0];

	// Get the width and height of the window
	UINT width = gMainWindow->getWidth();
	UINT height = gMainWindow->getHeight();
	
	// Buff Icon Locations
	//createBuffIcon(80, 30, 1, 1, L"assets/images/AttackUp.png", 25, 25, canvas, nullptr);
	//createBuffIcon(80, 30, 2, 1, L"assets/images/SpeedUp.png", 25, 25, canvas, nullptr);
	//createBuffIcon(80, 30, 3, 1, L"assets/images/DefenseUp.jpg", 25, 25, canvas, nullptr);
	//createBuffIcon(80, 30, 4, 1, L"assets/images/SpeedDown.png", 25, 25, canvas, nullptr);
	//createBuffIcon(80, 30, 5, 1, L"assets/images/Bleed.png", 25, 25, canvas, nullptr);
	//createBuffIcon(80, 30, 6, 1, L"assets/images/Poison.png", 25, 25, canvas, nullptr);
	//
	//createBuffIcon(1112, 30, 1, 1, L"assets/images/AttackUp.png", 25, 25, canvas, nullptr);
	//createBuffIcon(1112, 30, 2, 1, L"assets/images/DefenseUp.jpg", 25, 25, canvas, nullptr);
	//createBuffIcon(1112, 30, 3, 1, L"assets/images/SpeedUp.png", 25, 25, canvas, nullptr);
	//createBuffIcon(1112, 30, 4, 1, L"assets/images/SpeedDown.png", 25, 25, canvas, nullptr);
	//createBuffIcon(1112, 30, 5, 1, L"assets/images/Poison.png", 25, 25, canvas, nullptr);
	//createBuffIcon(1112, 30, 6, 1, L"assets/images/Bleed.png", 25, 25, canvas, nullptr);

	// Results Menu
	canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[1];
	UINT rewardsImageWidth = width*0.8;
	UINT rewardsImageHeight = height*0.8;
	float rewardsImageX = (width / 2.0f) - (static_cast<float>(rewardsImageWidth) / 2.0f);
	float rewardsImageY = (height / 2.0f) - (static_cast<float>(rewardsImageHeight) / 2.0f);
	//canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX, rewardsImageY), L"assets/images/ResultsMenu.png", rewardsImageWidth, rewardsImageHeight);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX, rewardsImageY), DirectX::XMFLOAT4(63.75f, 63.75f, 63.75f, 0.75f), rewardsImageWidth, rewardsImageHeight);

	Odyssey::TextProperties rewardsTextProperties = gDefaultText;
	rewardsTextProperties.fontSize = 30.0f;
	rewardsTextProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f, rewardsImageY + 30.0f), L"assets/images/PaladinPortrait.jpg", rewardsImageHeight /4, rewardsImageHeight /4);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f, rewardsImageY + (30.0f + rewardsImageHeight * 0.3333f)), L"assets/images/PaladinPortrait.jpg", rewardsImageHeight / 4, rewardsImageHeight / 4);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f, rewardsImageY + (30.0f + rewardsImageHeight * 0.6667f)), L"assets/images/PaladinPortrait.jpg", rewardsImageHeight / 4, rewardsImageHeight / 4);

	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f + (rewardsImageHeight / 4), rewardsImageY + 30.0f ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), width, height / 6, L"P1 - Attack: NN.NN% Defence: NN.NN% Aid: NN.NN%", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f + (rewardsImageHeight / 4), rewardsImageY + (30.0f + rewardsImageHeight *0.3333f) ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), width, height / 6, L"P2 - Attack: NN.NN% Defence: NN.NN% Aid: NN.NN%", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f + (rewardsImageHeight / 4), rewardsImageY + (30.0f + rewardsImageHeight * 0.6667f) ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), width, height / 6, L"P3 - Attack: NN.NN% Defence: NN.NN% Aid: NN.NN%", rewardsTextProperties);
	canvas->setActive(false); // The rewards screen won't show up at the start
	StatTracker::Instance().SetRewardsScreen(canvas);
}

void createCharacterHealthPopup(float anchorX, float anchorY, Odyssey::UICanvas* canvas, Character* owner)
{
	// Set properties
	Odyssey::TextProperties properties = gDefaultText;
	properties.fontSize = 18.0f;
	properties.bold = true;

	if (owner)
	{
		// Add Text2D element to the screen
		owner->pDmgText = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(anchorX, anchorY), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 72, 72, L"11.25", properties);
		owner->pDmgText->setOpacity(0.0f);
	}
	else
	{
		// Add Text2D element to the screen
		Odyssey::Text2D* text = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(anchorX, anchorY), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 72, 72, L"11.25", properties);
		text->setOpacity(1.0f);
	}
}

void setupAudio()
{
	//SFX
	RedAudioManager::Instance().AddAudio("assets/audio/bone_punch.mp3", "SkeletonAttack");
	RedAudioManager::Instance().AddAudio("assets/audio/sword_slash.mp3", "PaladinAttack");
	RedAudioManager::Instance().AddAudio("assets/audio/armor_hit.mp3", "PaladinHit");
	RedAudioManager::Instance().AddAudio("assets/audio/losing.mp3", "Loss");

	// Tower select screen door sounds
	RedAudioManager::Instance().AddAudio("assets/audio/Door_Open.mp3", "DoorOpen");
	RedAudioManager::Instance().AddAudio("assets/audio/Door_Close.mp3", "DoorClose");
	
	//Background Sound
	RedAudioManager::Instance().AddAudio("assets/audio/battle_music.mp3", "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/menu_music.mp3", "BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_4.mp3", "NoManaCritical");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_3.mp3", "NoManaBitch");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_2.mp3", "NoManaMidium");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_1.mp3", "NoManaLow");
	
	//Play Initial Loop
	//RedAudioManager::Instance().Loop("BackgroundMenu");
	//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
}

void createBuffIcon(UINT anchorX, UINT anchorY, int slot, int buildDirection, const wchar_t* image, UINT width, UINT height, Odyssey::UICanvas* canvas, Character* owner)
{
	Odyssey::TextProperties properties = gDefaultText;
	properties.fontSize = 12.0f;
	properties.bold = true;
	UINT iconStepX = static_cast<UINT>(width * 1.25f);
	UINT iconStepY = static_cast<UINT>(height * 1.25f);
	slot -= 1;
	int number = (slot % 3) + 1;
	float xPos = static_cast<float>(anchorX + (slot % 3) * iconStepX);
	float yPos = static_cast<float>(anchorY + (slot / 3) * iconStepY);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xPos, yPos), image, width, height);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(xPos, yPos + height / 2.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width, height, std::to_wstring(number), properties);
}

void setupFire()
{
	std::shared_ptr<Odyssey::Entity> fire1 = std::make_shared<Odyssey::Entity>();
	fire1->addComponent<Odyssey::Transform>();
	fire1->getComponent<Odyssey::Transform>()->setPosition(-5.65f, 4.67f, -6.42f);
	fire1->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire1->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire1->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire1->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire1->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire1->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire1->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire2 = std::make_shared<Odyssey::Entity>();
	fire2->addComponent<Odyssey::Transform>();
	fire2->getComponent<Odyssey::Transform>()->setPosition(-5.65f, 4.67f, -13.42f);
	fire2->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire2->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire2->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire2->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire2->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire2->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire2->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire3 = std::make_shared<Odyssey::Entity>();
	fire3->addComponent<Odyssey::Transform>();
	fire3->getComponent<Odyssey::Transform>()->setPosition(5.65f, 4.67f, -13.42f);
	fire3->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire3->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire3->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire3->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire3->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire3->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire3->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire4 = std::make_shared<Odyssey::Entity>();
	fire4->addComponent<Odyssey::Transform>();
	fire4->getComponent<Odyssey::Transform>()->setPosition(-12.6f, 4.17f, -6.42f);
	fire4->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire4->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire4->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire4->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire4->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire4->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire4->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire5 = std::make_shared<Odyssey::Entity>();
	fire5->addComponent<Odyssey::Transform>();
	fire5->getComponent<Odyssey::Transform>()->setPosition(12.65f, 4.67f, -2.92f);
	fire5->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire5->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire5->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire5->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire5->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire5->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire5->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire5->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire5->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire5->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire5->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	gGameScene->addEntity(fire1);
	gGameScene->addEntity(fire2);
	gGameScene->addEntity(fire3);
	gGameScene->addEntity(fire4);
	gGameScene->addEntity(fire5);
}

void setupSkillHover(Odyssey::UICanvas* canvas, std::wstring character, std::wstring skillName, std::wstring icon, std::wstring manaCost, std::wstring skillType, std::wstring numTargets, std::wstring skillValue, std::wstring description)
{
	// Append the number of targets
	std::wstring targets = L"Targets: ";
	std::wstring valueText;
	targets = targets.append(numTargets.c_str());

	DirectX::XMFLOAT4 themeColor;

	if (character == L"Paladin")
	{
		themeColor = DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f);
	}
	else if (character == L"Mage")
	{
		themeColor = DirectX::XMFLOAT4(31.0f, 255.0f, 203.0f, 1.0f);
	}

	if (skillType == L"Attack")
	{
		valueText = L"Damage: ";
		valueText = valueText.append(skillValue.c_str());
	}
	else if (skillType == L"Support")
	{
		valueText = L"Value: ";
		valueText = valueText.append(skillValue.c_str());
	}
	else if (skillType == L"Heal")
	{
		valueText = L"Heal: ";
		valueText = valueText.append(skillValue.c_str());
	}

	UINT windowWidth = gMainWindow->getWidth();
	UINT windowHeight = gMainWindow->getHeight();
	float x = 950;
	float y = 425;
	UINT width = 300;
	UINT height = 150;
	UINT pad = 10;

	Odyssey::TextProperties title;
	title.bold = true;
	title.italic = false;
	title.fontSize = 24.0f;
	title.textAlignment = Odyssey::TextAlignment::Center;
	title.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	title.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	properties.fontName = L"Tw Cen MT Condensed";

	// Background and Separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 40), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 80), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);

	// Title Text and Icons
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 40, y), themeColor, width - 80, 40, skillName, title);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), icon, 40, 40);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f), 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(0.0f, 122.5f, 122.5f, 1.0f), 40, 40, manaCost, title);

	// Skill Info
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 40 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 25 + pad, y + 50), themeColor, 150, 50, skillType, properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 80 + pad, y + 40 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 105 + pad, y + 50), themeColor, 150, 50, targets, properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 175 + pad, y + 40 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 200 + pad, y + 50), themeColor, 150, 50, skillValue, properties);

	// Description
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 85),
		DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), height - 110 - pad, description, properties);
	canvas->setActive(false);
}

void setupPaladinSkills(std::shared_ptr<Odyssey::Entity> character, float xAnchor, float yAnchor)
{
	// TODO: REFACTOR THIS LATER
	// Add skill icons to portrait
	Odyssey::UICanvas* canvas1 = character->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas2 = character->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas3 = character->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas4 = character->addComponent<Odyssey::UICanvas>();
	SkillHoverComponent* hover = character->addComponent<SkillHoverComponent>();

	// Basic Attack icon
	Odyssey::Sprite2D* skill1 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_1.png", 52, 45);
	// Basic Attack hover
	setupSkillHover(canvas1, L"Paladin", L"Basic Attack", L"assets/images/Paladin_Skill_1.png", L"0", L"Attack", L"1", L"15 dmg",
		L"Description: Strike an enemy with divine power dealing 15 damage with a 30% chance to apply provoke. Restores 5 mana.");
	// Basic Attack trigger
	hover->registerSprite(skill1, canvas1);

	// Increment the icon
	xAnchor += 56.5f;

	// Wind Slash icon
	Odyssey::Sprite2D* skill2 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_2.png", 52, 45);
	// Wind Slash hover
	setupSkillHover(canvas2, L"Paladin", L"Judgement", L"assets/images/Paladin_Skill_2.png", L"15", L"Attack", L"1", L"200 dmg",
		L"Description: Smite the enemy with holy light dealing 200 damage and healing the paladin for 15 health. Costs 15 mana.");
	// Wind Slash trigger
	hover->registerSprite(skill2, canvas2);

	// Increment the icon
	xAnchor += 56.5f;

	// Firestorm icon
	Odyssey::Sprite2D* skill3 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_3.png", 52, 45);
	// Firestorm hover
	setupSkillHover(canvas3, L"Paladin", L"Shield of Light", L"assets/images/Paladin_Skill_3.png", L"20", L"Support", L"4", L"+25 shield",
		L"Description: A shield of light slams down in front of all team members granting a 25 health shield for 3 turns. Costs 20 mana.");
	// Firestorm trigger
	hover->registerSprite(skill3, canvas3);

	// Increment the icon
	xAnchor += 56.5f;

	// Lightning Bolt icon
	Odyssey::Sprite2D* skill4 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_4.png", 52, 45);
	// Lightning Bolt hover
	setupSkillHover(canvas4, L"Paladin", L"Blessing of Light", L"assets/images/Paladin_Skill_4.png", L"15", L"Support", L"4", L"+50% def",
		L"Description: Protects all allies from harm granting them 50% reduced damage for 3 turns. Costs 15 mana.");
	// Lightning Bolt trigger
	hover->registerSprite(skill4, canvas4);
}

void setupMageSkills(std::shared_ptr<Odyssey::Entity> character, float xAnchor, float yAnchor)
{
	// TODO: REFACTOR THIS LATER
	// Add skill icons to portrait
	Odyssey::UICanvas* canvas1 = character->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas2 = character->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas3 = character->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas4 = character->addComponent<Odyssey::UICanvas>();
	SkillHoverComponent* hover = character->addComponent<SkillHoverComponent>();

	// Basic Attack icon
	Odyssey::Sprite2D* skill1 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Mage_Skill_1.png", 52, 45);
	// Basic Attack hover
	setupSkillHover(canvas1, L"Mage", L"Basic Attack", L"assets/images/Mage_Skill_1.png", L"0", L"Attack", L"1", L"10 dmg",
		L"Description: Send forth an orb of chaotic magic inflicting 10 damage with a 15% chance to stun. Refunds 10 mana.");
	// Basic Attack trigger
	hover->registerSprite(skill1, canvas1);

	// Increment the icon
	xAnchor += 56.5f;

	// Wind Slash icon
	Odyssey::Sprite2D* skill2 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Mage_Skill_2.png", 52, 45);
	// Wind Slash hover
	setupSkillHover(canvas2, L"Mage", L"Wind Slash", L"assets/images/Mage_Skill_2.png", L"10", L"Attack", L"4", L"15 x 4 dmg",
		L"Description: Slash all enemies with a burst of wind dealing 15 damage per hit with a 100% chance to inflict speed down. Costs 10 mana.");
	// Wind Slash trigger
	hover->registerSprite(skill2, canvas2);

	// Increment the icon
	xAnchor += 56.5f;

	// Firestorm icon
	Odyssey::Sprite2D* skill3 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Mage_Skill_3.png", 52, 45);
	// Firestorm hover
	setupSkillHover(canvas3, L"Mage", L"Firestorm", L"assets/images/Mage_Skill_3.png", L"30", L"Attack", L"4", L"50 x 4 dmg",
		L"Description: Conjure a hellstorm dealing 50 damage to all enemies and inflicting burn with a 100% chance. Costs 30 mana.");
	// Firestorm trigger
	hover->registerSprite(skill3, canvas3);

	// Increment the icon
	xAnchor += 56.5f;

	// Lightning Bolt icon
	Odyssey::Sprite2D* skill4 = character->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Mage_Skill_4.png", 52, 45);
	// Lightning Bolt hover
	setupSkillHover(canvas4, L"Mage", L"Lightning Bolt", L"assets/images/Mage_Skill_4.png", L"35", L"Attack", L"1", L"60 dmg",
		L"Description: Channel a bolt of lightning dealing 60 damage to a single enemy with a 100% chance to stun. Costs 35 mana.");
	// Lightning Bolt trigger
	hover->registerSprite(skill4, canvas4);
}

void setUpTowerManager()
{
	// Create the current tower entity
	gCurrentTower = std::make_shared<Odyssey::Entity>();
	gCurrentTower->addComponent<TowerManager>();
	gCurrentTower->getComponent<TowerManager>()->UI = gGameMenu->getComponents<Odyssey::UICanvas>()[0];
	gCurrentTower->getComponent<TowerManager>()->Rewards = gGameMenu->getComponents<Odyssey::UICanvas>()[1];
	
	// Create Character Factory
	std::shared_ptr<CharacterFactory> charFactory = std::make_shared<CharacterFactory>();
	charFactory->mRenderRefrence = gRenderDevice;
	std::shared_ptr<Odyssey::Entity> characterToAdd;

	// Get Canvas
	Odyssey::UICanvas* canvas = gGameMenu->getComponents<Odyssey::UICanvas>()[0];
	// Get the width and height of the window
	UINT width = gMainWindow->getWidth();
	UINT height = gMainWindow->getHeight();
	float heroUIYPosition = static_cast<float>(height) - 120.0f;

	// Paladin #1
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(6.0f, 0.3f, 4.5f, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Paladin, "Paladin Uno", charPosition, charRotation, gGameScene);
	// Create the character's portrait
	GameUIManager::getInstance().CreateCharacterPortrait(10.0f, heroUIYPosition, L"assets/images/PaladinPortrait.jpg", gGameMenu, characterToAdd->getComponent<Character>());

	// TODO: REFACTOR THIS LATER
	// Setup Paladin skills
	setupPaladinSkills(characterToAdd, 144.0f, 624.0f);

	// Added the Character's health popup
	createCharacterHealthPopup(150, 500, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gPlayerUnit.push_back(characterToAdd);

	// Paladin #2
	charPosition = DirectX::XMVectorSet(2.0f, -0.6f, 4.5f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Paladin, "Paladin Dos", charPosition, charRotation, gGameScene);
	// Create the character's portrait
	GameUIManager::getInstance().CreateCharacterPortrait((static_cast<float>(width) / 2.0f) - 170.0f, heroUIYPosition, L"assets/images/PaladinPortrait.jpg", gGameMenu, characterToAdd->getComponent<Character>());
	
	// TODO: REFACTOR THIS LATER
	// Setup Paladin skills
	setupPaladinSkills(characterToAdd, 604.0f, 624.0f);

	// Added the Character's health popup
	createCharacterHealthPopup(475, 550, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gPlayerUnit.push_back(characterToAdd);

	// Mage #1
	charPosition = DirectX::XMVectorSet(-2.0f, -0.6f, 4.5f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Mage, "Mage Uno", charPosition, charRotation, gGameScene);
	// Create the character's portrait
	GameUIManager::getInstance().CreateCharacterPortrait((static_cast<float>(width) - 370.0f), heroUIYPosition, L"assets/images/MagePortrait.jpg", gGameMenu, characterToAdd->getComponent<Character>());
	
	// TODO: REFACTOR THIS LATER
	// Setup Mage skills
	setupMageSkills(characterToAdd, 1044.0f, 624.0f);

	// Added the Character's health popup
	createCharacterHealthPopup(850, 550, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gPlayerUnit.push_back(characterToAdd);

	// Skeleton #1
	float enemyUIXPosition = 10.0f;
	float enemyUIYPosition = 10.0f;
	charPosition = DirectX::XMVectorSet(7.5f, 0.3f, -5.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, "Skeleton Un", charPosition, charRotation, gGameScene);
	//createCharacterPortrait(275, 200, canvas, characterToAdd->getComponent<Character>());
	// Create the character's portrait
	GameUIManager::getInstance().CreateCharacterPortrait(enemyUIXPosition, enemyUIYPosition, L"assets/images/SkeletonIcon.png", gGameMenu, characterToAdd->getComponent<Character>());
	// Added the Character's health popup
	createCharacterHealthPopup(300, 250, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gEnemyUnit.push_back(characterToAdd);

	// Skeleton #2
	charPosition = DirectX::XMVectorSet(3.0f, -0.6f, -5.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, "Skeleton Deux", charPosition, charRotation, gGameScene);
	//createCharacterPortrait(475, 225, canvas, characterToAdd->getComponent<Character>());
	// Create the character's portrait
	enemyUIXPosition += 329.7f;
	GameUIManager::getInstance().CreateCharacterPortrait(enemyUIXPosition, enemyUIYPosition, L"assets/images/SkeletonIcon.png", gGameMenu, characterToAdd->getComponent<Character>());
	// Added the Character's health popup
	createCharacterHealthPopup(500, 275, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gEnemyUnit.push_back(characterToAdd);

	// Skeleton #3
	charPosition = DirectX::XMVectorSet(-3.0f, -0.6f, -5.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, "Skeleton Trois", charPosition, charRotation, gGameScene);
	//createCharacterPortrait(700, 225, canvas, characterToAdd->getComponent<Character>());
	// Create the character's portrait
	enemyUIXPosition += 329.7f;
	GameUIManager::getInstance().CreateCharacterPortrait(enemyUIXPosition, enemyUIYPosition, L"assets/images/SkeletonIcon.png", gGameMenu, characterToAdd->getComponent<Character>());
	// Added the Character's health popup
	createCharacterHealthPopup(700, 275, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gEnemyUnit.push_back(characterToAdd);

	// Skeleton #4
	charPosition = DirectX::XMVectorSet(-7.5f, 0.3f, -5.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, "Skeleton Quatre", charPosition, charRotation, gGameScene);
	//createCharacterPortrait(875, 200, canvas, characterToAdd->getComponent<Character>());
	// Create the character's portrait
	enemyUIXPosition += 329.7f;
	GameUIManager::getInstance().CreateCharacterPortrait(enemyUIXPosition, enemyUIYPosition, L"assets/images/SkeletonIcon.png", gGameMenu, characterToAdd->getComponent<Character>());
	// Added the Character's health popup
	createCharacterHealthPopup(900, 250, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	gEnemyUnit.push_back(characterToAdd);

	// Ganfaul
	charPosition = DirectX::XMVectorSet(0.0f, 0.3f, -5.0f, 1.0f);
	characterToAdd = charFactory->CreateCharacter(CharacterFactory::CharacterOptions::Ganfaul, "Ganfaul", charPosition, charRotation, gGameScene);
	enemyUIXPosition = static_cast<float>(width) / 2.0f;
	GameUIManager::getInstance().CreateCharacterPortrait(enemyUIXPosition, enemyUIYPosition, L"assets/images/GanfaulIcon.jpg", gGameMenu, characterToAdd->getComponent<Character>());
	// Added the Character's health popup
	createCharacterHealthPopup(575.0f, height/2, canvas, characterToAdd->getComponent<Character>());
	gGameScene->addEntity(characterToAdd);
	characterToAdd->setActive(false);
	// Assign the boss character for the tower
	gCurrentTower->getComponent<TowerManager>()->SetBossCharacter(characterToAdd);
	
	// Create the turn indicator circle
	std::shared_ptr<Odyssey::Entity> turnIndicatorModel = std::make_shared<Odyssey::Entity>();
	turnIndicatorModel->addComponent<Odyssey::Transform>();
	turnIndicatorModel->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	turnIndicatorModel->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(turnIndicatorModel, "assets/models/TurnIndicator.dxm", false);
	DirectX::XMFLOAT4 turnIndicatorColor = { 0.0f, 0.0f, 255.0f, 1.0f };
	turnIndicatorModel->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor(turnIndicatorColor);
	turnIndicatorModel->setStatic(false);
	gGameScene->addEntity(turnIndicatorModel);

	// Set up the tower manager with the enemy and player teams
	gCurrentTower->getComponent<TowerManager>()->SetUpTowerManager(gPlayerUnit, gEnemyUnit, 2, turnIndicatorModel);
	gGameScene->addEntity(gCurrentTower);
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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SetUnhandledExceptionFilter(DumpOutput);
	//StatTracker::Instance().OutputStatSheet();
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