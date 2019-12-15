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
#include "Camera.h"
#include "RedAudioManager.h"

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
		//Scene
	std::shared_ptr<Odyssey::Scene> gMainScene;
	std::shared_ptr<Odyssey::GameObject> gMainCamera;
	//Game Objects
	std::shared_ptr<Odyssey::GameObject> gArena;
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
void setupPipeline(Odyssey::RenderDevice* renderDevice);
void setupLighting();
void setupArena();
void setupPaladin();
void setupSkeleton();

//Tristen's Stuff
void setUpTowerManager();

void setupPipeline(Odyssey::RenderDevice* renderDevice)
{
	// Create a clear render target pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ClearRenderTargetPass> rtvPass = renderDevice->createClearRTVPass(gMainWindow->getRenderTarget(), true);
	Odyssey::RenderPipelineManager::getInstance().addPass(rtvPass);

	// Create a skybox pass and add it to the render pipeline 
	std::shared_ptr<Odyssey::SkyboxPass> skyboxPass = renderDevice->createSkyboxPass("Skybox.dds", gMainWindow->getRenderTarget());
	Odyssey::RenderPipelineManager::getInstance().addPass(skyboxPass);

	// Create a shadow pass and add it to the render pipeline
	std::shared_ptr<Odyssey::ShadowPass> shadowPass = renderDevice->createShadowPass(gDirLight, 4096, 4096);
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
	gMainScene->addLight(gLights[11]);
	gMainScene->addLight(gLights[12]);
}

void setupArena()
{
	Odyssey::FileManager::getInstance().importScene(gMainScene, "assets/models/TestArena.dxm");
}

void setupPaladin()
{
	gPaladin = std::make_shared<Odyssey::GameObject>();
	gPaladin->addComponent<Odyssey::Transform>();
	gPaladin->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gPaladin->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.6f, 3.0f);
	gPaladin->getComponent<Odyssey::Transform>()->setRotation(0.0f, 180.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(gPaladin, "assets/models/Paladin.dxm", true);
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Paladin_Idle.dxanim");
	gPaladin->addComponent<HeroComponent>();
	gPaladin->getComponent<HeroComponent>()->SetName("Paladin");
	gMainScene->addSceneObject(gPaladin);
}

void setUpTowerManager()
{
	gPlayerUnit.push_back(gPaladin);
	gEnemyUnit.push_back(gSkeleton);
	gCurrentTower = std::make_shared<Odyssey::GameObject>();
	gCurrentTower->addComponent<TowerManager>(gPlayerUnit, gEnemyUnit, 5);
	gMainScene->addSceneObject(gCurrentTower);
}

void setUpCamera()
{
	gMainCamera = std::make_shared<Odyssey::GameObject>();
	gMainCamera->addComponent<Odyssey::Transform>();
	gMainCamera->getComponent<Odyssey::Transform>()->setPosition(7.44f, 6.13f, 4.03f);
	gMainCamera->getComponent<Odyssey::Transform>()->setRotation(23.5f, -129.55f, 0.0f);
	gMainCamera->addComponent<Odyssey::Camera>();
	gMainCamera->getComponent<Odyssey::Camera>()->setAspectRatio(gMainWindow->getAspectRatio());
	gMainCamera->addComponent<CameraController>();
	gMainScene->addSceneObject(gMainCamera);
}

void setupSkeleton()
{
	gSkeleton = std::make_shared<Odyssey::GameObject>();
	gSkeleton->addComponent<Odyssey::Transform>();
	gSkeleton->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gSkeleton->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.5f, -10.0f);
	gSkeleton->getComponent<Odyssey::Transform>()->setRotation(0.0f, 180,0);
	Odyssey::FileManager::getInstance().importModel(gSkeleton, "assets/models/Skeleton.dxm", false);
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Skeleton_Idle.dxanim");
	gSkeleton->getComponent<Odyssey::Animator>()->setDebugEnabled(true);
	gSkeleton->addComponent<EnemyComponent>();
	gSkeleton->getComponent<EnemyComponent>()->SetName("Skeleton");
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

	// Set up the skeleton
	setupSkeleton();

	// Set up the tower manager
	setUpTowerManager();

	// Set up camera
	setUpCamera();

	// Set the active scene
	application.setActiveScene(gMainScene);

	//Play audio
	//RedAudioManager::Instance()->AddAudio("assets/audio/battle_music.mp3", "Background");
	//RedAudioManager::Instance()->Loop("Background");

	// Run the application
	return application.update();
}

int main()
{
	wWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}

#pragma region WINDOWS CODE
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return playGame();
}
#pragma endregion