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
	std::shared_ptr<Odyssey::Scene> gScene2;
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
	std::shared_ptr<Odyssey::Entity> gScene1Lights[24];
	std::shared_ptr<Odyssey::Entity> gScene2Lights[24];
	std::shared_ptr<Odyssey::Entity> gMenuLights[24];
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
//void setupSkillHover(Odyssey::UICanvas* canvas, std::wstring character, std::wstring skillName, std::wstring icon, std::wstring manaCost, std::wstring skillType, std::wstring numTargets, std::wstring skillValue, std::wstring description);
//void setupPaladinSkills(std::shared_ptr<Odyssey::Entity> character, float xAnchor, float yAnchor);
//void setupMageSkills(std::shared_ptr<Odyssey::Entity> character, float xAnchor, float yAnchor);

// BUILD 3 STUFF
void setupScene2();

int playGame()
{
	// TODO: BREAKPOINT FOR YOUR DUMBASS MEMORY LEAKS
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(708704);
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
	gGameScene = gRenderDevice->createScene(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 50.0f);
	gGameScene->setSkybox("Skybox.dds");
	gScene2 = gRenderDevice->createScene(DirectX::XMFLOAT3(0.0f, 0.0f, 50.0f), 100.0f);
	gScene2->setSkybox("Dusk.dds");

	// Set up the scene lighting
	setupLighting();

	// Set up the default rendering pipeline
	setupPipeline(gRenderDevice, application);

	// Set up camera
	setupCamera();

	// Assign the width and height for the UI Manager
	GameUIManager::getInstance().SetScreenWidthAndHeight(gMainWindow->getWidth(), gMainWindow->getHeight());
	// Set the Character Factory's render refrence
	CharacterFactory::getInstance().mRenderRefrence = gRenderDevice;

	// Set up the main menu
	setupMainMenu(application.get());

	// Set up the tower selection screen
	setupMenu(gRenderDevice, application.get(), gTowerSelectScene, gTowerSelectMenu, L"assets/images/TowerSelectionBackground.png", "TowerSelection", MenuComponent::eTowerSelector);
	// Create the tower selection menu
	GameUIManager::getInstance().CreateTowerSelectMenuCanvas(gTowerSelectScene);

	// Set up the game user interface
	setupGameInterface();

	// Set up the team selection screen
	// I need to setupGameInterafce before this gets called because that is where the canvases are getting added to the gGameMenu.
	setupTeamSelectMenu(application.get());

	GameUIManager::getInstance().CreateStatsMenuCanvas(gMainMenu);

	// Create the battle log for the game
	GameUIManager::getInstance().CreateBattleLog(gGameScene);

	// Load the arena scene
	setupArena();

	// BUILD 2
	setupFire();

	// Add the gGameMenu to the gGameScene after I have added all the elements
	gGameScene->addEntity(gGameMenu);

	// Create Pause Menu
	GameUIManager::getInstance().CreatePauseMenuCanvas(gGameScene);

	// Create scene 2
	setupScene2();

	application->addScene("Scene2", gScene2);

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
	std::shared_ptr<Odyssey::SkyboxPass> skyboxPass = renderDevice->createSkyboxPass(gMainWindow);
	application->addRenderPass(skyboxPass);

	// Create a shadow pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ShadowPass> shadowPass = renderDevice->createShadowPass(4096, 4096);
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
	gScene1Lights[0] = std::make_shared<Odyssey::Entity>();
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
	gScene1Lights[1] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[1]->addComponent<Odyssey::Transform>();
	gScene1Lights[1]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 10.0f, 0.0f);
	light = gScene1Lights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.5f, 0.5f);
	light->setIntensity(2.0f);
	light->setRange(50.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 1
	gScene1Lights[2] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[2]->addComponent<Odyssey::Transform>();
	gScene1Lights[2]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, 14.4f);
	light = gScene1Lights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 2
	gScene1Lights[3] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[3]->addComponent<Odyssey::Transform>();
	gScene1Lights[3]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, 7.5f);
	light = gScene1Lights[3]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 3
	gScene1Lights[4] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[4]->addComponent<Odyssey::Transform>();
	gScene1Lights[4]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, -6.22f);
	light = gScene1Lights[4]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Pillar 4
	gScene1Lights[5] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[5]->addComponent<Odyssey::Transform>();
	gScene1Lights[5]->getComponent<Odyssey::Transform>()->setPosition(-5.45f, 4.75f, -13.22f);
	light = gScene1Lights[5]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Pillar 1
	gScene1Lights[6] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[6]->addComponent<Odyssey::Transform>();
	gScene1Lights[6]->getComponent<Odyssey::Transform>()->setPosition(5.45f, 4.75f, 14.4f);
	light = gScene1Lights[6]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Pillar 2
	gScene1Lights[7] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[7]->addComponent<Odyssey::Transform>();
	gScene1Lights[7]->getComponent<Odyssey::Transform>()->setPosition(5.45f, 4.75f, 7.5f);
	light = gScene1Lights[7]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Pillar 3
	gScene1Lights[8] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[8]->addComponent<Odyssey::Transform>();
	gScene1Lights[8]->getComponent<Odyssey::Transform>()->setPosition(5.45f, 4.75f, -13.22f);
	light = gScene1Lights[8]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Door Light 1
	gScene1Lights[9] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[9]->addComponent<Odyssey::Transform>();
	gScene1Lights[9]->getComponent<Odyssey::Transform>()->setPosition(-12.0f, 4.75f, -6.7f);
	light = gScene1Lights[9]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Left Door Light 2
	gScene1Lights[10] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[10]->addComponent<Odyssey::Transform>();
	gScene1Lights[10]->getComponent<Odyssey::Transform>()->setPosition(-12.0f, 4.75f, 1.2f);
	light = gScene1Lights[10]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Door Light 1
	gScene1Lights[11] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[11]->addComponent<Odyssey::Transform>();
	gScene1Lights[11]->getComponent<Odyssey::Transform>()->setPosition(12.74f, 5.0f, -2.85f);
	light = gScene1Lights[11]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// World-Space Right Door Light 2
	gScene1Lights[12] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[12]->addComponent<Odyssey::Transform>();
	gScene1Lights[12]->getComponent<Odyssey::Transform>()->setPosition(12.74f, 5.0f, 4.25f);
	light = gScene1Lights[12]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(5.0f);
	light->setSpotAngle(0.0f);

	// Library-Area Candle Light
	gScene1Lights[13] = std::make_shared<Odyssey::Entity>();
	gScene1Lights[13]->addComponent<Odyssey::Transform>();
	gScene1Lights[13]->getComponent<Odyssey::Transform>()->setPosition(-1.25f, 12.5f, -35.0f);
	light = gScene1Lights[13]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(2.0f);
	light->setRange(12.5f);
	light->setSpotAngle(0.0f);

	gGameScene->addLight(gScene1Lights[0]);
	gGameScene->addLight(gScene1Lights[1]);
	gGameScene->addLight(gScene1Lights[2]);
	gGameScene->addLight(gScene1Lights[3]);
	gGameScene->addLight(gScene1Lights[4]);
	gGameScene->addLight(gScene1Lights[5]);
	gGameScene->addLight(gScene1Lights[6]);
	gGameScene->addLight(gScene1Lights[7]);
	gGameScene->addLight(gScene1Lights[8]);
	gGameScene->addLight(gScene1Lights[9]);
	gGameScene->addLight(gScene1Lights[10]);
	gGameScene->addLight(gScene1Lights[11]);
	gGameScene->addLight(gScene1Lights[12]);
	gGameScene->addLight(gScene1Lights[13]);
}

