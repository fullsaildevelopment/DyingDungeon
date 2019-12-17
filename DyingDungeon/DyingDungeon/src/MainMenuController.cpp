#include "MainMenuController.h"
#include "InputManager.h"

CLASS_DEFINITION(Odyssey::Component, MainMenuController)
MainMenuController::MainMenuController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void MainMenuController::initialize()
{

}

void MainMenuController::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyDown(VK_RETURN))
	{
		mApplication->setActiveScene("Game");
	}
	if (Odyssey::InputManager::getInstance().getKeyPress(VK_SPACE))
	{
		mRect->addFill(-deltaTime);
	}
}
