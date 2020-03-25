#include "DyingDungeon.h"

// General includes
#include <stdio.h>
#include <DbgHelp.h>
#include <time.h>

//Game UI Manager
#include "GameUIManager.h"
#include "TeamManager.h"
#include "TowerSelectionPrefabFactory.h"

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
#include "LoadingScreenController.h"
#include "SkillShowcase.h"
#include "PurifyMover.h"
#include "SpinKickMover.h"
#include "PunchMover.h"
#include "BossAttackMover.h"
#include "JudgementMover.h"

// Engine includes
#include "OdysseyEngine.h"
#pragma comment(lib, "dbghelp.lib")
#include "Material.h"

// Memory leak includes
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "CasterMover.h"
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
	Odyssey::Scene* gLoadingScene;
	Odyssey::Scene* gSceneOne;
	Odyssey::Scene* gSceneBoss;
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
	Odyssey::Entity* gSceneOneTower;
	Odyssey::Entity* gSceneBossTower;
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
void setupLoadingScene(Odyssey::Application* application);
void setupSkillVFX(Odyssey::Application* application);
void setupBardVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupPaladinVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupMageVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupWarriorVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupMonkVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupGanfaulVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupCasterVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupSkeletonVFX(Odyssey::Application* application, Odyssey::Entity* showcase);
void setupMenu(Odyssey::Application* application, Odyssey::Scene*& _sceneObject, Odyssey::Entity*& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent);
void setupMainMenu(Odyssey::Application* application);
void setupTeamSelectMenu(Odyssey::Application* application);
void setupRewardsPrefab(Odyssey::Application* application);
void setupTowerManager(Odyssey::Scene* _sceneToAddTo, int _sceneNum);
void setupEnemiesToCreate(Odyssey::Application* application);
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
	setupLoadingScene(application.get());
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
	setupTowerManager(gSceneOne, 1);
	// Add tower manager for the tower select screen
	gTowerSelectMenu->getComponent<TowerSelectController>()->AddTowerManager(gSceneOneTower);
	// Add the game's towers
	gTeamSelectMenu->getComponent<TeamSelectionController>()->AddTowerManager(gSceneOneTower);
	// Set up the enemies that the player will battle
	setupEnemiesToCreate(application.get());
	//TeamManager::getInstance().initialize();
	// Create the battle log for the game
	GameUIManager::getInstance().CreateBattleLog(gSceneOne);

	// Create Tutorial UI
	//GameUIManager::getInstance().CreateTutorialCanvas(gSceneOne);

	// Create Scene Two
	gSceneBoss = application->createScene("Boss Scene", DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 50.0f);
	gSceneBoss->setSkybox("Skybox.dds");
	setupSceneTwo();
	// Set up the tower manager
	setupTowerManager(gSceneBoss, 2);
	// Add tower manager for the tower select screen
	gTowerSelectMenu->getComponent<TowerSelectController>()->AddTowerManager(gSceneBossTower);
	// Add the game's towers
	gTeamSelectMenu->getComponent<TeamSelectionController>()->AddTowerManager(gSceneBossTower);

	// Assign the team slection controller to the tower selector
	gTowerSelectMenu->getComponent<TowerSelectController>()->SetTeamSelector(gTeamSelectMenu->getComponent<TeamSelectionController>());

	// Create Pause Menus for the scenes
	std::vector<Odyssey::Scene*> gameScenes;
	gameScenes.push_back(gSceneOne);
	gameScenes.push_back(gSceneBoss);
	GameUIManager::getInstance().CreatePauseMenuCanvas(gameScenes);
	
	// Set up multithreading
	application->setMultithreading(false);

	// Play audio
	setupAudio();

	// BUILD REVIEW ONLY
	setupSkillVFX(application.get());

	// SET THE CURSOR
	Odyssey::EventManager::getInstance().publish(new Odyssey::ChangeMouseCursorEvent(L"assets/images/Cursor/Cursor_Basic.cur"));

	// Run the application
	return application->run();
}

void setupMenu(Odyssey::Application* application, Odyssey::Scene*& _sceneObject, Odyssey::Entity*& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent)
{
	_sceneObject = application->createScene(_menuName, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 50.0f);
	_sceneObject->setSkybox("Skybox.dds");
	Odyssey::RenderManager::getInstance().importScene(_sceneObject, "assets/models/BossScene.dxm");
	_entityToAdd = _sceneObject->createEntity();
	_entityToAdd->addComponent<Odyssey::Transform>();
	_entityToAdd->addComponent<Odyssey::UICanvas>();
	_entityToAdd->addComponent<Odyssey::Camera>();
	_entityToAdd->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	_entityToAdd->getComponent<Odyssey::Transform>()->setPosition(44.85f, 5.34f, -0.88f);
	_entityToAdd->getComponent<Odyssey::Transform>()->setRotation(3.7f, -88.0f, 0.0f);
	_entityToAdd->addComponent<CameraController>();
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

	// Set the light's position and direction
	gMenuLights[0] = gMainMenu->createEntity();
	gMenuLights[0]->addComponent<Odyssey::Transform>();
	gMenuLights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gMenuLights[0]->getComponent<Odyssey::Transform>()->setRotation(15.0f, 250.0f, 0.0f);

	// Set up the directional light
	Odyssey::Light* light = gMenuLights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(0.5f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	// Set the light's position and direction
	Odyssey::Entity* eLight = gMainMenu->createEntity();
	eLight->addComponent<Odyssey::Transform>();
	eLight->getComponent<Odyssey::Transform>()->setPosition(36.75f, 10.0f, 1.88f);
	eLight->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);

	// Set up the spot light
	light = eLight->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Spot);
	light->setColor(0.5f, 0.4f, 0.3f);
	light->setIntensity(1.0f);
	light->setRange(30.0f);
	light->setSpotAngle(0.4f);

	// Create the UI
	GameUIManager::getInstance().CreateMainMenuCanvas(gMainMenu);
}