void setupCamera()
{
	gMainCamera = std::make_shared<Odyssey::Entity>();
	gMainCamera->addComponent<Odyssey::Transform>();
	gMainCamera->getComponent<Odyssey::Transform>()->setPosition(0.395f, 6.703f, 13.438f);
	gMainCamera->getComponent<Odyssey::Transform>()->setRotation(23.669f, -178.152f, 0.0f);
	gMainCamera->addComponent<Odyssey::Camera>();
	gMainCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gMainCamera->addComponent<CameraController>();
	gGameScene->addEntity(gMainCamera);
}

void setupMenu(Odyssey::RenderDevice* renderDevice, Odyssey::Application* application, std::shared_ptr<Odyssey::Scene>& _sceneObject, std::shared_ptr<Odyssey::Entity>& _entityToAdd, const wchar_t* _imageName, std::string _menuName, MenuComponent _menuComponent)
{
	_sceneObject = renderDevice->createScene(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 20.0f);
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
	gMenuLights[0] = std::make_shared<Odyssey::Entity>();
	gMenuLights[0]->addComponent<Odyssey::Transform>();
	gMenuLights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gMenuLights[0]->getComponent<Odyssey::Transform>()->setRotation(25.0f, 100.0f, 0.0f);
	Odyssey::Light* light = gMenuLights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.4f, 0.5f, 0.7f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	gMenuLights[1] = std::make_shared<Odyssey::Entity>();
	gMenuLights[1]->addComponent<Odyssey::Transform>();
	gMenuLights[1]->getComponent<Odyssey::Transform>()->setPosition(0.0, 10.0f, 0.0f);
	light = gMenuLights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.5f, 0.5f);
	light->setIntensity(5.0f);
	light->setRange(30.0f);
	light->setSpotAngle(0.0f);

	gMainMenu->addLight(gMenuLights[0]);
	gMainMenu->addLight(gMenuLights[1]);

	// Create a paladin and add him to the main menu scene
	std::shared_ptr<Odyssey::Entity> characterToAdd;
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(2.0f, -2.5f, 6.0f, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 uiPosition = { 0.0f, 0.0f };
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Paladin, L"Main Menu Paladin", charPosition, charRotation, uiPosition, false, uiPosition, gMainMenu);

	// Create the UI
	GameUIManager::getInstance().CreateMainMenuCanvas(gMainMenu);
}

