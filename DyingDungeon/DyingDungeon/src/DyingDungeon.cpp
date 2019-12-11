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
#include "Component.h"
#include "FileManager.h"
#include "Transform.h"
#include "Application.h"
#include "RenderDevice.h"
#include "Material.h"

// Game Includes
#include "ExampleComponent.h"
#include "BattleInstance.h"

namespace
{
    // Rendering resources
    std::shared_ptr<Odyssey::RenderWindow> gMainWindow;
    std::shared_ptr<Odyssey::RenderTarget> gRenderTarget;
    // Scene resources
    std::shared_ptr<Odyssey::Scene> gMainScene;
	std::shared_ptr<Odyssey::GameObject> gArena;
	std::shared_ptr<Odyssey::GameObject> gPaladin;
	std::shared_ptr<Odyssey::GameObject> gSkeleton;
    // Light resources
    std::shared_ptr<Odyssey::Light> gDirLight;
    std::shared_ptr<Odyssey::Light> gLights[20];
}

// Forward declarations
int playGame();
void setupPipeline(Odyssey::RenderDevice* renderDevice);
void setupLighting();
void setupArena();
void setupPaladin();
void setupSkeleton();

//Tristen's Stuff
std::vector<std::shared_ptr<Odyssey::GameObject>> CreateTeam(int _amountOfPlayersOnTeam);