void setupTeamSelectMenu(Odyssey::Application* application)
{
	// Set up the team selection screen
	setupMenu(application, gTeamSelectScene, gTeamSelectMenu, L"", "TeamSelection", MenuComponent::eTeamSelector);

	// Get the team selection controller
	TeamSelectionController* teamSelectionController = gTeamSelectMenu->getComponent<TeamSelectionController>();

	// Set the light's position and direction
	gMenuLights[2] = gTeamSelectScene->createEntity();
	gMenuLights[2]->addComponent<Odyssey::Transform>();
	gMenuLights[2]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gMenuLights[2]->getComponent<Odyssey::Transform>()->setRotation(15.0f, 250.0f, 0.0f);

	// Set up the directional light
	Odyssey::Light* light = gMenuLights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(0.5f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	gMenuLights[3] = gTeamSelectScene->createEntity();
	gMenuLights[3]->addComponent<Odyssey::Transform>();
	gMenuLights[3]->getComponent<Odyssey::Transform>()->setPosition(44.85f, 5.34f, -0.88f);

	// Set up the ambient light
	light = gMenuLights[3]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(1.0f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	gMenuLights[4] = gTeamSelectScene->createEntity();
	gMenuLights[4]->addComponent<Odyssey::Transform>();
	gMenuLights[4]->getComponent<Odyssey::Transform>()->setPosition(34.85f, 5.34f, -5.5f);
	gMenuLights[4]->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);

	// Set up the directional light
	light = gMenuLights[4]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Spot);
	light->setColor(0.5f, 0.4f, 0.3f);
	light->setIntensity(1.0f);
	light->setRange(50.0f);
	light->setSpotAngle(0.5f);

	gMenuLights[5] = gTeamSelectScene->createEntity();
	gMenuLights[5]->addComponent<Odyssey::Transform>();
	gMenuLights[5]->getComponent<Odyssey::Transform>()->setPosition(34.85f, 5.34f, 0.0f);
	gMenuLights[5]->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);

	// Set up the directional light
	light = gMenuLights[5]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Spot);
	light->setColor(0.5f, 0.4f, 0.3f);
	light->setIntensity(1.0f);
	light->setRange(50.0f);
	light->setSpotAngle(0.5f);

	gMenuLights[6] = gTeamSelectScene->createEntity();
	gMenuLights[6]->addComponent<Odyssey::Transform>();
	gMenuLights[6]->getComponent<Odyssey::Transform>()->setPosition(34.85f, 5.34f, 5.5f);
	gMenuLights[6]->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);

	// Set up the directional light
	light = gMenuLights[6]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Spot);
	light->setColor(0.5f, 0.4f, 0.3f);
	light->setIntensity(1.0f);
	light->setRange(50.0f);
	light->setSpotAngle(0.5f);

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
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height)->setOpacity(0.5f);
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
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 215.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Dmg Blocked", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 250.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Healing Recived", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 285.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Support", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 320.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Healing Done", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 25.0f, rewardsImageY + 355.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Shields Given", rewardsTextProperties);

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
	
	rewardsTextProperties.fontSize = 26.0f;
	rewardsTextProperties.bold = true;
	rewardsTextProperties.fontName = L"Tw Cen MT Condensed";
	
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 200.0f, rewardsImageY + 15.0f + 390.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), (rewardsImageWidth /2), 50, L"Click Enter to Continue . . .", rewardsTextProperties);
	
	rewardsTextProperties.fontSize = 16.0f;
	rewardsTextProperties.bold = false;
	rewardsTextProperties.fontName = L"Constantia";
	
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

	canvas->setActive(false); // The rewards screen won't show up at the start
	StatTracker::Instance().SetCanvas(canvas, static_cast<unsigned int>(rewardsImageHeight / 4), static_cast<unsigned int>(rewardsImageHeight / 4));
}

void setupTowerManager(Odyssey::Scene* _sceneToAddTo, int _sceneNum)
{
	switch (_sceneNum)
	{
		case 1:
			gSceneOneTower = _sceneToAddTo->createEntity();
			gSceneOneTower->addComponent<TowerManager>();
			gSceneOneTower->getComponent<TowerManager>()->Rewards = gRewardsScreen->getComponent<Odyssey::UICanvas>();
			gSceneOneTower->getComponent<TowerManager>()->scene = _sceneToAddTo;
			break;
		case 2:
			// BOSS SCENE
			gSceneBossTower = _sceneToAddTo->createEntity();
			gSceneBossTower->addComponent<TowerManager>();
			gSceneBossTower->getComponent<TowerManager>()->Rewards = gRewardsScreen->getComponent<Odyssey::UICanvas>();
			gSceneBossTower->getComponent<TowerManager>()->scene = _sceneToAddTo;
			gSceneBossTower->getComponent<TowerManager>()->SetCurrentLevel(5);
			break;
		default:
			break;
	}
}

