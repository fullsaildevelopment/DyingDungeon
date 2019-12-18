#include "MenuManager.h"

MenuManager& MenuManager::GetInstance()
{
	// TODO: insert return statement here
	static MenuManager instance;
	return instance;
}

void MenuManager::initialize(Odyssey::Application* application)
{
	mApplication = application;
}

void MenuManager::loadScene(std::string sceneName)
{
	mApplication->setActiveScene(sceneName);
}