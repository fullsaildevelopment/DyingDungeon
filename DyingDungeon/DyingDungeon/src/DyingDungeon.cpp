#include "DyingDungeon.h"

// General includes
#include <stdio.h>
#include <DbgHelp.h>
#include <time.h>

//Game UI Manager
#include "GameUIManager.h"
#include "TeamManager.h"

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
#include "SaveLoad.h"

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
	Odyssey::RenderWindow* gMainWindow;
	std::shared_ptr<Odyssey::RenderTarget> gRenderTarget;
	// Scene resources
	Odyssey::Scene* gMainMenu;
	Odyssey::Scene* gSceneOne;
	Odyssey::Scene* gSceneTwo;
	Odyssey::Scene* gTowerSelectScene;
	Odyssey::Scene* gTeamSelectScene;
	Odyssey::Entity* gMainCamera;
	//Game Objects
	Odyssey::Entity* gMenu;
	Odyssey::Entity* gTowerSelectMenu;
	Odyssey::Entity* gTeamSelectMenu;
	Odyssey::Entity* gRewardsScreen;
	Odyssey::Entity* gPaladin;
	Odyssey::Entity* gSkeleton;
	Odyssey::Entity* gCurrentTower;
	Odyssey::Entity* gTurnIndicatorModel;
	//Vectors
	std::vector<Odyssey::Entity*> gPlayerUnit;
	std::vector<Odyssey::Entity*> gEnemyUnit;
	// Light resources
	Odyssey::Entity* gScene1Lights[24];
	Odyssey::Entity* gScene2Lights[24];
	Odyssey::Entity* gMenuLights[24];
	Odyssey::TextProperties gDefaultText;
	// Particle systems
	Odyssey::Entity* gFireBall;
	Odyssey::Entity* gFireStorm;
}

// Forward declarations
int playGame();
void setupSceneOne();
void setupSceneTwo();
void setupMenu(Odyssey::Application* application, Odyssey::Scene*& _sceneObject, Odyssey::Entity*& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent);
void setupMainMenu(Odyssey::Application* application);
void setupTeamSelectMenu(Odyssey::Application* application);
void setupRewardsPrefab(Odyssey::Application* application);
void setupTowerManager();
void setupEnemiesToCreate();
void setupAudio();
LONG WINAPI DumpOutput(struct _EXCEPTION_POINTERS* in_error);

// Functions
int playGame()
{
	// TODO: BREAKPOINT FOR YOUR DUMBASS MEMORY LEAKS
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2034525);
#endif // _DEBUG
	// Seed random using time
	srand((unsigned int)time(NULL));
	
	// Set up the application and create a render window
	std::shared_ptr<Odyssey::Application> application = std::make_shared<Odyssey::Application>();
	gMainWindow = application->createRenderWindow(L"Dying Dungeon", 1280, 720);

	gDefaultText.bold = false;
	gDefaultText.italic = false;
	gDefaultText.fontSize = 25.0f;
	gDefaultText.textAlignment = Odyssey::TextAlignment::Left;
	gDefaultText.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	gDefaultText.fontName = L"Constantia";

	// Setup the character and other model prefabs
	CharacterFactory::getInstance().initialize(application.get());
	// Assign the width and height for the UI Manager
	GameUIManager::getInstance().SetScreenWidthAndHeight(gMainWindow->getWidth(), gMainWindow->getHeight());
	// Setup the UI prefabs
	GameUIManager::getInstance().initialize(application.get());

	// Set up the main menu
	setupMainMenu(application.get());
	// Create the menu canvas
	GameUIManager::getInstance().CreateStatsMenuCanvas(gMainMenu);
	// TODO: M3B1 ONLY REFACTOR LATER
	GameUIManager::getInstance().CreateCreditsMenuCanvas(gMainMenu);
	// TODO: M3B1 ONLY END

	// Set up the tower selection screen
	setupMenu(application.get(), gTowerSelectScene, gTowerSelectMenu, L"assets/images/TowerSelectionBackground.png", "TowerSelection", MenuComponent::eTowerSelector);
	// Create the tower selection menu
	GameUIManager::getInstance().CreateTowerSelectMenuCanvas(gTowerSelectScene);

	// Set up the team selection screen
	setupTeamSelectMenu(application.get());

	// Create Scene One
	gSceneOne = application->createScene("Scene One", DirectX::XMFLOAT3(0.0f, 0.0f, 25.0f), 45.0f);
	gSceneOne->setSkybox("Dusk.dds");
	setupSceneOne();
	// Set up the rewards screen prefab
	setupRewardsPrefab(application.get());
	// Set up the tower manager
	setupTowerManager();
	// Set up the enemies that the player will battle
	setupEnemiesToCreate();
	//TeamManager::getInstance().initialize();
	// Create the battle log for the game
	GameUIManager::getInstance().CreateBattleLog(gSceneOne);
	// Create Pause Menu
	GameUIManager::getInstance().CreatePauseMenuCanvas(gSceneOne);

	// Create Scene Two
	gSceneTwo = application->createScene("Scene Two", DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 50.0f);
	gSceneTwo->setSkybox("Skybox.dds");
	setupSceneTwo();
	// Set the game's current tower
	gTeamSelectMenu->getComponent<TeamSelectionController>()->SetTowerManager(gCurrentTower);
	
	// Set up multithreading
	application->setMultithreading(false);

	// Play audio
	setupAudio();

	// Run the application
	return application->run();
}