void setupEnemiesToCreate(Odyssey::Application* application)
{
	//Set the current tower in the TeamManager 
	TeamManager::getInstance().SetTheCurrentTower(gSceneOneTower);
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
		levelOneEnemy.pPosition = middlePosition;
		levelOneEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelOneEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);

		// Create the info prefab for the tower selection screen
		TowerSelectionPrefabFactory::getInstance().CreateTowerSelectionPrefab(application, newEnemies);

		// Clear the new enemy list 
		newEnemies.clear();
	}

	// LEVEL TWO ENEMIES
	if (true)
	{
		// Level One Enemies
		TeamManager::EnemySetups levelTwoEnemy;
		// Set enemy properties
		levelTwoEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelTwoEnemy.pPosition = leftPosition;
		levelTwoEnemy.pRotation = leftRotation;
		// Add enemy to list
		newEnemies.push_back(levelTwoEnemy);
		// Set enemy properties
		levelTwoEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelTwoEnemy.pPosition = middlePosition;
		levelTwoEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelTwoEnemy);
		// Set enemy properties
		levelTwoEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelTwoEnemy.pPosition = rightPosition;
		levelTwoEnemy.pRotation = rightRotation;
		// Add enemy to list
		newEnemies.push_back(levelTwoEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);

		// Create the info prefab for the tower selection screen
		TowerSelectionPrefabFactory::getInstance().CreateTowerSelectionPrefab(application, newEnemies);

		// Clear the new enemy list 
		newEnemies.clear();
	}

	// LEVEL THREE ENEMIES
	if (true)
	{
		// Level Three Enemies
		TeamManager::EnemySetups levelThreeEnemy;
		// Set enemy properties
		levelThreeEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelThreeEnemy.pPosition = leftPosition;
		levelThreeEnemy.pRotation = leftRotation;
		// Add enemy to list
		newEnemies.push_back(levelThreeEnemy);
		// Set enemy properties
		levelThreeEnemy.pEnemyType = TeamManager::EnemyType::MeleeDemon;
		levelThreeEnemy.pPosition = middlePosition;
		levelThreeEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelThreeEnemy);
		// Set enemy properties
		levelThreeEnemy.pEnemyType = TeamManager::EnemyType::Skeleton;
		levelThreeEnemy.pPosition = rightPosition;
		levelThreeEnemy.pRotation = rightRotation;
		// Add enemy to list
		newEnemies.push_back(levelThreeEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);

		// Create the info prefab for the tower selection screen
		TowerSelectionPrefabFactory::getInstance().CreateTowerSelectionPrefab(application, newEnemies);

		// Clear the new enemy list 
		newEnemies.clear();
	}

	// LEVEL FOUR ENEMIES
	if (true)
	{
		// Level Four Enemies
		TeamManager::EnemySetups levelFourEnemy;
		// Set enemy properties
		levelFourEnemy.pEnemyType = TeamManager::EnemyType::CasterDemon;
		levelFourEnemy.pPosition = leftPosition;
		levelFourEnemy.pRotation = leftRotation;
		// Add enemy to list
		newEnemies.push_back(levelFourEnemy);
		// Set enemy properties
		levelFourEnemy.pEnemyType = TeamManager::EnemyType::MeleeDemon;
		levelFourEnemy.pPosition = middlePosition;
		levelFourEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelFourEnemy);
		// Set enemy properties
		levelFourEnemy.pEnemyType = TeamManager::EnemyType::CasterDemon;
		levelFourEnemy.pPosition = rightPosition;
		levelFourEnemy.pRotation = rightRotation;
		// Add enemy to list
		newEnemies.push_back(levelFourEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);

		// Create the info prefab for the tower selection screen
		TowerSelectionPrefabFactory::getInstance().CreateTowerSelectionPrefab(application, newEnemies);

		// Clear the new enemy list 
		newEnemies.clear();
	}

	// LEVEL FIVE ENEMIES
	if (true)
	{
		// Level Five Enemies
		TeamManager::EnemySetups levelFiveEnemy;
		// Set enemy properties
		levelFiveEnemy.pEnemyType = TeamManager::EnemyType::Ganfaul;
		levelFiveEnemy.pPosition = middlePosition;
		levelFiveEnemy.pRotation = middleRotation;
		// Add enemy to list
		newEnemies.push_back(levelFiveEnemy);
		// Add the list to the enemies to create variable in Team Manager
		TeamManager::getInstance().AddEnemiesListToCreate(newEnemies);

		// Create the info prefab for the tower selection screen
		TowerSelectionPrefabFactory::getInstance().CreateTowerSelectionPrefab(application, newEnemies);

		// Clear the new enemy list 
		newEnemies.clear();
	}
}