void setupTeamSelectMenu(Odyssey::Application* application)
{
	// Set up the team selection screen
	setupMenu(gRenderDevice, application, gTeamSelectScene, gTeamSelectMenu, L"", "TeamSelection", MenuComponent::eTeamSelector);

	// Create vector of all game scene
	std::vector<std::shared_ptr<Odyssey::Scene>> pListOfGameScenes;
	pListOfGameScenes.push_back(gGameScene);
	//pListOfGameScenes.push_back(gScene2);
	// Set the list of scenes in team select controller
	gTeamSelectMenu->getComponent<TeamSelectionController>()->SetGameScenes(pListOfGameScenes);

	// Set the game entity that the hud will be attached to
	gTeamSelectMenu->getComponent<TeamSelectionController>()->SetGameEntity(gGameMenu);

	// Set up a directional light
	gMenuLights[2] = std::make_shared<Odyssey::Entity>();
	gMenuLights[2]->addComponent<Odyssey::Transform>();
	gMenuLights[2]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gMenuLights[2]->getComponent<Odyssey::Transform>()->setRotation(25.0f, 100.0f, 0.0f);
	Odyssey::Light* light = gMenuLights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.4f, 0.5f, 0.7f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	gMenuLights[3] = std::make_shared<Odyssey::Entity>();
	gMenuLights[3]->addComponent<Odyssey::Transform>();
	gMenuLights[3]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 2.0f);
	light = gMenuLights[3]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.5f, 0.5f);
	light->setIntensity(1.0f);
	light->setRange(30.0f);
	light->setSpotAngle(0.0f);

	gTeamSelectScene->addLight(gMenuLights[2]);
	gTeamSelectScene->addLight(gMenuLights[3]);

	// Set some variables for positioning
	float scaleAmount = 0.020f;
	float xOffset = -3.0f;
	float yHeight = -2.0f;
	float zDepth = 6.0f;

	// Create the character's for the second slot
	std::shared_ptr<Odyssey::Entity> characterToAdd;
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 uiPosition = { 0.0f, 0.0f };
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Paladin, L"Team Select Paladin", charPosition, charRotation, uiPosition, false, uiPosition, gTeamSelectScene);
	characterToAdd->getComponent<Odyssey::Transform>()->setScale(scaleAmount, scaleAmount, scaleAmount);

	// Create the character's for the second slot
	xOffset = 0.0f;
	charPosition = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Mage, L"Team Select Mage", charPosition, charRotation, uiPosition, false, uiPosition, gTeamSelectScene);
	characterToAdd->getComponent<Odyssey::Transform>()->setScale(scaleAmount, scaleAmount, scaleAmount);

	// Create the character's for the third slot
	xOffset = 3.0f;
	charPosition = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Bard, L"Team Select Bard", charPosition, charRotation, uiPosition, false, uiPosition, gTeamSelectScene);
	characterToAdd->getComponent<Odyssey::Transform>()->setScale(scaleAmount, scaleAmount, scaleAmount);

	// Create the UI for the team selection
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
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height)->setOpacity(0.5f);
	UINT rewardsImageWidth = width*0.6;
	UINT rewardsImageHeight = height*0.6;
	float rewardsImageX = (width / 2.0f) - (static_cast<float>(rewardsImageWidth) / 2.0f);
	float rewardsImageY = (height / 2.0f) - (static_cast<float>(rewardsImageHeight) / 2.0f);

	Odyssey::TextProperties rewardsTextProperties = gDefaultText;
	rewardsTextProperties.fontSize = 30.0f;
	rewardsTextProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;

	//canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX, rewardsImageY), L"assets/images/ResultsMenu.png", rewardsImageWidth, rewardsImageHeight);
	//canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX, rewardsImageY), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), rewardsImageWidth, rewardsImageHeight)->setOpacity(0.5);
	
	//Stat Background
	//protrait backgrounds
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 20.0f, rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, (rewardsImageHeight / 4) + 50)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 20.0f, rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 20.0f, rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);

	//attack backgrounds
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY - 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, 40)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f, rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);

	//defence backgrounds
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY - 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, 40)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageWidth / 4.0f) + 20.0f, rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	
	//aid backgrounds
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY - 40.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, 40)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY + (rewardsImageHeight * 0.3333f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + 2.0f * ((rewardsImageWidth / 4.0f) + 20.0f), rewardsImageY + (rewardsImageHeight * 0.6667f) + 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20)->setOpacity(0.8f);

	//Character Protraits
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f, rewardsImageY + 20.0f), L"assets/images/Guy.png", rewardsImageHeight /4, rewardsImageHeight /4);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f, rewardsImageY + (20.0f + rewardsImageHeight * 0.3333f)), L"assets/images/Guy.png", rewardsImageHeight / 4, rewardsImageHeight / 4);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(rewardsImageX + 30.0f, rewardsImageY + (20.0f + rewardsImageHeight * 0.6667f)), L"assets/images/Guy.png", rewardsImageHeight / 4, rewardsImageHeight / 4);
	
	//Stat Text
	//stat discriptors
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 175.0f + (rewardsImageHeight / 8) + 10, rewardsImageY - 50.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Attack", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 190.0f + (rewardsImageHeight / 2) + 40, rewardsImageY - 50.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Defense", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + 205.0f + (rewardsImageHeight)+80, rewardsImageY - 50.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageHeight / 4) + 20, 60, L"Aid", rewardsTextProperties);
	
	rewardsTextProperties.fontSize = 16.0f;
	
	//attack text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (rewardsImageWidth / 4), rewardsImageY + 7.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Attack: NN.NN%\nDamage Dealt: NN.NN\nDamage Success: NN.NN%", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (rewardsImageWidth / 4), rewardsImageY + (7.0f + rewardsImageHeight *0.3333f) ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Attack: NN.NN%\nDamage Dealt: NN.NN\nDamage Success: NN.NN%", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (rewardsImageWidth / 4), rewardsImageY + (7.0f + rewardsImageHeight * 0.6667f) ), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Attack: NN.NN%\nDamage Dealt: NN.NN\nDamage Success: NN.NN%", rewardsTextProperties);
	//defense text
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (2.0f * (rewardsImageWidth / 4)) + 30.0f, rewardsImageY + 7.0f), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Defend: NN.NN%\nDamage Taken: NN.NN\nDamage Blocked: NN.NN%\nHealth Gained: NN.NN", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (2.0f * (rewardsImageWidth / 4)) + 30.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.3333f)), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Defend: NN.NN%\nDamage Taken: NN.NN\nDamage Blocked: NN.NN%\nHealth Gained: NN.NN", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (2.0f * (rewardsImageWidth / 4)) + 30.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.6667f)), DirectX::XMFLOAT4(0.0f, 151.0f, 255.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Defend: NN.NN%\nDamage Taken: NN.NN\nDamage Blocked: NN.NN%\nHealth Gained: NN.NN", rewardsTextProperties);
	//aid texts
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (3.0f * (rewardsImageWidth / 4)) + 70.0f, rewardsImageY + 7.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Aid: NN.NN%\nHeal: NN.NN\nDefence Buff: NN.NN", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (3.0f * (rewardsImageWidth / 4)) + 70.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.3333f)), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Aid: NN.NN%\nHeal: NN.NN\nDefence Buff: NN.NN", rewardsTextProperties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(rewardsImageX + (3.0f * (rewardsImageWidth / 4)) + 70.0f, rewardsImageY + (7.0f + rewardsImageHeight * 0.6667f)), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), (rewardsImageWidth / 4) + 20, (rewardsImageHeight / 4) + 20, L"Aid: NN.NN%\nHeal: NN.NN\nDefence Buff: NN.NN", rewardsTextProperties);

	canvas->setActive(false); // The rewards screen won't show up at the start
	StatTracker::Instance().SetCanvas(canvas, rewardsImageHeight / 4, rewardsImageHeight / 4);
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
		/*owner->pDmgText = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(anchorX, anchorY), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 72, 72, L"11.25", properties);
		owner->pDmgText->setOpacity(0.0f);*/
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
	//RedAudioManager::Instance();
	//SFX
	RedAudioManager::Instance().AddAudio("assets/audio/bone_punch.mp3", "SkeletonAttack");
	RedAudioManager::Instance().AddAudio("assets/audio/sword_slash.mp3", "PaladinAttack");
	RedAudioManager::Instance().AddAudio("assets/audio/armor_hit.mp3", "PaladinHit");
	RedAudioManager::Instance().AddAudio("assets/audio/losing.mp3", "Loss");
	RedAudioManager::Instance().AddAudio("assets/audio/medieval_parade.mp3", "Win");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_4.mp3", "NoManaCritical");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_3.mp3", "NoManaBitch");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_2.mp3", "NoManaMidium");
	RedAudioManager::Instance().AddAudio("assets/audio/no_mana_clip_1.mp3", "NoManaLow");
	RedAudioManager::Instance().AddAudio("assets/audio/arrow_woosh_impact.mp3", "ArrowReleaseHit");
	RedAudioManager::Instance().AddAudio("assets/audio/armor_hit.mp3", "ArrowHit");
	RedAudioManager::Instance().AddAudio("assets/audio/magic_energy_burst.mp3", "ElectricBlast");
	RedAudioManager::Instance().AddAudio("assets/audio/magic_swish.mp3", "MagicWoosh");
	RedAudioManager::Instance().AddAudio("assets/audio/magic_zap.mp3", "MagicZap");
	RedAudioManager::Instance().AddAudio("assets/audio/magical_vanish.mp3", "MagicalVanish");
	RedAudioManager::Instance().AddAudio("assets/audio/medieval_impact_plate_armor.mp3", "PlateArmorHit");


	// Tower select screen door sounds
	RedAudioManager::Instance().AddAudio("assets/audio/Door_Open.mp3", "DoorOpen");
	RedAudioManager::Instance().AddAudio("assets/audio/Door_Close.mp3", "DoorClose");
	
	//Background Sound
	RedAudioManager::Instance().AddAudio("assets/audio/battle_music.mp3", "BackgroundBattle");
	RedAudioManager::Instance().AddAudio("assets/audio/menu_music.mp3", "BackgroundMenu");
	
	//Background Sound
	//RedAudioManager::Instance().AddAudio("assets/audio/battle_music.mp3", "BackgroundBattle");
	//RedAudioManager::Instance().AddAudio("assets/audio/menu_music.mp3", "BackgroundMenu");
	
	//Play Initial Loop
	RedAudioManager::Instance().Loop("NoManaLow");
	//RedAudioManager::Instance().Stop("BackgroundMenu");
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

	std::shared_ptr<Odyssey::Entity> fire6 = std::make_shared<Odyssey::Entity>();
	fire6->addComponent<Odyssey::Transform>();
	fire6->getComponent<Odyssey::Transform>()->setPosition(5.59f, 4.67f, 7.59f);
	fire6->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire6->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire6->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire6->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire6->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire6->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire6->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire6->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire6->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire6->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire6->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire7 = std::make_shared<Odyssey::Entity>();
	fire7->addComponent<Odyssey::Transform>();
	fire7->getComponent<Odyssey::Transform>()->setPosition(5.59f, 4.67f, 14.62f);
	fire7->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire7->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire7->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire7->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire7->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire7->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire7->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire7->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire7->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire7->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire7->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire8 = std::make_shared<Odyssey::Entity>();
	fire8->addComponent<Odyssey::Transform>();
	fire8->getComponent<Odyssey::Transform>()->setPosition(-5.74f, 4.67f, 7.53f);
	fire8->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire8->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire8->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire8->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire8->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire8->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire8->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire8->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire8->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire8->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire8->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire9 = std::make_shared<Odyssey::Entity>();
	fire9->addComponent<Odyssey::Transform>();
	fire9->getComponent<Odyssey::Transform>()->setPosition(-5.78f, 4.67f, 14.62f);
	fire9->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire9->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire9->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire9->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire9->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire9->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire9->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire9->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire9->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire9->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire9->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire10 = std::make_shared<Odyssey::Entity>();
	fire10->addComponent<Odyssey::Transform>();
	fire10->getComponent<Odyssey::Transform>()->setPosition(12.66f, 4.7f, 4.12f);
	fire10->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire10->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire10->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire10->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire10->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire10->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire10->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire10->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire10->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire10->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire10->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	std::shared_ptr<Odyssey::Entity> fire11 = std::make_shared<Odyssey::Entity>();
	fire11->addComponent<Odyssey::Transform>();
	fire11->getComponent<Odyssey::Transform>()->setPosition(-12.66f, 4.25f, 1.53f);
	fire11->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire11->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire11->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire11->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.75f);
	fire11->getComponent<Odyssey::ParticleSystem>()->setParticleCount(25, 75);
	fire11->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire11->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire11->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 0.45f);
	fire11->getComponent<Odyssey::ParticleSystem>()->setSize(0.4f, 0.45f);
	fire11->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire11->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.075f, 35.0f, 35.0f));

	gGameScene->addEntity(fire1);
	gGameScene->addEntity(fire2);
	gGameScene->addEntity(fire3);
	gGameScene->addEntity(fire4);
	gGameScene->addEntity(fire5);
	gGameScene->addEntity(fire6);
	gGameScene->addEntity(fire7);
	gGameScene->addEntity(fire8);
	gGameScene->addEntity(fire9);
	gGameScene->addEntity(fire10);
	gGameScene->addEntity(fire11);
}