void setupMenu(Odyssey::Application* application, Odyssey::Scene*& _sceneObject, Odyssey::Entity*& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent)
{
	_sceneObject = application->createScene(_menuName, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 20.0f);
	_entityToAdd = _sceneObject->createEntity();
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
		break;
	}
	default:
		break;
	}
}

void setupMainMenu(Odyssey::Application* application)
{
	// Create Menu
	setupMenu(application, gMainMenu, gMenu, L"", "MainMenu", MenuComponent::eMainMenu);

	// Set up a directional light
	gMenuLights[0] = gMainMenu->createEntity();
	gMenuLights[0]->addComponent<Odyssey::Transform>();
	gMenuLights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gMenuLights[0]->getComponent<Odyssey::Transform>()->setRotation(25.0f, 100.0f, 0.0f);
	Odyssey::Light* light = gMenuLights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.4f, 0.5f, 0.7f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	gMenuLights[1] = gMainMenu->createEntity();
	gMenuLights[1]->addComponent<Odyssey::Transform>();
	gMenuLights[1]->getComponent<Odyssey::Transform>()->setPosition(0.0, 10.0f, 0.0f);
	light = gMenuLights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.5f, 0.5f);
	light->setIntensity(5.0f);
	light->setRange(30.0f);
	light->setSpotAngle(0.0f);

	// Create the UI
	GameUIManager::getInstance().CreateMainMenuCanvas(gMainMenu);
}

void setupTeamSelectMenu(Odyssey::Application* application)
{
	// Set up the team selection screen
	setupMenu(application, gTeamSelectScene, gTeamSelectMenu, L"", "TeamSelection", MenuComponent::eTeamSelector);

	// Get the team selection controller
	TeamSelectionController* teamSelectionController = gTeamSelectMenu->getComponent<TeamSelectionController>();

	// Set up a directional light
	gMenuLights[2] = gTeamSelectScene->createEntity();
	gMenuLights[2]->addComponent<Odyssey::Transform>();
	gMenuLights[2]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gMenuLights[2]->getComponent<Odyssey::Transform>()->setRotation(25.0f, 100.0f, 0.0f);
	Odyssey::Light* light = gMenuLights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.4f, 0.5f, 0.7f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	gMenuLights[3] = gTeamSelectScene->createEntity();
	gMenuLights[3]->addComponent<Odyssey::Transform>();
	gMenuLights[3]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 2.0f);
	light = gMenuLights[3]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.5f, 0.5f);
	light->setIntensity(1.0f);
	light->setRange(30.0f);
	light->setSpotAngle(0.0f);

	// Create the UI for the team selection
	GameUIManager::getInstance().CreateTeamSelectMenuCanvas(gTeamSelectScene);
}