void setupAudio()
{
	//RedAudioManager::Instance();
	//SFX                                |                          Paths                       |  |        Alias        |     |           Audio Type          |        |     Group     |
	// Male Hit reactions / Death
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/PaladinSFX/SFX_Paladin_Plate_Hit_Reaction_1.mp3",	"PaladinHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/PaladinSFX/SFX_Paladin_Blessing_Of_Light_1.mp3",		"BlessingOfLight",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/PaladinSFX/SFX_Paladin_Judgement_2.mp3",				"Judgement",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/PaladinSFX/SFX_Paladin_Shield_Of_Light_1.mp3",		"ShieldOfLight",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/PaladinSFX/SFX_Paladin_Smite_2.mp3",					"Smite",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/grunt1-death-pain.mp3",								"MaleHitReaction",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/grunt2-death-pain.mp3",								"MaleDeath",				RedAudioManager::AudioType::SFX);
	// Female Hit Reaction / Death																				
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/female_hit_reaction_1.mp3",							"FemaleHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/female_hit_reaction_2.mp3",							"FemaleDeath",				RedAudioManager::AudioType::SFX);
																												
	// Paladin Sound effects																					
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/armor_hit.mp3",										"PaladinHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/sword_slash.mp3",									"PaladinSwordSwing",		RedAudioManager::AudioType::SFX);
																												
	// Mage Sound Effects																						
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MageSFX/SFX_Mage_Female_Hit_Reaction_2.mp3",			"MageHitReaction",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MageSFX/SFX_Mage_Lightning_Bolt_1.mp3",				"LightningBolt",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MageSFX/SFX_Mage_Meteor_Shard_1.mp3",				"MeteorShard",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MageSFX/SFX_Mage_Fire_Storm_1.mp3", 					"FireStorm",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MageSFX/SFX_Mage_Wind_Slash_1.mp3", 					"WindSlash",				RedAudioManager::AudioType::SFX);
																												
	// Bard Sound Effects																						
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/BardSFX/SFX_Bard_Leather_Hit Reaction_1.mp3",		"BardHitReaction",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/BardSFX/SFX_Bard_Purify_1.mp3",						"Purify",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/BardSFX/SFX_Bard_Song_Of_Hope_1.mp3",				"SongOfHope",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/BardSFX/SFX_Bard_Song_Of_Misery_1.mp3",				"SongOfMisery",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/BardSFX/SFX_Bard_Star_Fire_Arrow_1.mp3",				"StarFireArrow",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/arrow_woosh_impact.mp3",								"ShootArrow",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/heal_sound.mp3",										"Heal",						RedAudioManager::AudioType::SFX);
																												
	// Warrior Sound Effects																					
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/WarriorSFX/SFX_Warrior_Man_Hit_Reaction_1.mp3",		"WarriorHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/WarriorSFX/SFX_Warrior_Armor_Buster_1.mp3",			"ArmorBuster",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/WarriorSFX/SFX_Warrior_Cleave_1.mp3",				"Cleave",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/WarriorSFX/SFX_Warrior_RAGE_1.mp3",					"Rage",						RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/WarriorSFX/SFX_Warrior_Splitting_Strike_1.mp3",		"SplittingStrike",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/three_axe_swing.mp3",								"WarriorTripleAttack",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/single_slash.mp3",									"WarriorSingleAttack",		RedAudioManager::AudioType::SFX);
																												
	// Monk Sound Effects																						
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MonkSFX/SFX_Monk_Body_Blow_Hit_Reaction_1.mp3",		"MonkHitReaction",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MonkSFX/SFX_Monk_Armor_Break_1.mp3",					"ArmorBreak",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MonkSFX/SFX_Monk_Break_Ribs_1.mp3",					"BreakRibs",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MonkSFX/SFX_Monk_Jab_1.mp3",							"Jab",						RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MonkSFX/SFX_Monk_Pressure_Point_1.mp3",				"PressurePoint",			RedAudioManager::AudioType::SFX);
																												
	// Skeleton Sound Effects																					
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/SkeletonSFX/SFX_Skeleton_Attack_1.mp3",				"SkeletonPunch",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/skeleton_death.mp3",									"SkeletonDeath",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/SkeletonSFX/SFX_Skeleton_Attack_1.mp3",				"SkeletonAttack1",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/SkeletonSFX/SFX_Skeleton_Hit_Reaction_1.mp3",		"SkeletonHit",				RedAudioManager::AudioType::SFX);
																											 
	// Ganfoul sound effects																				 
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/GanfoulSFX/SFX_Ganfoul_Hit_Reaction_1.mp3",			"GanfoulHitReaction",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/GanfoulSFX/SFX_Ganfoul_Attack_1.mp3",				"GanfoulAttack",			RedAudioManager::AudioType::SFX);

	// Summoner Sound Effects																				 
	

	// Caster Demon Sound Effects																			 
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/CasterDemonSFX/SFX_Caster_Demon_Hit_Reaction_1.mp3",	"CasterDemonHitReaction",	RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/CasterDemonSFX/SFX_Caster_Demon_Attack_1.mp3",		"CasterDemonAttack",		RedAudioManager::AudioType::SFX);

	// Melee Demon Sound Efffects																			 
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MeleeDemonSFX/SFX_Melee_Demon_Hit_Reaction_1.mp3",	"MeleeDemonHitReaction",	RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/MeleeDemonSFX/SFX_Melee_Demon_Attack_1.mp3",			"MeleeDemonAttack",			RedAudioManager::AudioType::SFX);

	// Enemy Mage Sound Efffects																			 
	

	// Music																								 
	RedAudioManager::Instance().AddAudio("assets/audio/Music/losing.mp3",										"Loss",						RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/Music/medieval_parade.mp3",								"Win",						RedAudioManager::AudioType::SFX);
																												
	// Misc Sounds Effects																						
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/fire_torch_burning.mp3",						"TorchBurning",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/fire_torch_burning_quietly.mp3",				"TorchBurningQuietly",		RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/fire_torch_burning_wind.mp3",					"TorchBurningWind",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Open.mp3",										"DoorOpen",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Close.mp3",										"DoorClose",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_poof.mp3",										"CharacterEntrance",		RedAudioManager::AudioType::SFX);
																												
	// Memes																									
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_4.mp3",								"NoManaCritical",			RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_3.mp3",								"NoManaBitch",				RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_2.mp3",								"NoManaMidium",				RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/no_mana_clip_1.mp3",								"NoManaLow",				RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/forward_aerial.mp3",								"ForwardAerial",			RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/gokusjj3.mp3",										"GokuSJJ3",					RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/windows_death_sound.mp3",							"DeathMeme",				RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/fbi_rade.mp3",										"FBIRade",					RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/gta_wanted.mp3",									"GTAWanted",				RedAudioManager::AudioType::SFX);
	// Tower select screen door sounds																			
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Open.mp3",										"DoorOpen",					RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/Door_Close.mp3",										"DoorClose",				RedAudioManager::AudioType::SFX);
																												
	//Background Sound																						 
	//RedAudioManager::Instance().AddAudio("assets/audio/Music/battle_music.mp3",							 "BackgroundBattle1",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	//RedAudioManager::Instance().AddAudio("assets/audio/Music/rage_in_the_darkness.mp3",					 "BackgroundBattle2",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/BattleMusic/Music_Battle_Track_1.mp3",				"BackgroundBattle1",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/BattleMusic/Music_Battle_Track_2.mp3",				"BackgroundBattle2",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/BattleMusic/Music_Battle_Track_3.mp3",				"BackgroundBattle3",		RedAudioManager::AudioType::Background, "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/Ambience/dark_depths.mp3",								"BackgroundMenu1",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/MenuMusic/Music_Menu_Track_1.mp3",					"BackgroundMenu2",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/MenuMusic/Music_Menu_Track_2.mp3",					"BackgroundMenu3",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	RedAudioManager::Instance().AddAudio("assets/audio/Music/MenuMusic/Music_Menu_Track_3.mp3",					"BackgroundMenu4",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	//RedAudioManager::Instance().AddAudio("assets/audio/Music/menu_music.mp3",								 "BackgroundMenu2",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	//RedAudioManager::Instance().AddAudio("assets/audio/Music/divinum_creaturae.mp3",						 "BackgroundMenu3",			RedAudioManager::AudioType::Background, "BackgroundMenu");
	// Extras
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/armor_hit.mp3",								 "ArrowHit",				RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_energy_burst.mp3",						 "ElectricBlast",			RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/magic_zap.mp3",								 "MagicZap",				RedAudioManager::AudioType::SFX);
	//RedAudioManager::Instance().AddAudio("assets/audio/SFX/magical_vanish.mp3",							 "MagicalVanish",			RedAudioManager::AudioType::SFX);																								  
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/medieval_impact_plate_armor.mp3",					"PlateArmorHit",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/large_fireball.mp3",									"LargeFireball",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/slime_sound.mp3",									"PoisonSlime",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/charge_and_fire.mp3",								"ChargeAndFire",			RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/spell_cast.mp3",										"SpellCasting",				RedAudioManager::AudioType::SFX);
	RedAudioManager::Instance().AddAudio("assets/audio/SFX/lightning_2.mp3",									"Lightning2",				RedAudioManager::AudioType::SFX);

	//Play Initial Loop
	//RedAudioManager::Instance().Loop("Death");
	//RedAudioManager::Instance().Mute();
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
	light->setColor(0.3f, 0.3f, 0.5f);
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
	light->setColor(0.3f, 0.3f, 0.5f);
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

	// Fire color
	//DirectX::XMFLOAT3 fireColor = { 0.65f, 0.3f, 0.15f };
	DirectX::XMFLOAT3 fireColor = { 0.6f, 0.3f, 0.2f };
	float torchInstensity = 2.5f;
	float torchRange = 15.0f;

	// Create the fire entity
	Odyssey::Entity* fire1 = gSceneOne->createEntity();
	
	// Set the transform position and rotation
	fire1->addComponent<Odyssey::Transform>();
	fire1->getComponent<Odyssey::Transform>()->setPosition(-9.0f, 5.571f, 2.075f);
	fire1->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, -15.0f);
	
	// Set the light's values
	light = fire1->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(fireColor);
	light->setIntensity(torchInstensity);
	light->setRange(torchRange);
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
	light->setColor(fireColor);
	light->setIntensity(torchInstensity);
	light->setRange(torchRange);
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
	light->setColor(fireColor);
	light->setIntensity(torchInstensity);
	light->setRange(torchRange);
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
	light->setColor(fireColor);
	light->setIntensity(torchInstensity);
	light->setRange(torchRange);
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

	//// Create the table candle entity
	//Odyssey::Entity* tableCandle = gSceneOne->createEntity();
	//
	//// Set the transform position and rotation
	//tableCandle->addComponent<Odyssey::Transform>();
	//tableCandle->getComponent<Odyssey::Transform>()->setPosition(7.492f, 1.964f, 16.716f);
	//tableCandle->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);
	//
	//// Set the light's values
	//light = tableCandle->addComponent<Odyssey::Light>();
	//light->setLightType(Odyssey::LightType::Point);
	//light->setColor(fireColor);
	//light->setIntensity(1.1f);
	//light->setRange(20.0f);
	//light->setSpotAngle(0.0f);

	//// Create the table candle entity
	//Odyssey::Entity* bigTableCandle = gSceneOne->createEntity();
	//
	//// Set the transform position and rotation
	//bigTableCandle->addComponent<Odyssey::Transform>();
	//bigTableCandle->getComponent<Odyssey::Transform>()->setPosition(-8.95f, 4.076f, 27.416f);
	//bigTableCandle->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);
	//
	//// Set the light's values
	//light = bigTableCandle->addComponent<Odyssey::Light>();
	//light->setLightType(Odyssey::LightType::Point);
	//light->setColor(fireColor);
	//light->setIntensity(1.5f);
	//light->setRange(20.0f);
	//light->setSpotAngle(0.0f);

	// Create the table candle entity
	Odyssey::Entity* topLight = gSceneOne->createEntity();

	// Set the transform position and rotation
	topLight->addComponent<Odyssey::Transform>();
	topLight->getComponent<Odyssey::Transform>()->setPosition(0.0f, 11.169f, 15.364f);
	topLight->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);

	// Set the light's values
	light = topLight->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(4.0f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	// Create the table candle entity
	Odyssey::Entity* backLight = gSceneOne->createEntity();

	// Set the transform position and rotation
	backLight->addComponent<Odyssey::Transform>();
	backLight->getComponent<Odyssey::Transform>()->setPosition(0.0f, 11.169f, 0.364f);
	backLight->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);

	// Set the light's values
	light = backLight->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(4.0f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	Odyssey::Entity* fog = gSceneOne->createEntity();
	fog->addComponent<Odyssey::Transform>();
	fog->getComponent<Odyssey::Transform>()->setPosition(0, 0, 0);
	fog->addComponent<Odyssey::ParticleSystem>();
	fog->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Smoke.jpg");
	fog->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.125f, 0.1f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
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
	Odyssey::Entity* gSceneOneCamera = gSceneBoss->createEntity();
	gSceneOneCamera->addComponent<Odyssey::Transform>();
	gSceneOneCamera->getComponent<Odyssey::Transform>()->setPosition(0.105f, 7.827f, 1.286f);
	gSceneOneCamera->getComponent<Odyssey::Transform>()->setRotation(28.965f, 0.0f, 0.0f);
	gSceneOneCamera->addComponent<Odyssey::Camera>();
	gSceneOneCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gSceneOneCamera->addComponent<CameraController>();

	Odyssey::RenderManager::getInstance().importScene(gSceneBoss, "assets/models/BossScene.dxm");

	// Set the light's position and direction
	gScene1Lights[0] = gSceneBoss->createEntity();
	gScene1Lights[0]->addComponent<Odyssey::Transform>();
	gScene1Lights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gScene1Lights[0]->getComponent<Odyssey::Transform>()->setRotation(15.0f, 250.0f, 0.0f);

	// Set up the directional light
	Odyssey::Light* light = gScene1Lights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	// Set the light's position and direction
	gScene1Lights[1] = gSceneBoss->createEntity();
	gScene1Lights[1]->addComponent<Odyssey::Transform>();
	gScene1Lights[1]->getComponent<Odyssey::Transform>()->setPosition(-3.5f, 3.0f, 4.5f);
	gScene1Lights[1]->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);

	// Set up the ambient light
	light = gScene1Lights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(0.55f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	// Set the light's position and direction
	gScene1Lights[2] = gSceneBoss->createEntity();
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

	// Create the table candle entity
	Odyssey::Entity* topLight = gSceneBoss->createEntity();

	// Set the transform position and rotation
	topLight->addComponent<Odyssey::Transform>();
	topLight->getComponent<Odyssey::Transform>()->setPosition(0.0f, 11.169f, 15.364f);
	topLight->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);

	// Set the light's values
	light = topLight->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(4.0f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	// Create the table candle entity
	Odyssey::Entity* backLight = gSceneBoss->createEntity();

	// Set the transform position and rotation
	backLight->addComponent<Odyssey::Transform>();
	backLight->getComponent<Odyssey::Transform>()->setPosition(0.0f, 11.169f, 0.364f);
	backLight->getComponent<Odyssey::Transform>()->setRotation(-15.0f, 0.0f, 0.0f);

	// Set the light's values
	light = backLight->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.3f, 0.3f, 0.5f);
	light->setIntensity(4.0f);
	light->setRange(20.0f);
	light->setSpotAngle(0.0f);

	Odyssey::Entity* fog = gSceneBoss->createEntity();
	fog->addComponent<Odyssey::Transform>();
	fog->getComponent<Odyssey::Transform>()->setPosition(0, 0, 0);
	fog->addComponent<Odyssey::ParticleSystem>();
	fog->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Smoke.jpg");
	fog->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.125f, 0.1f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
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