void setupPipeline(Odyssey::RenderDevice* renderDevice)
{
	// Create a clear render target pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ClearRenderTargetPass> rtvPass = renderDevice->createClearRTVPass(gMainWindow->getRenderTarget(), true);
	Odyssey::RenderPipelineManager::getInstance().addPass(rtvPass);

	// Create a skybox pass and add it to the render pipeline 
	std::shared_ptr<Odyssey::SkyboxPass> skyboxPass = renderDevice->createSkyboxPass("Skybox.dds", gMainWindow->getRenderTarget());
	Odyssey::RenderPipelineManager::getInstance().addPass(skyboxPass);

	// Create a shadow pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ShadowPass> shadowPass = renderDevice->createShadowPass(gDirLight, 2048, 2048);
	Odyssey::RenderPipelineManager::getInstance().addPass(shadowPass);

	// Create an opaque pass and add it to the render pipeline
	std::shared_ptr<Odyssey::OpaquePass> opaquePass = renderDevice->createOpaquePass(gMainWindow->getRenderTarget());
	Odyssey::RenderPipelineManager::getInstance().addPass(opaquePass);

	// Create a transparent pass and add it to the render pipeline
	std::shared_ptr<Odyssey::TransparentPass> transparentPass = renderDevice->createTransparentPass(gMainWindow->getRenderTarget());
	Odyssey::RenderPipelineManager::getInstance().addPass(transparentPass);

	// Create a debugging pass and add it to the render pipeline
	//std::shared_ptr<Odyssey::DebugPass>debugPass = renderDevice->createDebugPass(gMainWindow->getRenderTarget());
	//Odyssey::RenderPipelineManager::getInstance().addPass(debugPass);
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

	gLights[0] = std::make_shared<Odyssey::Light>();
	gLights[0]->mLightType = Odyssey::LightType::Point;
	gLights[0]->mWorldPosition = DirectX::XMFLOAT4(0.0, 10.0f, -20.0f, 1.0f);
	gLights[0]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[0]->mColor = DirectX::XMFLOAT4(0.2f, 0.2f, 0.3f, 1.0f);
	gLights[0]->mIntensity = 3.0f;
	gLights[0]->mRange = 15.0f;
	gLights[0]->mSpotAngle = 0.0f;

	gLights[1] = std::make_shared<Odyssey::Light>();
	gLights[1]->mLightType = Odyssey::LightType::Point;
	gLights[1]->mWorldPosition = DirectX::XMFLOAT4(0.0, 10.0f, 20.0f, 1.0f);
	gLights[1]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[1]->mColor = DirectX::XMFLOAT4(0.2f, 0.2f, 0.3f, 1.0f);
	gLights[1]->mIntensity = 3.0f;
	gLights[1]->mRange = 15.0f;
	gLights[1]->mSpotAngle = 0.0f;

	gLights[2] = std::make_shared<Odyssey::Light>();
	gLights[2]->mLightType = Odyssey::LightType::Point;
	gLights[2]->mWorldPosition = DirectX::XMFLOAT4(-1.0f, 11.0f, -35.0f, 1.0f);
	gLights[2]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[2]->mColor = DirectX::XMFLOAT4(0.7f, 0.5f, 0.4f, 1.0f);
	gLights[2]->mIntensity = 1.0f;
	gLights[2]->mRange = 10.0f;
	gLights[2]->mSpotAngle = 0.0f;

	gLights[3] = std::make_shared<Odyssey::Light>();
	gLights[3]->mLightType = Odyssey::LightType::Point;
	gLights[3]->mWorldPosition = DirectX::XMFLOAT4(1.0f, 24.0f, -1.7f, 1.0f);
	gLights[3]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[3]->mColor = DirectX::XMFLOAT4(0.1f, 0.1f, 0.25f, 1.0f);
	gLights[3]->mIntensity = 1.0f;
	gLights[3]->mRange = 10.0f;
	gLights[3]->mSpotAngle = 0.0f;

	gLights[4] = std::make_shared<Odyssey::Light>();
	gLights[4]->mLightType = Odyssey::LightType::Point;
	gLights[4]->mWorldPosition = DirectX::XMFLOAT4(5.25f, 5.5f, -13.34f, 1.0f);
	gLights[4]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[4]->mColor = DirectX::XMFLOAT4(0.7f, 0.5f, 0.4f, 1.0f);
	gLights[4]->mIntensity = 1.0f;
	gLights[4]->mRange = 10.0f;
	gLights[4]->mSpotAngle = 0.0f;

	gLights[5] = std::make_shared<Odyssey::Light>();
	gLights[5]->mLightType = Odyssey::LightType::Point;
	gLights[5]->mWorldPosition = DirectX::XMFLOAT4(-13.67f, 5.17f, 4.15f, 1.0f);
	gLights[5]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[5]->mColor = DirectX::XMFLOAT4(0.7f, 0.5f, 0.4f, 1.0f);
	gLights[5]->mIntensity = 1.0f;
	gLights[5]->mRange = 10.0f;
	gLights[5]->mSpotAngle = 0.0f;

	gLights[6] = std::make_shared<Odyssey::Light>();
	gLights[6]->mLightType = Odyssey::LightType::Point;
	gLights[6]->mWorldPosition = DirectX::XMFLOAT4(-3.0f, 5.4f, 14.66f, 1.0f);
	gLights[6]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[6]->mColor = DirectX::XMFLOAT4(0.7f, 0.5f, 0.4f, 1.0f);
	gLights[6]->mIntensity = 1.0f;
	gLights[6]->mRange = 10.0f;
	gLights[6]->mSpotAngle = 0.0f;

	gLights[7] = std::make_shared<Odyssey::Light>();
	gLights[7]->mLightType = Odyssey::LightType::Point;
	gLights[7]->mWorldPosition = DirectX::XMFLOAT4(3.0f, 5.4f, 14.66f, 1.0f);
	gLights[7]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[7]->mColor = DirectX::XMFLOAT4(0.7f, 0.5f, 0.4f, 1.0f);
	gLights[7]->mIntensity = 1.0f;
	gLights[7]->mRange = 10.0f;
	gLights[7]->mSpotAngle = 0.0f;

	gLights[8] = std::make_shared<Odyssey::Light>();
	gLights[8]->mLightType = Odyssey::LightType::Point;
	gLights[8]->mWorldPosition = DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f);
	gLights[8]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[8]->mColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	gLights[8]->mIntensity = 2.0f;
	gLights[8]->mRange = 20.0f;
	gLights[8]->mSpotAngle = 0.0f;

	gLights[9] = std::make_shared<Odyssey::Light>();
	gLights[9]->mLightType = Odyssey::LightType::Spot;
	gLights[9]->mWorldPosition = DirectX::XMFLOAT4(0.0f, 25.0f, -10.0f, 1.0f);
	gLights[9]->mWorldDirection = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	gLights[9]->mColor = DirectX::XMFLOAT4(1.0f, 0.7f, 0.5f, 1.0f);
	gLights[9]->mIntensity = 2.0f;
	gLights[9]->mRange = 50.0f;
	gLights[9]->mSpotAngle = 0.1f;

	gLights[10] = std::make_shared<Odyssey::Light>();
	gLights[10]->mLightType = Odyssey::LightType::Spot;
	gLights[10]->mWorldPosition = DirectX::XMFLOAT4(0.0f, 25.0f, 3.0f, 1.0f);
	gLights[10]->mWorldDirection = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	gLights[10]->mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 0.7f, 1.0f);
	gLights[10]->mIntensity = 1.0f;
	gLights[10]->mRange = 50.0f;
	gLights[10]->mSpotAngle = 0.1f;


	gMainScene->addLight(gDirLight);
	gMainScene->addLight(gLights[0]);
	gMainScene->addLight(gLights[1]);
	gMainScene->addLight(gLights[2]);
	gMainScene->addLight(gLights[3]);
	gMainScene->addLight(gLights[4]);
	gMainScene->addLight(gLights[5]);
	gMainScene->addLight(gLights[6]);
	gMainScene->addLight(gLights[7]);
	gMainScene->addLight(gLights[8]);
	gMainScene->addLight(gLights[9]);
	gMainScene->addLight(gLights[10]);
}

