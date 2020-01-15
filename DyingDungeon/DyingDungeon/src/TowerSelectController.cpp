#include "TowerSelectController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "UICanvas.h"

CLASS_DEFINITION(Odyssey::Component, TowerSelectController)
TowerSelectController::TowerSelectController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void TowerSelectController::initialize()
{
	//RedAudioManager::Instance().Play("BackgroundMenu");
	// Turn on the tower select canvas
	std::shared_ptr<Odyssey::Entity> towerSelectMenu = GameUIManager::getInstance().GetTowerSelectMenu();
	GameUIManager::getInstance().ToggleCanvas(towerSelectMenu->getComponent<Odyssey::UICanvas>(), true);

	// Make the door switch to the team selection scene when clicked
	GameUIManager::getInstance().GetDoorButton()->registerCallback("onMouseClick", this, &TowerSelectController::GoToTeamSelection);
}

void TowerSelectController::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
	{
		//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
		//RedAudioManager::Instance().Play("BackgroundBattle");
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TeamSelection"));
	}
}

void TowerSelectController::GoToTeamSelection()
{
	// Turn off the tower select canvas
	std::shared_ptr<Odyssey::Entity> towerSelectMenu = GameUIManager::getInstance().GetTowerSelectMenu();
	GameUIManager::getInstance().ToggleCanvas(towerSelectMenu->getComponent<Odyssey::UICanvas>(), false);

	// Switch to the team select scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TeamSelection"));
}
