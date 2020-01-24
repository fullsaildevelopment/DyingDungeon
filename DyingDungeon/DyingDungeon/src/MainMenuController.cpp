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
	RedAudioManager::Instance().Loop("BackgroundMenu");
}

void MainMenuController::update(double deltaTime)
{
	static double totalTime = 0;
	totalTime += deltaTime;
	static bool madeItPassedLogo = false;
	double waitTime = 3.0f; 
	 
	if (totalTime >= waitTime)
	{
		if(!madeItPassedLogo)
		{
			// Turn off logo image
			GameUIManager::getInstance().GetTeamLogo()->setVisible(false);
			// Register callback for new game text
			GameUIManager::getInstance().GetNewGameText()->registerCallback("onMouseClick", this, &MainMenuController::EnterTowerSelectScreen);
			GameUIManager::getInstance().GetStatsText()->registerCallback("onMouseClick", &GameUIManager::getInstance(), &GameUIManager::DisplayStatsMenu);
			madeItPassedLogo = true;	
		}

		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
		{
			//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
			//RedAudioManager::Instance().Play("BackgroundBattle");
			Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TowerSelection"));
		}
	}
}

void MainMenuController::EnterTowerSelectScreen()
{
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TowerSelection"));
}
