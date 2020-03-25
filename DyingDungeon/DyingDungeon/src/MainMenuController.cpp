#include "MainMenuController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "CharacterFactory.h"

CLASS_DEFINITION(Odyssey::Component, MainMenuController)
MainMenuController::MainMenuController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
	mPaladinCharacter = nullptr;
}

std::shared_ptr<Odyssey::Component> MainMenuController::clone() const
{
	return std::make_shared<MainMenuController>(*this);
}

void MainMenuController::initialize()
{
	RedAudioManager::Instance().StopGroup("BackgroundBattle");
	RedAudioManager::Instance().Stop("TorchBurningQuietly");
	RedAudioManager::Instance().LoopRandom("BackgroundMenu");

	// Register callbacks
	GameUIManager::getInstance().GetNewGameText()->registerCallback("onMouseClick", this, &MainMenuController::EnterTowerSelectScreen);
	GameUIManager::getInstance().GetOptionsButtonMain()->registerCallback("onMouseClick", &GameUIManager::getInstance(), &GameUIManager::ShowMainOptions);
	
	// Set the new cursor
	Odyssey::EventManager::getInstance().publish(new Odyssey::ChangeMouseCursorEvent(L"assets/images/Cursor/Cursor_Basic.cur"));

	if (StatTracker::Instance().GetLevelSize() > 0) 
	{
		GameUIManager::getInstance().GetStatsText()->setColor(DirectX::XMFLOAT3(255.0f, 255.0f, 255.0f));
		GameUIManager::getInstance().GetStatsText()->registerCallback("onMouseClick", &GameUIManager::getInstance(), &GameUIManager::ShowStatsMenu);
	}
	else
	{
		GameUIManager::getInstance().GetStatsText()->setColor(DirectX::XMFLOAT3(124.5f, 124.5f, 124.5f));
	}
	// TODO: M3B1 ONLY REFACTOR LATER
	GameUIManager::getInstance().GetCreditsText()->registerCallback("onMouseClick", &GameUIManager::getInstance(), &GameUIManager::ShowCreditsMenu);
	GameUIManager::getInstance().GetExitGameText()->registerCallback("onMouseClick", this, &MainMenuController::ExitGame);
	// TODO: M3B1 ONLY END

	// Animating bool
	mAnimatingLaser = true;

	// Create a paladin and add him to the main menu scene
	mPaladinCharacter = nullptr;
	Odyssey::Entity* prefab = nullptr;
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(36.75f, 1.34f, 1.88f, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 90.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 uiPosition = { 0.0f, 0.0f };
	prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
	Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &mPaladinCharacter, charPosition, charRotation));
	//mPaladinCharacter->getChildren()[1]->setVisible(false);
}

void MainMenuController::update(double deltaTime)
{
	static double totalTime = 0;
	totalTime += deltaTime;
	static bool madeItPassedLogo = false;
	double waitTime = 3.0f; 

	// If we are animating, animate the sprite
	if (mAnimatingLaser)
	{
		// Set the fill of the logo based on the (totalTime / waitTime)
		float fillRatio = static_cast<float>(totalTime / (waitTime - 0.5f)); // Give some padding

		// If the fillRatio is at 100%, stop animating
		if (fillRatio >= 1.0f)
			mAnimatingLaser = false;

		// Set the image to the new fill ratio
		GameUIManager::getInstance().GetAnimatedLaser()->setFill(fillRatio);
	}
	 
	if (totalTime >= waitTime)
	{
		if(!madeItPassedLogo)
		{
			// Turn off logo image
			GameUIManager::getInstance().GetApeBackground()->setVisible(false);
			GameUIManager::getInstance().GetAnimatedLaser()->setVisible(false);
			madeItPassedLogo = true;	
		}
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::M))
	{
		if (!RedAudioManager::Instance().isMuted())
		{
			RedAudioManager::Instance().Mute();
		}
		else
		{
			RedAudioManager::Instance().Unmute();
		}
	}
}

void MainMenuController::onDestroy()
{
	// unregister callbacks
	GameUIManager::getInstance().GetNewGameText()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetStatsText()->unregisterCallback("onMouseClick");
	// TODO: M3B1 ONLY REFACTOR LATER
	GameUIManager::getInstance().GetCreditsText()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetExitGameText()->unregisterCallback("onMouseClick");

	for (int i = 0; i < 4; i++) {
		GameUIManager::getInstance().GetMainMenuPlusVolumeButtons()[i]->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetMainMenuMinusVolumeButtons()[i]->unregisterCallback("onMouseClick");
	}
	// TODO: M3B1 ONLY END
	GameUIManager::getInstance().GetOptionsButtonMain()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetBackButtonOptions()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetVolumeButtonOptions()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetControlsButtonOptions()->unregisterCallback("onMouseClick");
}

void MainMenuController::EnterTowerSelectScreen()
{
	// Destroy the paladin
	Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mPaladinCharacter));

	// Change to the tower selection screen
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TowerSelection"));
}

void MainMenuController::ExitGame()
{
	Odyssey::EventManager::getInstance().publish(new Odyssey::ShutdownApplicationEvent());
}

void MainMenuController::ShowVolumeConfermation()
{
	 //GameUIManager::getInstance().Get
}

void MainMenuController::VolumeConfermationYes()
{

}

void MainMenuController::VolumeConfermationNo()
{

}