void setupLoadingScene(Odyssey::Application* application)
{
	gLoadingScene = application->createScene("Loading Screen", DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 50.0f);

	Odyssey::Entity* camera = gLoadingScene->createEntity();
	camera->addComponent<Odyssey::Transform>();
	camera->getComponent<Odyssey::Transform>()->setPosition(0.395f, 6.703f, 13.438f);
	camera->getComponent<Odyssey::Transform>()->setRotation(23.669f, -178.152f, 0.0f);
	camera->addComponent<Odyssey::Camera>();
	camera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	camera->addComponent<CameraController>();

	Odyssey::Entity* controller = gLoadingScene->createEntity();
	controller->addComponent<Odyssey::Transform>();
	controller->addComponent<LoadingScreenController>();
}

void setupSkillVFX(Odyssey::Application* application)
{
	// Create the showcase entity in the game scene
	Odyssey::Entity* showcase = gSceneOne->createEntity();
	showcase->addComponent<Odyssey::Transform>();
	showcase->getComponent<Odyssey::Transform>()->setPosition(0.0f, 1.0f, 0.0f);
	showcase->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	showcase->addComponent<SkillShowcase>();

	// PALADIN SKILLS START HERE
	setupBardVFX(application, showcase);
	setupPaladinVFX(application, showcase);
	setupSkeletonVFX(application, showcase);
	setupGanfaulVFX(application, showcase);
	setupCasterVFX(application, showcase);
}