void setupRewardsPrefab(Odyssey::Application* application)
{
	// TODO: spawn prefab in a scene
	// Create the rewards prefab
	gRewardsScreen = gSceneOne->createEntity();

	// Add a UI Canvas for the rewards screen
	gRewardsScreen->addComponent<Odyssey::UICanvas>(); // The rewards screen

	// Get the width and height of the window
	UINT width = gMainWindow->getWidth();
	UINT height = gMainWindow->getHeight();

	// Get a reference to the UI canvas and get the width/height of the screen
	Odyssey::UICanvas* canvas = gRewardsScreen->getComponent<Odyssey::UICanvas>();
	// Results Menu
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height)->setOpacity(0.5f);
	UINT rewardsImageWidth = width * 0.6f;
	UINT rewardsImageHeight = height * 0.6f;
	float rewardsImageX = (width / 2.0f) - (static_cast<float>(rewardsImageWidth) / 2.0f);
	float rewardsImageY = (height / 2.0f) - (static_cast<float>(rewardsImageHeight) / 2.0f);

	Odyssey::TextProperties rewardsTextProperties = gDefaultText;
	rewardsTextProperties.fontSize = 30.0f;
	rewardsTextProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;

	//Stat Background
	//protrait backgrounds
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 135.0f + (rewardsImageHeight / 8) - 10.0f, rewardsImageY - 90.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 100.0f + (rewardsImageHeight / 2) + 55.0f, rewardsImageY - 90.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 165.0f + (rewardsImageHeight) - 40.0f, rewardsImageY - 90.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//
	////attack backgrounds
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY - 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, 40)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//
	////defence backgrounds
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY - 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, 40)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//
	////aid backgrounds
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY - 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, 40)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	//
	////Character Protraits
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 145.0f + (rewardsImageHeight / 8) - 10.0f, rewardsImageY - 80.0f), L"assets/images/Guy.png", rewardsImageHeight /4, rewardsImageHeight /4);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 110.0f + (rewardsImageHeight / 2) + 55.0f, rewardsImageY - 80.0f), L"assets/images/Guy.png", rewardsImageHeight / 4, rewardsImageHeight / 4);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f + (rewardsImageHeight) - 40.0f, rewardsImageY - 80.0f), L"assets/images/Guy.png", rewardsImageHeight / 4, rewardsImageHeight / 4);
	//
	////Stat Text
	//stat discriptors
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f + (rewardsImageHeight / 8) + 10, rewardsImageY - 50.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Attack", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageHeight / 2) + 60, rewardsImageY - 50.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Defense", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + (rewardsImageHeight) + 80, rewardsImageY - 50.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Aid", rewardsTextProperties);

	rewardsTextProperties.fontSize = 16.0f;

	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 4) + 20, 30);

	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);

	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);

	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(15.0f, 15.0f, 15.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30);

	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 40.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Attack", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 75.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Dmg Dealt", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 110.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Dmg Landed", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 145.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Defense", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 180.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Dmg Taken", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 215.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Dmg Block", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 250.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Health Gained", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 285.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Support", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 320.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Health Given", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 355.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Defense Given", rewardsTextProperties);

	rewardsTextProperties.textAlignment = Odyssey::TextAlignment::Center;
	rewardsTextProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;

	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 4) + 30, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);

	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + (rewardsImageHeight / 2) + 110, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);

	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 40.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 75.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 110.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 145.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 180.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 215.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 250.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 285.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"0 times", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 320.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 15.0f + ((rewardsImageHeight / 4) * 3) + 190, rewardsImageY + 15.0f + 355.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 3) + 40, 30, L"00.00", rewardsTextProperties);
	
	rewardsTextProperties.fontSize = 16.0f;
	
	//attack text
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (rewardsImageWidth / 4), rewardsImageY + 7.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Attack: NN.NN%\nDamage Dealt: NN.NN\nDamage Success: NN.NN%", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (rewardsImageWidth / 4), rewardsImageY + (7.0f + rewardsImageHeight *0.3333f) ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Attack: NN.NN%\nDamage Dealt: NN.NN\nDamage Success: NN.NN%", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (rewardsImageWidth / 4), rewardsImageY + (7.0f + rewardsImageHeight * 0.6667f) ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Attack: NN.NN%\nDamage Dealt: NN.NN\nDamage Success: NN.NN%", rewardsTextProperties);
	//defense text
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (2.0f * (rewardsImageWidth / 4)) + 30.0f, rewardsImageY + 7.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Defend: NN.NN%\nDamage Taken: NN.NN\nDamage Blocked: NN.NN%\nHealth Gained: NN.NN", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (2.0f * (rewardsImageWidth / 4)) + 30.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.3333f)), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Defend: NN.NN%\nDamage Taken: NN.NN\nDamage Blocked: NN.NN%\nHealth Gained: NN.NN", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (2.0f * (rewardsImageWidth / 4)) + 30.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.6667f)), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Defend: NN.NN%\nDamage Taken: NN.NN\nDamage Blocked: NN.NN%\nHealth Gained: NN.NN", rewardsTextProperties);
	//aid texts
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (3.0f * (rewardsImageWidth / 4)) + 70.0f, rewardsImageY + 7.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Aid: NN.NN%\nHeal: NN.NN\nDefence Buff: NN.NN", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (3.0f * (rewardsImageWidth / 4)) + 70.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.3333f)), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Aid: NN.NN%\nHeal: NN.NN\nDefence Buff: NN.NN", rewardsTextProperties);
	//canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (3.0f * (rewardsImageWidth / 4)) + 70.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.6667f)), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Aid: NN.NN%\nHeal: NN.NN\nDefence Buff: NN.NN", rewardsTextProperties);

	canvas->setActive(true); // The rewards screen won't show up at the start
	StatTracker::Instance().SetCanvas(canvas, static_cast<unsigned int>(rewardsImageHeight / 4), static_cast<unsigned int>(rewardsImageHeight / 4));
}

void setupTowerManager()
{
	// TODO: Add the tower manger to other scenes as well
	// Create the current tower entity
	gCurrentTower = gSceneOne->createEntity();
	gCurrentTower->addComponent<TowerManager>();
	gCurrentTower->getComponent<TowerManager>()->Rewards = gRewardsScreen->getComponent<Odyssey::UICanvas>();
	// TODO: REFACTOR LATER
	gCurrentTower->getComponent<TowerManager>()->scene = gSceneOne;
}