//void setuppaladinskills(std::shared_ptr<odyssey::entity> character, float xanchor, float yanchor)
//{
//	// todo: refactor this later
//	// add skill icons to portrait
//	odyssey::uicanvas* canvas1 = character->addcomponent<odyssey::uicanvas>();
//	odyssey::uicanvas* canvas2 = character->addcomponent<odyssey::uicanvas>();
//	odyssey::uicanvas* canvas3 = character->addcomponent<odyssey::uicanvas>();
//	odyssey::uicanvas* canvas4 = character->addcomponent<odyssey::uicanvas>();
//	skillhovercomponent* hover = character->addcomponent<skillhovercomponent>();
//
//	// basic attack icon
//	odyssey::sprite2d* skill1 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/paladin_skill_1.png", 52, 45);
//	// basic attack hover
//	setupskillhover(canvas1, l"paladin", l"basic attack", l"assets/images/paladin_skill_1.png", l"0", l"attack", l"1", l"15 dmg",
//		l"description: strike an enemy with divine power dealing 15 damage with a 30% chance to apply provoke. restores 5 mana.");
//	// basic attack trigger
//	hover->registersprite(skill1, canvas1);
//
//	// increment the icon
//	xanchor += 56.5f;
//
//	// wind slash icon
//	odyssey::sprite2d* skill2 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/paladin_skill_2.png", 52, 45);
//	// wind slash hover
//	setupskillhover(canvas2, l"paladin", l"judgement", l"assets/images/paladin_skill_2.png", l"15", l"attack", l"1", l"200 dmg",
//		l"description: smite the enemy with holy light dealing 200 damage and healing the paladin for 15 health. costs 15 mana.");
//	// wind slash trigger
//	hover->registersprite(skill2, canvas2);
//
//	// increment the icon
//	xanchor += 56.5f;
//
//	// firestorm icon
//	odyssey::sprite2d* skill3 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/paladin_skill_3.png", 52, 45);
//	// firestorm hover
//	setupskillhover(canvas3, l"paladin", l"shield of light", l"assets/images/paladin_skill_3.png", l"20", l"support", l"4", l"+25 shield",
//		l"description: a shield of light slams down in front of all team members granting a 25 health shield for 3 turns. costs 20 mana.");
//	// firestorm trigger
//	hover->registersprite(skill3, canvas3);
//
//	// increment the icon
//	xanchor += 56.5f;
//
//	// lightning bolt icon
//	odyssey::sprite2d* skill4 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/paladin_skill_4.png", 52, 45);
//	// lightning bolt hover
//	setupskillhover(canvas4, l"paladin", l"blessing of light", l"assets/images/paladin_skill_4.png", l"15", l"support", l"4", l"+50% def",
//		l"description: protects all allies from harm granting them 50% reduced damage for 3 turns. costs 15 mana.");
//	// lightning bolt trigger
//	hover->registersprite(skill4, canvas4);
//}
//
//void setupmageskills(std::shared_ptr<odyssey::entity> character, float xanchor, float yanchor)
//{
//	// todo: refactor this later
//	// add skill icons to portrait
//	odyssey::uicanvas* canvas1 = character->addcomponent<odyssey::uicanvas>();
//	odyssey::uicanvas* canvas2 = character->addcomponent<odyssey::uicanvas>();
//	odyssey::uicanvas* canvas3 = character->addcomponent<odyssey::uicanvas>();
//	odyssey::uicanvas* canvas4 = character->addcomponent<odyssey::uicanvas>();
//	skillhovercomponent* hover = character->addcomponent<skillhovercomponent>();
//
//	// basic attack icon
//	odyssey::sprite2d* skill1 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/mage_skill_1.png", 52, 45);
//	// basic attack hover
//	setupskillhover(canvas1, l"mage", l"basic attack", l"assets/images/mage_skill_1.png", l"0", l"attack", l"1", l"10 dmg",
//		l"description: send forth an orb of chaotic magic inflicting 10 damage with a 15% chance to stun. refunds 10 mana.");
//	// basic attack trigger
//	hover->registersprite(skill1, canvas1);
//
//	// increment the icon
//	xanchor += 56.5f;
//
//	// wind slash icon
//	odyssey::sprite2d* skill2 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/mage_skill_2.png", 52, 45);
//	// wind slash hover
//	setupskillhover(canvas2, l"mage", l"wind slash", l"assets/images/mage_skill_2.png", l"10", l"attack", l"4", l"15 x 4 dmg",
//		l"description: slash all enemies with a burst of wind dealing 15 damage per hit with a 100% chance to inflict speed down. costs 10 mana.");
//	// wind slash trigger
//	hover->registersprite(skill2, canvas2);
//
//	// increment the icon
//	xanchor += 56.5f;
//
//	// firestorm icon
//	odyssey::sprite2d* skill3 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/mage_skill_3.png", 52, 45);
//	// firestorm hover
//	setupskillhover(canvas3, l"mage", l"firestorm", l"assets/images/mage_skill_3.png", l"30", l"attack", l"4", l"50 x 4 dmg",
//		l"description: conjure a hellstorm dealing 50 damage to all enemies and inflicting burn with a 100% chance. costs 30 mana.");
//	// firestorm trigger
//	hover->registersprite(skill3, canvas3);
//
//	// increment the icon
//	xanchor += 56.5f;
//
//	// lightning bolt icon
//	odyssey::sprite2d* skill4 = character->getcomponent<odyssey::uicanvas>()->addelement<odyssey::sprite2d>(directx::xmfloat2(xanchor, yanchor), l"assets/images/mage_skill_4.png", 52, 45);
//	// lightning bolt hover
//	setupskillhover(canvas4, l"mage", l"lightning bolt", l"assets/images/mage_skill_4.png", l"35", l"attack", l"1", l"60 dmg",
//		l"description: channel a bolt of lightning dealing 60 damage to a single enemy with a 100% chance to stun. costs 35 mana.");
//	// lightning bolt trigger
//	hover->registersprite(skill4, canvas4);
//}

