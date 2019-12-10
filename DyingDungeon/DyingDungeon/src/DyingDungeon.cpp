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
    // Light resources
    std::shared_ptr<Odyssey::Light> gDirLight;
    std::shared_ptr<Odyssey::Light> gPointLight[10];
}

// Forward declarations
int playGame();
void setupPipeline(Odyssey::RenderDevice* renderDevice);
void setupLighting();
void setupArena();
void setupPaladin();

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
	gDirLight->mLightType = Odyssey::LightType::Directional;
	gDirLight->mWorldPosition = DirectX::XMFLOAT4(0, 0, 0, 1);
	gDirLight->mWorldDirection = DirectX::XMFLOAT4(0.0f, -0.5f, -0.5f, 0.0f);
	gDirLight->mColor = DirectX::XMFLOAT4(0.4f, 0.5f, 0.6f, 1.0f);
	gDirLight->mIntensity = 1.0f;
	gDirLight->mRange = 0.0f;
	gDirLight->mSpotAngle = 0.0f;

	gPointLight[0] = std::make_shared<Odyssey::Light>();
	gPointLight[0]->mLightType = Odyssey::LightType::Point;
	gPointLight[0]->mWorldPosition = DirectX::XMFLOAT4(0.0f, 20.0f, 0.0f, 1.0f);
	gPointLight[0]->mWorldDirection = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	gPointLight[0]->mColor = DirectX::XMFLOAT4(0.9f, 0.7f, 0.7f, 1.0f);
	gPointLight[0]->mIntensity = 1.0f;
	gPointLight[0]->mRange = 50.0f;
	gPointLight[0]->mSpotAngle = 0.0f;

	gPointLight[1] = std::make_shared<Odyssey::Light>();
	gPointLight[1]->mLightType = Odyssey::LightType::Spot;
	gPointLight[1]->mWorldPosition = DirectX::XMFLOAT4(0.0f, 20.0f, -13.0f, 1.0f);
	gPointLight[1]->mWorldDirection = DirectX::XMFLOAT4(0.0f, -1.0f, 0.15f, 0.0f);
	gPointLight[1]->mColor = DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f);
	gPointLight[1]->mIntensity = 5.0f;
	gPointLight[1]->mRange = 100.0f;
	gPointLight[1]->mSpotAngle = 0.1f;

	gMainScene->addLight(gDirLight);
	gMainScene->addLight(gPointLight[0]);
	gMainScene->addLight(gPointLight[1]);
}

void setupArena()
{
	gArena = std::make_shared<Odyssey::GameObject>();
	Odyssey::FileManager::getInstance().importModel(gArena, "assets/models/TestArena.dxm");
	gArena->addComponent<ExampleComponent>();
	gMainScene->addSceneObject(gArena);
}

void setupPaladin()
{
	gPaladin = std::make_shared<Odyssey::GameObject>();
	gPaladin->addComponent<Odyssey::Transform>();
	gPaladin->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	gPaladin->getComponent<Odyssey::Transform>()->setPosition(0.0f, -0.5f, -10.0f);
	Odyssey::FileManager::getInstance().importModel(gPaladin, "assets/models/Paladin.dxm");
	gPaladin->getComponent<Odyssey::Animator>()->importAnimation("Judgement", "assets/animations/Paladin_Judgement.dxanim");
	gMainScene->addSceneObject(gPaladin);
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

	// Set up Battle Instance

	// Set the initial view and projection matrix
	gMainScene->mMainCamera.setPosition(0, 0, 0);
	gMainScene->mMainCamera.setProjectionValues(60.0f, gMainWindow->getAspectRatio(), 0.1f, 100.0f);

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