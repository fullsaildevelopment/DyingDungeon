#include "MainMenuController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"

CLASS_DEFINITION(Odyssey::Component, MainMenuController)
MainMenuController::MainMenuController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void MainMenuController::initialize()
{
	RedAudioManager::Instance().Stop("BackgroundBattle");
	RedAudioManager::Instance().Stop("TorchBurningQuietly");
	RedAudioManager::Instance().LoopRandom("BackgroundMenu");

	// Register callbacks
	GameUIManager::getInstance().GetNewGameText()->registerCallback("onMouseClick", this, &MainMenuController::EnterTowerSelectScreen);
	GameUIManager::getInstance().GetStatsText()->registerCallback("onMouseClick", &GameUIManager::getInstance(), &GameUIManager::ShowStatsMenu);

	// Animating bool
	mAnimatingLaser = true;
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
		float fillRatio = totalTime / (waitTime - 0.5f); // Give some padding

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
}

void MainMenuController::EnterTowerSelectScreen()
{
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TowerSelection"));
}