void setupBardVFX(Odyssey::Application* application, Odyssey::Entity* showcase)
{

	// Create the skill 1 prefab
	showcase->getComponent<SkillShowcase>()->bard1 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->bard1->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->bard1->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 5.0f);

	// Add a light to the prefab
	Odyssey::Light* light = showcase->getComponent<SkillShowcase>()->bard1->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.3f, 0.6f);
	light->setRange(10.0f);
	light->setIntensity(2.0f);

	// Setup the starfire arrow vfx
	Odyssey::ParticleSystem* skillVFX = showcase->getComponent<SkillShowcase>()->bard1->addComponent<Odyssey::ParticleSystem>();
	showcase->getComponent<SkillShowcase>()->bard1->setStatic(false);
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star2.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.5f, 0.3f, 0.6f), DirectX::XMFLOAT3(0.2f, 0.4f, 0.5f));
	skillVFX->setLifetime(0.5f, 1.0f);
	skillVFX->setParticleCount(100, 100);
	skillVFX->setEmissionOverLifetime(0);
	skillVFX->setDuration(0.75f);
	skillVFX->setSpeed(2.5f, 5.0f);
	skillVFX->setSize(0.25f, 0.5f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(4.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::SpherePS(0.0f, 0.0f, 0.0f, 0.5f));

	// Create the skill 2 prefab
	showcase->getComponent<SkillShowcase>()->bard2 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->bard2->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->bard2->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 10.0f);

	// Add a light to the prefab
	light = showcase->getComponent<SkillShowcase>()->bard2->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.1f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup skill 2 vfx
	skillVFX = showcase->getComponent<SkillShowcase>()->bard2->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Bard_VFX/Music.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.8f, 0.1f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(1.0f, 1.5f);
	skillVFX->setParticleCount(0, 15);
	skillVFX->setEmissionOverLifetime(15);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.5f, 2.25f);
	skillVFX->setSize(0.75f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(1.5f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::BoxPS(0.0f, 1.0f, 0.0f, 1.5f, 2.5f, 1.5f));

	// Create the skill 3 prefab
	showcase->getComponent<SkillShowcase>()->bard3 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->bard3->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->bard3->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 15.0f);

	// Add a light to the prefab
	light = showcase->getComponent<SkillShowcase>()->bard3->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.1f, 0.8f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Skill 3 prefab
	skillVFX = showcase->getComponent<SkillShowcase>()->bard3->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Bard_VFX/Music.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.1f, 0.8f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(1.0f, 1.5f);
	skillVFX->setParticleCount(0, 15);
	skillVFX->setEmissionOverLifetime(15);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.5f, 2.25f);
	skillVFX->setSize(0.75f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(1.5f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::BoxPS(0.0f, 1.0f, 0.0f, 1.5f, 2.5f, 1.5f));

	// Create the skill 4 prefab
	showcase->getComponent<SkillShowcase>()->bard4 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->bard4->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 20.0f);

	// Add a light to the prefab
	light = showcase->getComponent<SkillShowcase>()->bard4->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(1.0f, 0.8f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup the skill 4 vfx
	showcase->getComponent<SkillShowcase>()->bard4->addComponent<PurifyMover>();
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::Transform>()->setScale(0.02f, 0.02f, 0.02f);
	Odyssey::RenderManager::getInstance().importModel(showcase->getComponent<SkillShowcase>()->bard4, "assets/models/Magic_Rune.dxm", false);
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::MeshRenderer>()->setShadowCaster(false);
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setAlphaBlend(true);
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setReceiveShadow(false);
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor({ 0.12f, 1.0f, 0.29f, 1.0f });
	showcase->getComponent<SkillShowcase>()->bard4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setShader("../OdysseyEngine/shaders/UnlitPixelShader.cso");
	skillVFX = showcase->getComponent<SkillShowcase>()->bard4->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.12f, 1.0f, 0.29f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.75f, 1.5f);
	skillVFX->setParticleCount(0, 90);
	skillVFX->setEmissionOverLifetime(90);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(3.0f, 4.5f);
	skillVFX->setSize(0.25f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 1.0f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, true));
}