void setupEnemiesToCreate()
{
	//Set the current tower in the TeamManager 
	TeamManager::getInstance().SetTheCurrentTower(gCurrentTower);
	// Set the first scene in the TeamManager
	TeamManager::getInstance().SetTheFirstScene(gSceneOne);

	// Some varibale to setup the enemies easier
	// Left character
	DirectX::XMVECTOR leftPosition = DirectX::XMVectorSet(-4.5f, 0.0f, 20.0f, 1.0f);
	DirectX::XMVECTOR leftRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 leftHudPosition = { 200.0f, 10.0f };
	DirectX::XMFLOAT2 leftHpPopupPosition = { 425.0f, 150.0f };
	// Middle character
	DirectX::XMVECTOR middlePosition = DirectX::XMVectorSet(0.0f, 0.0f, 20.0f, 1.0f);
	DirectX::XMVECTOR middleRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 middleHudPosition = { 533.0f, 10.0f };
	DirectX::XMFLOAT2 middleHpPopupPosition = { 575.0f, 150.0f };
	// Right character
	DirectX::XMVECTOR rightPosition = DirectX::XMVectorSet(4.5f, 0.0f, 20.0f, 1.0f);
	DirectX::XMVECTOR rightRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 rightHudPosition = { 863.0f, 10.0f };
	DirectX::XMFLOAT2 rightHpPopupPosition = { 725.0f, 150.0f };

	// The list of enemies to add to the list of enemies to create in Team Manager
	std::vector<TeamManager::EnemySetups> newEnemies;

	// LEVEL ONE ENEMIES
	if (true)
	{
		// Level One Enemies
		TeamManager::EnemySetups levelOneEnemy;
		// Set enemy properties
		levelOneEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelOneEnemy.pPosition = leftPosition;
		levelOneEnemy.pRotation = leftRotation;
		// Add enemy to list
		newEnemies.push_back(levelOneEnemy);
		// Set enemy properties
		levelOneEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelOneEnemy.pPosition = middlePosition;
		levelOneEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelOneEnemy);
		// Set enemy properties
		levelOneEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelOneEnemy.pPosition = rightPosition;
		levelOneEnemy.pRotation = rightRotation;
		// Add enemy to list
		newEnemies.push_back(levelOneEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);
		// Clear the new enemy list 
		newEnemies.clear();
	}

	// LEVEL TWO ENEMIES
	if (true)
	{
		// Level Two Enemies
		TeamManager::EnemySetups levelTwoEnemy;
		// Set enemy properties
		levelTwoEnemy.pEnemyType = TeamManager::EnemyType::CasterDemon;
		levelTwoEnemy.pPosition = leftPosition;
		levelTwoEnemy.pRotation = leftRotation;
		// Add enemy to list
		newEnemies.push_back(levelTwoEnemy);
		// Set enemy properties
		levelTwoEnemy.pEnemyType = TeamManager::EnemyType::MeleeDemon;
		levelTwoEnemy.pPosition = middlePosition;
		levelTwoEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelTwoEnemy);
		// Set enemy properties
		levelTwoEnemy.pEnemyType = TeamManager::EnemyType::CasterDemon;
		levelTwoEnemy.pPosition = rightPosition;
		levelTwoEnemy.pRotation = rightRotation;
		// Add enemy to list
		newEnemies.push_back(levelTwoEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);
		// Clear the new enemy list 
		newEnemies.clear();
	}

	// LEVEL THREE ENEMIES
	if (true)
	{
		// Level One Enemies
		TeamManager::EnemySetups levelThreeEnemy;
		// Set enemy properties
		levelThreeEnemy.pEnemyType = TeamManager::EnemyType::Ganfaul;
		levelThreeEnemy.pPosition = middlePosition;
		levelThreeEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelThreeEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);
		// Clear the new enemy list 
		newEnemies.clear();
	}
}