void setupScene2()
{
	// Setup the camera
	std::shared_ptr<Odyssey::Entity> gMainCamera = std::make_shared<Odyssey::Entity>();
	gMainCamera->addComponent<Odyssey::Transform>();
	gMainCamera->getComponent<Odyssey::Transform>()->setPosition(-5.64f, 12.85f, 1.2f);
	gMainCamera->getComponent<Odyssey::Transform>()->setRotation(14.17f, 8.14f, 0.0f);
	gMainCamera->addComponent<Odyssey::Camera>();
	gMainCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gMainCamera->addComponent<CameraController>();
	gScene2->addEntity(gMainCamera);

	// Import the scene
	Odyssey::FileManager::getInstance().importScene(gScene2, "assets/models/SceneFinal.dxm");

	// Set up a directional light
	gScene2Lights[0] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[0]->addComponent<Odyssey::Transform>();
	gScene2Lights[0]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	gScene2Lights[0]->getComponent<Odyssey::Transform>()->setRotation(15.0f, 250.0f, 0.0f);

	Odyssey::Light* light = gScene2Lights[0]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Directional);
	light->setColor(0.65f, 0.2f, 0.1f);
	light->setIntensity(1.0f);
	light->setRange(0.0f);
	light->setSpotAngle(0.0f);

	// Ambient Lighting
	gScene2Lights[1] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[1]->addComponent<Odyssey::Transform>();
	gScene2Lights[1]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 5.0f, 30.0f);
	light = gScene2Lights[1]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.25f, 0.25f, 0.25f);
	light->setIntensity(0.1f);
	light->setRange(75.0f);
	light->setSpotAngle(0.0f);

	// Ambient Lighting
	gScene2Lights[2] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[2]->addComponent<Odyssey::Transform>();
	gScene2Lights[2]->getComponent<Odyssey::Transform>()->setPosition(0.0f, 10.0f, 40.0f);
	light = gScene2Lights[2]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.2f, 0.1f);
	light->setIntensity(0.5f);
	light->setRange(100.0f);
	light->setSpotAngle(0.0f);

	// Fire 1
	gScene2Lights[3] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[3]->addComponent<Odyssey::Transform>();
	gScene2Lights[3]->getComponent<Odyssey::Transform>()->setPosition(-18.3f, 11.75f, 22.8f);
	light = gScene2Lights[3]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(1.0f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	// Fire 2
	gScene2Lights[4] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[4]->addComponent<Odyssey::Transform>();
	gScene2Lights[4]->getComponent<Odyssey::Transform>()->setPosition(-18.3f, 11.75f, 4.0f);
	light = gScene2Lights[4]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(1.0f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	// Fire 3
	gScene2Lights[5] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[5]->addComponent<Odyssey::Transform>();
	gScene2Lights[5]->getComponent<Odyssey::Transform>()->setPosition(-9.58f, 10.5f, 84.08f);
	light = gScene2Lights[5]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(1.0f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	// Fire 4
	gScene2Lights[6] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[6]->addComponent<Odyssey::Transform>();
	gScene2Lights[6]->getComponent<Odyssey::Transform>()->setPosition(-23.1f, 10.50f, 84.08f);
	light = gScene2Lights[6]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(1.0f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	// Candle Light
	gScene2Lights[7] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[7]->addComponent<Odyssey::Transform>();
	gScene2Lights[7]->getComponent<Odyssey::Transform>()->setPosition(-19.06f, 10.17f, 56.03f);
	light = gScene2Lights[7]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.5f, 0.4f);
	light->setIntensity(0.25f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	// Hall Light
	gScene2Lights[8] = std::make_shared<Odyssey::Entity>();
	gScene2Lights[8]->addComponent<Odyssey::Transform>();
	gScene2Lights[8]->getComponent<Odyssey::Transform>()->setPosition(-10.0f, 5.0f, 100.0f);
	light = gScene2Lights[8]->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.7f, 0.25f, 0.1f);
	light->setIntensity(2.0f);
	light->setRange(25.0f);
	light->setSpotAngle(0.0f);

	gScene2->addLight(gScene2Lights[0]);
	gScene2->addLight(gScene2Lights[1]);
	gScene2->addLight(gScene2Lights[2]);
	gScene2->addLight(gScene2Lights[3]);
	gScene2->addLight(gScene2Lights[4]);
	gScene2->addLight(gScene2Lights[5]);
	gScene2->addLight(gScene2Lights[6]);
	gScene2->addLight(gScene2Lights[7]);
	gScene2->addLight(gScene2Lights[8]);

	// Setup VFX
	std::shared_ptr<Odyssey::Entity> fire1 = std::make_shared<Odyssey::Entity>();
	fire1->addComponent<Odyssey::Transform>();
	fire1->getComponent<Odyssey::Transform>()->setPosition(-18.4f, 11.75f, 22.8f);
	fire1->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire1->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire1->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire1->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.55f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire1->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire1->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.25f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setSize(0.75f, 1.0f);
	fire1->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire1->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.2f, 30.0f, 30.0f));
	gScene2->addEntity(fire1);

	std::shared_ptr<Odyssey::Entity> fire2 = std::make_shared<Odyssey::Entity>();
	fire2->addComponent<Odyssey::Transform>();
	fire2->getComponent<Odyssey::Transform>()->setPosition(-18.4f, 11.75f, 4.0f);
	fire2->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire2->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire2->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire2->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.5f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire2->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire2->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.25f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setSize(0.75f, 1.0f);
	fire2->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire2->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.2f, 30.0f, 30.0f));
	gScene2->addEntity(fire2);

	std::shared_ptr<Odyssey::Entity> fire3 = std::make_shared<Odyssey::Entity>();
	fire3->addComponent<Odyssey::Transform>();
	fire3->getComponent<Odyssey::Transform>()->setPosition(-9.58f, 10.50f, 84.08f);
	fire3->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire3->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire3->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire3->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.5f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire3->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire3->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.25f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setSize(0.75f, 1.0f);
	fire3->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire3->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.2f, 30.0f, 30.0f));
	gScene2->addEntity(fire3);

	std::shared_ptr<Odyssey::Entity> fire4 = std::make_shared<Odyssey::Entity>();
	fire4->addComponent<Odyssey::Transform>();
	fire4->getComponent<Odyssey::Transform>()->setPosition(-23.1f, 10.50f, 84.08f);
	fire4->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fire4->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fire4->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(1.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fire4->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.25f, 1.5f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setParticleCount(75, 175);
	fire4->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(90);
	fire4->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSpeed(0.25f, 1.25f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setSize(0.75f, 1.0f);
	fire4->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fire4->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.2f, 30.0f, 30.0f));
	gScene2->addEntity(fire4);

	std::shared_ptr<Odyssey::Entity> fog = std::make_shared<Odyssey::Entity>();
	fog->addComponent<Odyssey::Transform>();
	fog->getComponent<Odyssey::Transform>()->setPosition(0, 0, 0);
	fog->addComponent<Odyssey::ParticleSystem>(*gRenderDevice);
	fog->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Smoke.jpg");
	fog->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.1f, 0.05f, 0.05f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fog->getComponent<Odyssey::ParticleSystem>()->setLifetime(12.5f, 25.0f);
	fog->getComponent<Odyssey::ParticleSystem>()->setParticleCount(0, 1000);
	fog->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(60);
	fog->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	fog->getComponent<Odyssey::ParticleSystem>()->setSpeed(1.25f, 1.5f);
	fog->getComponent<Odyssey::ParticleSystem>()->setSize(20.0f, 20.0f);
	fog->getComponent<Odyssey::ParticleSystem>()->setGravity(3.0f);
	fog->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	fog->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::BoxPS(-10.0f, -5.5f, 30.0f, 55.0f, 1.0f, 95.0f));
	gScene2->addEntity(fog);

	// Set up the characters
	std::shared_ptr<Odyssey::Entity> gMage = std::make_shared<Odyssey::Entity>();
	gMage->addComponent<Odyssey::Transform>();
	gMage->getComponent<Odyssey::Transform>()->setScale(0.055f, 0.055f, 0.055f);
	gMage->getComponent<Odyssey::Transform>()->setPosition(-10.0f, 0.0f, 25.0f);
	gMage->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(gMage, "assets/models/Mage.dxm", true);
	gMage->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Mage/Mage_Idle.dxanim");
	gMage->setStatic(false);
	gScene2->addEntity(gMage);

	std::shared_ptr<Odyssey::Entity> paladin = std::make_shared<Odyssey::Entity>();
	paladin->addComponent<Odyssey::Transform>();
	paladin->getComponent<Odyssey::Transform>()->setScale(0.055f, 0.055f, 0.055f);
	paladin->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 25.0f);
	paladin->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(paladin, "assets/models/Paladin.dxm", true);
	paladin->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Paladin/Paladin_Idle.dxanim");
	paladin->setStatic(false);
	gScene2->addEntity(paladin);

	std::shared_ptr<Odyssey::Entity> paladin2 = std::make_shared<Odyssey::Entity>();
	paladin2->addComponent<Odyssey::Transform>();
	paladin2->getComponent<Odyssey::Transform>()->setScale(0.055f, 0.055f, 0.055f);
	paladin2->getComponent<Odyssey::Transform>()->setPosition(10.0f, 0.0f, 25.0f);
	paladin2->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(paladin2, "assets/models/Paladin.dxm", true);
	paladin2->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Paladin/Paladin_Idle.dxanim");
	paladin2->setStatic(false);
	gScene2->addEntity(paladin2);

	std::shared_ptr<Odyssey::Entity> ganfaul = std::make_shared<Odyssey::Entity>();
	ganfaul->addComponent<Odyssey::Transform>();
	ganfaul->getComponent<Odyssey::Transform>()->setScale(0.055f, 0.055f, 0.055f);
	ganfaul->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 45.0f);
	ganfaul->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(ganfaul, "assets/models/Ganfaul.dxm", false);
	ganfaul->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Ganfaul/Ganfaul_Idle.dxanim");
	ganfaul->setStatic(false);
	gScene2->addEntity(ganfaul);
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