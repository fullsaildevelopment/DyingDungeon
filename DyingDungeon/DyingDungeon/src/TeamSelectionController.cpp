#include "TeamSelectionController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"

CLASS_DEFINITION(Odyssey::Component, TeamSelectionController)
TeamSelectionController::TeamSelectionController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void TeamSelectionController::initialize()
{
	//RedAudioManager::Instance().Loop("BackgroundMenu");
	//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
}

void TeamSelectionController::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
	{
		RedAudioManager::Instance().Stop("BackgroundMenu");
		RedAudioManager::Instance().Loop("BackgroundBattle");
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Game"));
	}
}