void setupPaladinVFX(Odyssey::Application* application, Odyssey::Entity* showcase)
{
	// Create the skill 1 prefab
	showcase->getComponent<SkillShowcase>()->paladin1 = application->createPrefab();
	// Import the hammer model
	Odyssey::RenderManager::getInstance().importModel(showcase->getComponent<SkillShowcase>()->paladin1, "assets/models/Hammer.dxm", false);
	showcase->getComponent<SkillShowcase>()->paladin1->getComponent<Odyssey::Transform>()->setScale(0.05f, 0.05f, 0.05f);
	showcase->getComponent<SkillShowcase>()->paladin1->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 5.0f);
	// Add the skill mover to the prefab
	showcase->getComponent<SkillShowcase>()->paladin1->addComponent<JudgementMover>();
	showcase->getComponent<SkillShowcase>()->paladin1->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });

	// Add a light to the prefab
	Odyssey::Light* light = showcase->getComponent<SkillShowcase>()->paladin1->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.45f, 0.65f, 0.35f);
	light->setRange(10.0f);
	light->setIntensity(1.0f);

	// Setup the skill 1 vfx
	Odyssey::ParticleSystem* skillVFX = showcase->getComponent<SkillShowcase>()->paladin1->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Electric.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.25f, 0.75f);
	skillVFX->setParticleCount(0, 50);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(4.0f);
	skillVFX->setSpeed(1.25f, 1.75f);
	skillVFX->setSize(0.25f, 0.75f);
	skillVFX->setSizeOverLifetime(0.0f, 0.5f);
	skillVFX->setGravity(2.5f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::SpherePS(0.0f, 0.25f, 0.0f, 0.5f));

	// Create the skill 2 prefab
	showcase->getComponent<SkillShowcase>()->paladin2 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->paladin2->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->paladin2->getComponent<Odyssey::Transform>()->setPosition(20.0f, 7.5f, 10.0f);
	showcase->getComponent<SkillShowcase>()->paladin2->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 180.0f);

	// Add a light to the prefab
	light = showcase->getComponent<SkillShowcase>()->paladin2->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.75f, 0.65f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup the skill 2 vfx
	showcase->getComponent<SkillShowcase>()->paladin2->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	skillVFX = showcase->getComponent<SkillShowcase>()->paladin2->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Lightning.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.4f, 0.2f), DirectX::XMFLOAT3(0.75f, 0.1f, 0.1f));
	skillVFX->setLifetime(2.0f, 2.0f);
	skillVFX->setParticleCount(0, 15);
	skillVFX->setEmissionOverLifetime(30);
	skillVFX->setDuration(2.0f);
	skillVFX->setSpeed(8.5f, 9.0f);
	skillVFX->setSize(1.0f, 1.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.75f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.001f, 0.001f, 0.001f));
	skillVFX = showcase->getComponent<SkillShowcase>()->paladin2->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Electric.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(2.0f, 2.0f);
	skillVFX->setParticleCount(0, 75);
	skillVFX->setEmissionOverLifetime(150);
	skillVFX->setDuration(2.0f);
	skillVFX->setSpeed(8.5f, 9.0f);
	skillVFX->setSize(0.5f, 0.75f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));

	// Create the skill 3 prefab
	showcase->getComponent<SkillShowcase>()->paladin3 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->paladin3->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 15.0f);

	// Add a light to the prefab
	light = showcase->getComponent<SkillShowcase>()->paladin3->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(1.0f, 0.8f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup the skill 3 vfx
	showcase->getComponent<SkillShowcase>()->paladin3->addComponent<PurifyMover>();
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::Transform>()->setScale(0.02f, 0.02f, 0.02f);
	Odyssey::RenderManager::getInstance().importModel(showcase->getComponent<SkillShowcase>()->paladin3, "assets/models/Magic_Rune.dxm", false);
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::MeshRenderer>()->setShadowCaster(false);
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setAlphaBlend(true);
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setReceiveShadow(false);
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor({ 0.75f, 0.65f, 0.1f, 1.0f });
	showcase->getComponent<SkillShowcase>()->paladin3->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setShader("../OdysseyEngine/shaders/UnlitPixelShader.cso");
	skillVFX = showcase->getComponent<SkillShowcase>()->paladin3->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star2.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.75f, 1.5f);
	skillVFX->setParticleCount(0, 90);
	skillVFX->setEmissionOverLifetime(90);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(3.0f, 4.5f);
	skillVFX->setSize(0.25f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 1.0f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, true));

	// Create the skill 4 prefab
	showcase->getComponent<SkillShowcase>()->paladin4 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->paladin4->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 20.0f);

	// Add a light to the prefab
	light = showcase->getComponent<SkillShowcase>()->paladin4->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(1.0f, 0.8f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup the skill 4 vfx
	showcase->getComponent<SkillShowcase>()->paladin4->addComponent<PurifyMover>();
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::Transform>()->setScale(0.02f, 0.02f, 0.02f);
	Odyssey::RenderManager::getInstance().importModel(showcase->getComponent<SkillShowcase>()->paladin4, "assets/models/Magic_Rune.dxm", false);
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::MeshRenderer>()->setShadowCaster(false);
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setAlphaBlend(true);
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setReceiveShadow(false);
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor({ 0.75f, 0.65f, 0.1f, 1.0f });
	showcase->getComponent<SkillShowcase>()->paladin4->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setShader("../OdysseyEngine/shaders/UnlitPixelShader.cso");
	skillVFX = showcase->getComponent<SkillShowcase>()->paladin4->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star2.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.75f, 1.5f);
	skillVFX->setParticleCount(0, 90);
	skillVFX->setEmissionOverLifetime(90);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(3.0f, 4.5f);
	skillVFX->setSize(0.25f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 1.0f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, true));
}

