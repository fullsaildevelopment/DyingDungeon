#include "MainMenuController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"

CLASS_DEFINITION(Odyssey::Component, MainMenuController)
MainMenuController::MainMenuController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void MainMenuController::initialize()
{
	RedAudioManager::Instance().Loop("BackgroundMenu");
}

void MainMenuController::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
	{
		//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
		//RedAudioManager::Instance().Play("BackgroundBattle");
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TowerSelection"));
	}
}
