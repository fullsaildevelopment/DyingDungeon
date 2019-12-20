#include "MainMenuController.h"
#include "InputManager.h"
#include "RedAudioManager.h"

CLASS_DEFINITION(Odyssey::Component, MainMenuController)
MainMenuController::MainMenuController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void MainMenuController::initialize()
{
	RedAudioManager::Instance().Play("BackgroundMenu");
}

void MainMenuController::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(VK_RETURN))
	{
		RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
		RedAudioManager::Instance().Play("BackgroundBattle");
		mApplication->setActiveScene("Game");
	}
}