void setupAudio()
{
	//RedAudioManager::Instance();
	//SFX                                |                          Paths                       |  |        Alias        |     |           Audio Type          |        |     Group     |
	// Male Hit reactions / Death
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/grunt1-death-pain.mp3",					"MaleHitReaction",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/grunt2-death-pain.mp3",					"MaleDeath",				RedAudioManager::AudioType::SFX);
	// Female Hit Reaction / Death
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/female_hit_reaction_1.mp3",				"FemaleHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/female_hit_reaction_2.mp3",				"FemaleDeath",				RedAudioManager::AudioType::SFX);

	// Paladin Sound effects
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/armor_hit.mp3",							"PaladinHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/sword_slash.mp3",						"PaladinSwordSwing",			RedAudioManager::AudioType::SFX);

	// Mage Sound Effects
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_swish.mp3",						"MagicMissle",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/small_fireball.mp3",						"FireStorm",			RedAudioManager::AudioType::SFX);

	// Bard Sound Effects
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/arrow_woosh_impact.mp3",					"ShootArrow",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/heal_sound.mp3",							"Heal",					RedAudioManager::AudioType::SFX);

	// Warrior Sound Effects

	// Monk Sound Effects

	// Skeleton Sound Effects
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/bone_punch.mp3",							"SkeletonPunch",			RedAudioManager::AudioType::SFX);

	// Ganfoul sound effects

	// Summoner Sound Effects

	// Caster Demon Sound Effects

	// Melee Demon Sound Efffects

	// Enemy Mage Sound Efffects

	// Music
	RedAudioManager::Instance().AddAudio("assets/audio/Music/battle_music.mp3",						"BackgroundBattle",			RedAudioManager::AudioType::Background);
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/dark_depths.mp3",					"BackgroundMenu1",			RedAudioManager::AudioType::Background,		"BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/menu_music.mp3",						"BackgroundMenu2",			RedAudioManager::AudioType::Background,		"BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/divinum_creaturae.mp3",				"BackgroundMenu3",			RedAudioManager::AudioType::Background,		"BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/losing.mp3",							"Loss",						RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/Music/medieval_parade.mp3",					"Win",						RedAudioManager::AudioType::SFX);

	// Misc Sounds Effects
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/fire_torch_burning.mp3",			"TorchBurning",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/fire_torch_burning_quietly.mp3",	"TorchBurningQuietly",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/fire_torch_burning_wind.mp3",		"TorchBurningWind",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Open.mp3",							"DoorOpen",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Close.mp3",							"DoorClose",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_poof.mp3",							"CharacterEntrance",		RedAudioManager::AudioType::SFX);

	// Memes
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_4.mp3",						"NoManaCritical",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_3.mp3",						"NoManaBitch",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_2.mp3",						"NoManaMidium",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_1.mp3",						"NoManaLow",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/forward_aerial.mp3",						"ForwardAerial",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/gokusjj3.mp3",							"GokuSJJ3",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/windows_death_sound.mp3",				"DeathMeme",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/fbi_rade.mp3",							"FBIRade",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/gta_wanted.mp3",							"GTAWanted",				RedAudioManager::AudioType::SFX);
	// Tower select screen door sounds
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Open.mp3",							"DoorOpen",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Close.mp3",							"DoorClose",				RedAudioManager::AudioType::SFX);
	
	//Background Sound
	RedAudioManager::Instance().AddAudio("assets/audio/Music/battle_music.mp3",						"BackgroundBattle1",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/rage_in_the_darkness.mp3",				"BackgroundBattle2",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/dark_depths.mp3",					"BackgroundMenu1",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/menu_music.mp3",						"BackgroundMenu2",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/divinum_creaturae.mp3",				"BackgroundMenu3",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	// Extras
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/armor_hit.mp3",							"ArrowHit",					RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_energy_burst.mp3",					"ElectricBlast",			RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_zap.mp3",							"MagicZap",					RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/magical_vanish.mp3",						"MagicalVanish",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/medieval_impact_plate_armor.mp3",		"PlateArmorHit",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/large_fireball.mp3",						"LargeFireball",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/slime_sound.mp3",						"PoisonSlime",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/charge_and_fire.mp3",					"ChargeAndFire",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/spell_cast.mp3",							"SpellCasting",				RedAudioManager::AudioType::SFX);
  
	//Play Initial Loop
	//RedAudioManager::Instance().Loop("Death");
	//RedAudioManager::Instance().Stop("BackgroundMenu");
}

