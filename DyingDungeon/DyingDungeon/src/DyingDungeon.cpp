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
	gDirLight->mWorldDirection = DirectX::XMFLOAT4(0.75f, -0.45f, -0.055f, 0.0f);
	gDirLight->mColor = DirectX::XMFLOAT4(0.4f, 0.5f, 0.7f, 1.0f);
	gDirLight->mIntensity = 1.0f;
	gDirLight->mRange = 0.0f;
	gDirLight->mSpotAngle = 0.0f;

	// First Level Arena Lights
	// Arena ambient
	gLights[0] = std::make_shared<Odyssey::Light>();
	gLights[0]->mLightType = Odyssey::LightType::Point;
	gLights[0]->mWorldPosition = DirectX::XMFLOAT4(0.0, 10.0f, 0.0f, 1.0f);
	gLights[0]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[0]->mColor = DirectX::XMFLOAT4(0.3f, 0.4f, 0.5f, 1.0f);
	gLights[0]->mIntensity = 2.0f;
	gLights[0]->mRange = 20.0f;
	gLights[0]->mSpotAngle = 0.0f;

	// World-Space Left Pillar 1
	gLights[1] = std::make_shared<Odyssey::Light>();
	gLights[1]->mLightType = Odyssey::LightType::Point;
	gLights[1]->mWorldPosition = DirectX::XMFLOAT4(-5.45f, 4.75f, 14.4f, 1.0f);
	gLights[1]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[1]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[1]->mIntensity = 2.0f;
	gLights[1]->mRange = 5.0f;
	gLights[1]->mSpotAngle = 0.0f;

	// World-Space Left Pillar 2
	gLights[2] = std::make_shared<Odyssey::Light>();
	gLights[2]->mLightType = Odyssey::LightType::Point;
	gLights[2]->mWorldPosition = DirectX::XMFLOAT4(-5.45f, 4.75f, 7.5f, 1.0f);
	gLights[2]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[2]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[2]->mIntensity = 2.0f;
	gLights[2]->mRange = 5.0f;
	gLights[2]->mSpotAngle = 0.0f;

	// World-Space Left Pillar 3
	gLights[3] = std::make_shared<Odyssey::Light>();
	gLights[3]->mLightType = Odyssey::LightType::Point;
	gLights[3]->mWorldPosition = DirectX::XMFLOAT4(-5.45f, 4.75f, -6.22f, 1.0f);
	gLights[3]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[3]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[3]->mIntensity = 2.0f;
	gLights[3]->mRange = 5.0f;
	gLights[3]->mSpotAngle = 0.0f;

	// World-Space Left Pillar 4
	gLights[4] = std::make_shared<Odyssey::Light>();
	gLights[4]->mLightType = Odyssey::LightType::Point;
	gLights[4]->mWorldPosition = DirectX::XMFLOAT4(-5.45f, 4.75f, -13.22f, 1.0f);
	gLights[4]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[4]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[4]->mIntensity = 2.0f;
	gLights[4]->mRange = 5.0f;
	gLights[4]->mSpotAngle = 0.0f;

	// World-Space Right Pillar 1
	gLights[5] = std::make_shared<Odyssey::Light>();
	gLights[5]->mLightType = Odyssey::LightType::Point;
	gLights[5]->mWorldPosition = DirectX::XMFLOAT4(5.45f, 4.75f, 14.4f, 1.0f);
	gLights[5]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[5]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[5]->mIntensity = 2.0f;
	gLights[5]->mRange = 5.0f;
	gLights[5]->mSpotAngle = 0.0f;

	// World-Space Right Pillar 2
	gLights[6] = std::make_shared<Odyssey::Light>();
	gLights[6]->mLightType = Odyssey::LightType::Point;
	gLights[6]->mWorldPosition = DirectX::XMFLOAT4(5.45f, 4.75f, 7.5f, 1.0f);
	gLights[6]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[6]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[6]->mIntensity = 2.0f;
	gLights[6]->mRange = 5.0f;
	gLights[6]->mSpotAngle = 0.0f;

	// World-Space Right Pillar 3
	gLights[7] = std::make_shared<Odyssey::Light>();
	gLights[7]->mLightType = Odyssey::LightType::Point;
	gLights[7]->mWorldPosition = DirectX::XMFLOAT4(5.45f, 4.75f, -13.22f, 1.0f);
	gLights[7]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[7]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[7]->mIntensity = 2.0f;
	gLights[7]->mRange = 5.0f;
	gLights[7]->mSpotAngle = 0.0f;

	// World-Space Left Door Light 1
	gLights[8] = std::make_shared<Odyssey::Light>();
	gLights[8]->mLightType = Odyssey::LightType::Point;
	gLights[8]->mWorldPosition = DirectX::XMFLOAT4(-12.0f, 4.75f, -6.7f, 1.0f);
	gLights[8]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[8]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[8]->mIntensity = 2.0f;
	gLights[8]->mRange = 5.0f;
	gLights[8]->mSpotAngle = 0.0f;

	// World-Space Left Door Light 2
	gLights[9] = std::make_shared<Odyssey::Light>();
	gLights[9]->mLightType = Odyssey::LightType::Point;
	gLights[9]->mWorldPosition = DirectX::XMFLOAT4(-12.0f, 4.75f, 1.2f, 1.0f);
	gLights[9]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[9]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[9]->mIntensity = 2.0f;
	gLights[9]->mRange = 5.0f;
	gLights[9]->mSpotAngle = 0.0f;

	// World-Space Right Door Light 1
	gLights[10] = std::make_shared<Odyssey::Light>();
	gLights[10]->mLightType = Odyssey::LightType::Point;
	gLights[10]->mWorldPosition = DirectX::XMFLOAT4(12.74f, 5.0f, -2.85f, 1.0f);
	gLights[10]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[10]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[10]->mIntensity = 2.0f;
	gLights[10]->mRange = 5.0f;
	gLights[10]->mSpotAngle = 0.0f;

	// World-Space Right Door Light 2
	gLights[11] = std::make_shared<Odyssey::Light>();
	gLights[11]->mLightType = Odyssey::LightType::Point;
	gLights[11]->mWorldPosition = DirectX::XMFLOAT4(12.74f, 5.0f, 4.25f, 1.0f);
	gLights[11]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[11]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[11]->mIntensity = 2.0f;
	gLights[11]->mRange = 5.0f;
	gLights[11]->mSpotAngle = 0.0f;

	// Library-Area Candle Light
	gLights[12] = std::make_shared<Odyssey::Light>();
	gLights[12]->mLightType = Odyssey::LightType::Point;
	gLights[12]->mWorldPosition = DirectX::XMFLOAT4(-1.25f, 12.5f, -35.0f, 1.0f);
	gLights[12]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gLights[12]->mColor = DirectX::XMFLOAT4(0.8f, 0.5f, 0.4f, 1.0f);
	gLights[12]->mIntensity = 2.0f;
	gLights[12]->mRange = 12.5f;

	// Arena-Paladin Spotlight
	gLights[13] = std::make_shared<Odyssey::Light>();
	gLights[13]->mLightType = Odyssey::LightType::Spot;
	gLights[13]->mWorldPosition = DirectX::XMFLOAT4(-1.0f, 5.0f, 5.0f, 1.0f);
	gLights[13]->mWorldDirection = DirectX::XMFLOAT4(0.1f, -0.95f, -0.25f, 0.0f);
	gLights[13]->mColor = DirectX::XMFLOAT4(0.3f, 0.3f, 0.8f, 1.0f);
	gLights[13]->mIntensity = 2.0f;
	gLights[13]->mRange = 20.0f;
	gLights[13]->mSpotAngle = 0.75f;

	// Arena-Skeleton Spotlight
	gLights[14] = std::make_shared<Odyssey::Light>();
	gLights[14]->mLightType = Odyssey::LightType::Spot;
	gLights[14]->mWorldPosition = DirectX::XMFLOAT4(-1.0f, 5.0f, -8.0f, 1.0f);
	gLights[14]->mWorldDirection = DirectX::XMFLOAT4(0.1f, -0.95f, -0.25f, 0.0f);
	gLights[14]->mColor = DirectX::XMFLOAT4(0.8f, 0.3f, 0.3f, 1.0f);
	gLights[14]->mIntensity = 2.0f;
	gLights[14]->mRange = 20.0f;
	gLights[14]->mSpotAngle = 0.75f;

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
	gMainScene->addLight(gLights[13]);
	gMainScene->addLight(gLights[14]);
}

void setupArena()
{
	gArena = std::make_shared<Odyssey::GameObject>();
	gArena->addComponent<Odyssey::Transform>();
	Odyssey::FileManager::getInstance().importModel(gArena, "assets/models/TestArena.dxm");
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
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Paladin_Idle.dxanim");
	gPaladin->addComponent<HeroComponent>();
	gPaladin->getComponent<HeroComponent>()->SetName("Paladin");
	//Buffs* paladinBuff = new Buffs(0, 0.05f, 2, true);
	//Skills* skill1 = new Skills(5.0f, 5.0f, paladinBuff);
	//gPaladin->getComponent<HeroComponent>()->SetSkills(*skill1);
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
	Odyssey::FileManager::getInstance().importModel(gSkeleton, "assets/models/Skeleton.dxm");
	gSkeleton->getComponent<Odyssey::Animator>()->importAnimation("Death", "assets/animations/Skeleton_Idle.dxanim");
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
	RedAudioManager::Instance()->AddAudio("assets/audio/battle_music.mp3", "Background");
	RedAudioManager::Instance()->Loop("Background");

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