void setupArena()
{
	gArena = std::make_shared<Odyssey::GameObject>();
	gArena->addComponent<Odyssey::Transform>();
	Odyssey::FileManager::getInstance().importModel(gArena, "assets/models/TestArena.dxm");
	gArena->addComponent<ExampleComponent>();
	gMainScene->addSceneObject(gArena);
}

void setupPaladin()
{
	gPaladin = std::make_shared<Odyssey::GameObject>();
	gPaladin->addComponent<Odyssey::Transform>();
	gPaladin->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gPaladin->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.6f, 3.0f);
	gPaladin->getComponent<Odyssey::Transform>()->setRotation(0.0f, 180.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(gPaladin, "assets/models/Paladin.dxm");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Judgement", "assets/animations/Paladin_Idle.dxanim");
	gMainScene->addSceneObject(gPaladin);
}

std::vector<std::shared_ptr<Odyssey::GameObject>> CreateTeam(int _amountOfPlayersOnTeam)
{
	//Make a new vector
	std::vector<std::shared_ptr<Odyssey::GameObject>> newTeam;

	int NumOfPlayersOnEachTeam = _amountOfPlayersOnTeam;
	//Give amount of player to the team
	for (int i = 0; i < NumOfPlayersOnEachTeam; i++)
	{
		std::shared_ptr<Odyssey::GameObject> newPlayer;
		newTeam.push_back(newPlayer);
	}

	return newTeam;
}

void setupSkeleton()
{
	gSkeleton = std::make_shared<Odyssey::GameObject>();
	gSkeleton->addComponent<Odyssey::Transform>();
	gSkeleton->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gSkeleton->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.5f, -10.0f);
	Odyssey::FileManager::getInstance().importModel(gSkeleton, "assets/models/Skeleton.dxm");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Skeleton_Idle.dxanim");
	gMainScene->addSceneObject(gSkeleton);
}

int playGame()
{
	// Set up the application and create a render window
	Odyssey::Application application;
	gMainWindow = application.createRenderWindow("Dying Dungeon", 1920, 1080);

	// Get the render device
	Odyssey::RenderDevice* renderDevice = application.getRenderDevice();

	// Create the main scene
	gMainScene = renderDevice->createScene();

	// Set up the scene lighting
	setupLighting();

	// Set up the default rendering pipeline
	setupPipeline(renderDevice);

	// Load the arena scene
	setupArena();

	// Set up the paladin
	setupPaladin();

	setupSkeleton();

	// Set up Battle Instance
	//std::vector<std::shared_ptr<Odyssey::GameObject>> playerUnit = CreateTeam(1);
	//std::vector<std::shared_ptr<Odyssey::GameObject>> enemyUnit = CreateTeam(1);
	//std::shared_ptr<Odyssey::GameObject> currentBattle;
	//currentBattle->addComponent<BattleInstance>(playerUnit, enemyUnit);

	// Set the initial view and projection matrix
	gMainScene->mMainCamera.setPosition(7.31f, 6.578f, 5.579f);
	gMainScene->mMainCamera.setProjectionValues(60.0f, gMainWindow->getAspectRatio(), 0.1f, 75.0f);

	// Set the active scene
	application.setActiveScene(gMainScene);

	// Run the application
	return application.update();
}

#pragma region WINDOWS CODE
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return playGame();
}
#pragma endregion