void setupSceneOne()
{
	Odyssey::Entity* gSceneOneCamera = gSceneOne->createEntity();
	gSceneOneCamera->addComponent<Odyssey::Transform>();
	gSceneOneCamera->getComponent<Odyssey::Transform>()->setPosition(0.105f, 7.827f, 1.286f);
	gSceneOneCamera->getComponent<Odyssey::Transform>()->setRotation(28.965f, 0.0f, 0.0f);
	gSceneOneCamera->addComponent<Odyssey::Camera>();
	gSceneOneCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gSceneOneCamera->addComponent<CameraController>();

	Odyssey::RenderManager::getInstance().importScene(gSceneOne, "assets/models/SceneOne.dxm");

	// Set the light's position and direction
	gScene1Lights[0] = gSceneOne->createEntity();
	gScene1Lights[0]->addComponent<Odyssey::Transform>();
	gScene1Lights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gScene1Lights[0]->getComponent<Odyssey::Transform>()->setRotation(15.0f, 250.0f, 0.0f);

	// Set up the directional light
	Odyssey::Light* light = gScene1Lights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.65f, 0.3f, 0.15f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	// Set the light's position and direction
	gScene1Lights[1] = gSceneOne->createEntity();
	gScene1Lights[1]->addComponent<Odyssey::Transform>();
	gScene1Lights[1]->getComponent<Odyssey::Transform>()->setPosition(-3.5f, 3.0f, 4.5f);
	gScene1Lights[1]->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);

	// Set up the ambient light
	light = gScene1Lights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.65f, 0.3f, 0.15f);
	light->setIntensity(0.55f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	// Set the light's position and direction
	gScene1Lights[2] = gSceneOne->createEntity();
	gScene1Lights[2]->addComponent<Odyssey::Transform>();
	gScene1Lights[2]->getComponent<Odyssey::Transform>()->setPosition(-3.5f, 4.0f, 25.0f);
	gScene1Lights[2]->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);

	// Set up the directional light
	light = gScene1Lights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.65f, 0.3f, 0.15f);
	light->setIntensity(0.55f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	// Create the fire entity
	Odyssey::Entity* fire1 = gSceneOne->createEntity();

	// Set the transform position and rotation
	fire1->addComponent<Odyssey::Transform>();
	fire1->getComponent<Odyssey::Transform>()->setPosition(-9.0f, 5.571f, 2.075f);
	fire1->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, -15.0f);

	// Set the light's values
	light = fire1->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(0.2f);
	light->setRange(10.0f);
	light->setSpotAngle(0.0f);

	// Create the particle system
	fire1->addComponent<Odyssey::ParticleSystem>();
	fire1->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire1->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.8f, 0.5f, 0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire1->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.0f, 1.50f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire1->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire1->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.0f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSize(0.5f, 0.5f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire1->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.09f, 30.0f, 30.0f));

	// Create the fire entity
	Odyssey::Entity* fire2 = gSceneOne->createEntity();

	// Set the transform position and rotation
	fire2->addComponent<Odyssey::Transform>();
	fire2->getComponent<Odyssey::Transform>()->setPosition(-9.0f, 5.571f, 11.175f);
	fire2->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, -15.0f);

	// Set the light's values
	light = fire2->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(0.2f);
	light->setRange(10.0f);
	light->setSpotAngle(0.0f);

	// Create the particle system
	fire2->addComponent<Odyssey::ParticleSystem>();
	fire2->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire2->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.8f, 0.5f, 0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire2->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.0f, 1.50f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire2->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire2->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.0f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSize(0.5f, 0.5f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire2->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.09f, 30.0f, 30.0f));

	// Create the fire entity
	Odyssey::Entity* fire3 = gSceneOne->createEntity();

	// Set the transform position and rotation
	fire3->addComponent<Odyssey::Transform>();
	fire3->getComponent<Odyssey::Transform>()->setPosition(-4.545f, 5.0f, 40.9f);
	fire3->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);

	// Set the light's values
	light = fire3->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(0.28f);
	light->setRange(15.0f);
	light->setSpotAngle(0.0f);

	// Create the particle system
	fire3->addComponent<Odyssey::ParticleSystem>();
	fire3->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire3->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.8f, 0.5f, 0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire3->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.0f, 1.50f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire3->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire3->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.0f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSize(0.5f, 0.5f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire3->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.09f, 30.0f, 30.0f));

	// Create the fire entity
	Odyssey::Entity* fire4 = gSceneOne->createEntity();

	// Set the transform position and rotation
	fire4->addComponent<Odyssey::Transform>();
	fire4->getComponent<Odyssey::Transform>()->setPosition(-11.075f, 5.0f, 40.9f);
	fire4->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);

	// Set the light's values
	light = fire4->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(0.28f);
	light->setRange(15.0f);
	light->setSpotAngle(0.0f);

	// Create the particle system
	fire4->addComponent<Odyssey::ParticleSystem>();
	fire4->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire4->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.8f, 0.5f, 0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire4->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.0f, 1.50f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire4->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire4->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.0f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSize(0.5f, 0.5f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire4->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.09f, 30.0f, 30.0f));

	Odyssey::Entity* fog = gSceneOne->createEntity();
	fog->addComponent<Odyssey::Transform>();
	fog->getComponent<Odyssey::Transform>()->setPosition(0, 0, 0);
	fog->addComponent<Odyssey::ParticleSystem>();
	fog->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Smoke.jpg");
	fog->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.075f, 0.05f, 0.05f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fog->getComponent<Odyssey::ParticleSystem>()->setLifetime(12.5f, 25.0f);
	fog->getComponent<Odyssey::ParticleSystem>()->setParticleCount(0, 1000);
	fog->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(60);
	fog->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fog->getComponent<Odyssey::ParticleSystem>()->setSpeed(1.0f, 1.25f);
	fog->getComponent<Odyssey::ParticleSystem>()->setSize(20.0f, 20.0f);
	fog->getComponent<Odyssey::ParticleSystem>()->setGravity(4.0f);
	fog->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fog->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::BoxPS(0.0f, -5.5f, 20.0f, 25.0f, 1.0f, 75.0f));
}