void setupSkeletonVFX(Odyssey::Application* application, Odyssey::Entity* showcase)
{
	// Create the skill 4 prefab
	showcase->getComponent<SkillShowcase>()->skeleton1 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->skeleton1->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->skeleton1->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 5.0f);

	Odyssey::ParticleSystem* skillVFX = showcase->getComponent<SkillShowcase>()->skeleton1->addComponent<Odyssey::ParticleSystem>();
	//Odyssey::RenderManager::getInstance().importModel(showcase->getComponent<SkillShowcase>()->skeleton1, "assets/models/Hammer.dxm", false);
	//showcase->getComponent<SkillShowcase>()->skeleton1->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 90.0f);
	showcase->getComponent<SkillShowcase>()->skeleton1->addComponent<SpinKickMover>();
	//showcase->getComponent<SkillShowcase>()->skeleton1->addComponent<SpinKickMover>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "SmokeParticle.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.0f), DirectX::XMFLOAT3(144.0f, 144.0f, 144.0f));
	skillVFX->setLifetime(0.75f, 0.75f);
	skillVFX->setParticleCount(0, 100);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(7.0f);
	skillVFX->setSpeed(1.25f, 1.75f);
	skillVFX->setSize(0.25f, 0.75f);
	skillVFX->setSizeOverLifetime(0.0f, 0.5f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));

	showcase->getComponent<SkillShowcase>()->skeleton2 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->skeleton2->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->skeleton2->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 10.0f);

	skillVFX = showcase->getComponent<SkillShowcase>()->skeleton2->addComponent<Odyssey::ParticleSystem>();
	//Odyssey::RenderManager::getInstance().importModel(showcase->getComponent<SkillShowcase>()->skeleton1, "assets/models/Hammer.dxm", false);
	//showcase->getComponent<SkillShowcase>()->skeleton1->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 90.0f);
	showcase->getComponent<SkillShowcase>()->skeleton2->addComponent<PunchMover>(DirectX::XMFLOAT3(20.0f, 2.5f, 10.0f), DirectX::XMFLOAT3(20.0f, 4.5f, 15.0f));
	//showcase->getComponent<SkillShowcase>()->skeleton1->addComponent<SpinKickMover>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "SmokeParticle.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.0f), DirectX::XMFLOAT3(144.0f, 144.0f, 144.0f));
	skillVFX->setLifetime(0.75f, 0.75f);
	skillVFX->setParticleCount(0, 100);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(7.0f);
	skillVFX->setSpeed(1.25f, 1.75f);
	skillVFX->setSize(0.75f, 0.75f);
	skillVFX->setSizeOverLifetime(0.5f, 1.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));
}

void setupGanfaulVFX(Odyssey::Application* application, Odyssey::Entity* showcase)
{
	// Create the skill 1 prefab
	showcase->getComponent<SkillShowcase>()->ganfaul1 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->ganfaul1->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->ganfaul1->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 5.0f);

	Odyssey::ParticleSystem* skillVFX = showcase->getComponent<SkillShowcase>()->ganfaul1->addComponent<Odyssey::ParticleSystem>();
	showcase->getComponent<SkillShowcase>()->ganfaul1->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);
	showcase->getComponent<SkillShowcase>()->ganfaul1->addComponent<BossAttackMover>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.0f, 10.0f, 215.0f), DirectX::XMFLOAT3(144.0f, 0.0f, 73.0f));
	skillVFX->setLifetime(0.75f, 0.75f);
	skillVFX->setParticleCount(0, 300);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(8.0f);
	skillVFX->setSpeed(1.75f, 1.75f);
	skillVFX->setSize(0.25f, 0.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(-0.5f, 0.0f, 0.0f, 1.0f, 1.0f, true));
	
	/*skillVFX = showcase->getComponent<SkillShowcase>()->ganfaul1->addComponent<Odyssey::ParticleSystem>();
	showcase->getComponent<SkillShowcase>()->ganfaul1->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);
	showcase->getComponent<SkillShowcase>()->ganfaul1->addComponent<BossAttackMover>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.0f), DirectX::XMFLOAT3(144.0f, 144.0f, 144.0f));
	skillVFX->setLifetime(0.75f, 0.75f);
	skillVFX->setParticleCount(0, 300);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(8.0f);
	skillVFX->setSpeed(1.25f, 1.75f);
	skillVFX->setSize(0.25f, 0.25f);
	skillVFX->setSizeOverLifetime(0.0f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(0.5f, 0.0f, 0.0f, 1.0f, 1.0f, true));*/
}

void setupCasterVFX(Odyssey::Application* application, Odyssey::Entity* showcase)
{
	showcase->getComponent<SkillShowcase>()->caster1 = application->createPrefab();
	showcase->getComponent<SkillShowcase>()->caster1->addComponent<Odyssey::Transform>();
	showcase->getComponent<SkillShowcase>()->caster1->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 5.0f);

	Odyssey::ParticleSystem* skillVFX = showcase->getComponent<SkillShowcase>()->caster1->addComponent<Odyssey::ParticleSystem>();
	showcase->getComponent<SkillShowcase>()->caster1->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);
	showcase->getComponent<SkillShowcase>()->caster1->addComponent<CasterMover>(0.0f);
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.0f, 10.0f, 215.0f), DirectX::XMFLOAT3(144.0f, 0.0f, 73.0f));
	skillVFX->setLifetime(0.9f, 0.9f);
	skillVFX->setParticleCount(0, 300);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.75f, 1.75f);
	skillVFX->setSize(0.25f, 0.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));

	skillVFX = showcase->getComponent<SkillShowcase>()->caster1->addComponent<Odyssey::ParticleSystem>();
	//showcase->getComponent<SkillShowcase>()->caster1->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);
	showcase->getComponent<SkillShowcase>()->caster1->addComponent<CasterMover>(3.14159265f/2.0f);
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.0f, 10.0f, 215.0f), DirectX::XMFLOAT3(144.0f, 0.0f, 73.0f));
	skillVFX->setLifetime(0.9f, 0.9f);
	skillVFX->setParticleCount(0, 300);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.75f, 1.75f);
	skillVFX->setSize(0.25f, 0.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));

	skillVFX = showcase->getComponent<SkillShowcase>()->caster1->addComponent<Odyssey::ParticleSystem>();
	//showcase->getComponent<SkillShowcase>()->caster1->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);
	showcase->getComponent<SkillShowcase>()->caster1->addComponent<CasterMover>(3.14159265f);
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.0f, 10.0f, 215.0f), DirectX::XMFLOAT3(144.0f, 0.0f, 73.0f));
	skillVFX->setLifetime(0.9f, 0.9f);
	skillVFX->setParticleCount(0, 300);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.75f, 1.75f);
	skillVFX->setSize(0.25f, 0.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));

	skillVFX = showcase->getComponent<SkillShowcase>()->caster1->addComponent<Odyssey::ParticleSystem>();
	//showcase->getComponent<SkillShowcase>()->caster1->getComponent<Odyssey::Transform>()->setRotation(90.0f, 0.0f, 0.0f);
	showcase->getComponent<SkillShowcase>()->caster1->addComponent<CasterMover>(3.14159265f + (3.14159265f/2.0f));
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.0f, 10.0f, 215.0f), DirectX::XMFLOAT3(144.0f, 0.0f, 73.0f));
	skillVFX->setLifetime(0.9f, 0.9f);
	skillVFX->setParticleCount(0, 300);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.75f, 1.75f);
	skillVFX->setSize(0.25f, 0.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));
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
	SaveLoad::Instance().LoadSettings();
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