void setupSceneTwo()
{
	// LIGHTS
	// Set up a directional light
	gScene1Lights[0] = gSceneTwo->createEntity();
	gScene1Lights[0]->addComponent<Odyssey::Transform>();
	gScene1Lights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gScene1Lights[0]->getComponent<Odyssey::Transform>()->setRotation(25.0f, 100.0f, 0.0f);
	Odyssey::Light* light = gScene1Lights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.4f, 0.5f, 0.7f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	// Arena ambient
	gScene1Lights[1] = gSceneTwo->createEntity();
	gScene1Lights[1]->addComponent<Odyssey::Transform>();
	gScene1Lights[1]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 10.0f, 0.0f);
	light = gScene1Lights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.5f, 0.5f);
	light->setIntensity(2.0f);
	light->setRange(50.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 1
	gScene1Lights[2] = gSceneTwo->createEntity();
	gScene1Lights[2]->addComponent<Odyssey::Transform>();
	gScene1Lights[2]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, 14.4f);
	light = gScene1Lights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 2
	gScene1Lights[3] = gSceneTwo->createEntity();
	gScene1Lights[3]->addComponent<Odyssey::Transform>();
	gScene1Lights[3]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, 7.5f);
	light = gScene1Lights[3]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 3
	gScene1Lights[4] = gSceneTwo->createEntity();
	gScene1Lights[4]->addComponent<Odyssey::Transform>();
	gScene1Lights[4]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, -6.22f);
	light = gScene1Lights[4]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 4
	gScene1Lights[5] = gSceneTwo->createEntity();
	gScene1Lights[5]->addComponent<Odyssey::Transform>();
	gScene1Lights[5]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, -13.22f);
	light = gScene1Lights[5]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Pillar 1
	gScene1Lights[6] = gSceneTwo->createEntity();
	gScene1Lights[6]->addComponent<Odyssey::Transform>();
	gScene1Lights[6]->getComponent<Odyssey::Transform>()->setPosition(5.45f, 4.75f, 14.4f);
	light = gScene1Lights[6]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Pillar 2
	gScene1Lights[7] = gSceneTwo->createEntity();
	gScene1Lights[7]->addComponent<Odyssey::Transform>();
	gScene1Lights[7]->getComponent<Odyssey::Transform>()->setPosition(5.45f, 4.75f, 7.5f);
	light = gScene1Lights[7]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Pillar 3
	gScene1Lights[8] = gSceneTwo->createEntity();
	gScene1Lights[8]->addComponent<Odyssey::Transform>();
	gScene1Lights[8]->getComponent<Odyssey::Transform>()->setPosition(5.45f, 4.75f, -13.22f);
	light = gScene1Lights[8]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Door Light 1
	gScene1Lights[9] = gSceneTwo->createEntity();
	gScene1Lights[9]->addComponent<Odyssey::Transform>();
	gScene1Lights[9]->getComponent<Odyssey::Transform>()->setPosition(-12.0f, 4.75f, -6.7f);
	light = gScene1Lights[9]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Door Light 2
	gScene1Lights[10] = gSceneTwo->createEntity();
	gScene1Lights[10]->addComponent<Odyssey::Transform>();
	gScene1Lights[10]->getComponent<Odyssey::Transform>()->setPosition(-12.0f, 4.75f, 1.2f);
	light = gScene1Lights[10]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Door Light 1
	gScene1Lights[11] = gSceneTwo->createEntity();
	gScene1Lights[11]->addComponent<Odyssey::Transform>();
	gScene1Lights[11]->getComponent<Odyssey::Transform>()->setPosition(12.74f, 5.0f, -2.85f);
	light = gScene1Lights[11]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Door Light 2
	gScene1Lights[12] = gSceneTwo->createEntity();
	gScene1Lights[12]->addComponent<Odyssey::Transform>();
	gScene1Lights[12]->getComponent<Odyssey::Transform>()->setPosition(12.74f, 5.0f, 4.25f);
	light = gScene1Lights[12]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// Library-Area Candle Light
	gScene1Lights[13] = gSceneTwo->createEntity();
	gScene1Lights[13]->addComponent<Odyssey::Transform>();
	gScene1Lights[13]->getComponent<Odyssey::Transform>()->setPosition(-1.25f, 12.5f, -35.0f);
	light = gScene1Lights[13]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(12.5f);
	light->setSpotAngle(0.0f);

	//CAMERA
	gMainCamera = gSceneTwo->createEntity();
	gMainCamera->addComponent<Odyssey::Transform>();
	gMainCamera->getComponent<Odyssey::Transform>()->setPosition(0.395f, 6.703f, 13.438f);
	gMainCamera->getComponent<Odyssey::Transform>()->setRotation(23.669f, -178.152f, 0.0f);
	gMainCamera->addComponent<Odyssey::Camera>();
	gMainCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gMainCamera->addComponent<CameraController>();

	// SCENE
	//Odyssey::RenderManager::getInstance().importScene(gSceneTwo, "assets/models/TestArena.dxm");

	// FIRE PARTICLE EFFECTS
	Odyssey::Entity* fire1 = gSceneTwo->createEntity();
	fire1->addComponent<Odyssey::Transform>();
	fire1->getComponent<Odyssey::Transform>()->setPosition(-5.65f, 4.67f, -6.42f);
	fire1->addComponent<Odyssey::ParticleSystem>();
	fire1->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire1->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire1->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire1->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire1->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire1->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire2 = gSceneTwo->createEntity();
	fire2->addComponent<Odyssey::Transform>();
	fire2->getComponent<Odyssey::Transform>()->setPosition(-5.65f, 4.67f, -13.42f);
	fire2->addComponent<Odyssey::ParticleSystem>();
	fire2->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire2->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire2->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire2->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire2->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire2->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire3 = gSceneTwo->createEntity();
	fire3->addComponent<Odyssey::Transform>();
	fire3->getComponent<Odyssey::Transform>()->setPosition(5.65f, 4.67f, -13.42f);
	fire3->addComponent<Odyssey::ParticleSystem>();
	fire3->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire3->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire3->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire3->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire3->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire3->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire4 = gSceneTwo->createEntity();
	fire4->addComponent<Odyssey::Transform>();
	fire4->getComponent<Odyssey::Transform>()->setPosition(-12.6f, 4.17f, -6.42f);
	fire4->addComponent<Odyssey::ParticleSystem>();
	fire4->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire4->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire4->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire4->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire4->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire4->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire5 = gSceneTwo->createEntity();
	fire5->addComponent<Odyssey::Transform>();
	fire5->getComponent<Odyssey::Transform>()->setPosition(12.65f, 4.67f, -2.92f);
	fire5->addComponent<Odyssey::ParticleSystem>();
	fire5->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire5->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire5->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire5->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire5->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire5->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire5->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire5->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire5->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire5->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire6 = gSceneTwo->createEntity();
	fire6->addComponent<Odyssey::Transform>();
	fire6->getComponent<Odyssey::Transform>()->setPosition(5.59f, 4.67f, 7.59f);
	fire6->addComponent<Odyssey::ParticleSystem>();
	fire6->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire6->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire6->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire6->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire6->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire6->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire6->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire6->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire6->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire6->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire7 = gSceneTwo->createEntity();
	fire7->addComponent<Odyssey::Transform>();
	fire7->getComponent<Odyssey::Transform>()->setPosition(5.59f, 4.67f, 14.62f);
	fire7->addComponent<Odyssey::ParticleSystem>();
	fire7->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire7->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire7->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire7->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire7->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire7->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire7->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire7->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire7->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire7->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire8 = gSceneTwo->createEntity();
	fire8->addComponent<Odyssey::Transform>();
	fire8->getComponent<Odyssey::Transform>()->setPosition(-5.74f, 4.67f, 7.53f);
	fire8->addComponent<Odyssey::ParticleSystem>();
	fire8->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire8->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire8->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire8->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire8->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire8->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire8->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire8->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire8->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire8->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire9 = gSceneTwo->createEntity();
	fire9->addComponent<Odyssey::Transform>();
	fire9->getComponent<Odyssey::Transform>()->setPosition(-5.78f, 4.67f, 14.62f);
	fire9->addComponent<Odyssey::ParticleSystem>();
	fire9->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire9->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire9->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire9->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire9->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire9->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire9->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire9->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire9->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire9->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire10 = gSceneTwo->createEntity();
	fire10->addComponent<Odyssey::Transform>();
	fire10->getComponent<Odyssey::Transform>()->setPosition(12.66f, 4.7f, 4.12f);
	fire10->addComponent<Odyssey::ParticleSystem>();
	fire10->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire10->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire10->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire10->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire10->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire10->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire10->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire10->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire10->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire10->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	Odyssey::Entity* fire11 = gSceneTwo->createEntity();
	fire11->addComponent<Odyssey::Transform>();
	fire11->getComponent<Odyssey::Transform>()->setPosition(-12.66f, 4.25f, 1.53f);
	fire11->addComponent<Odyssey::ParticleSystem>();
	fire11->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire11->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire11->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire11->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire11->getComponent<Odyssey::ParticleSystem>()->setEmissionOverLifetime(90);
	fire11->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire11->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire11->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire11->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire11->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));
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
	SaveLoad::Instance().CreateProfileDirectory();
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