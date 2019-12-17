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

MenuScreen* MenuManager::CreateMenuScreen(std::string _menuName)
{
	// Create the new menu screen object
	MenuScreen* newMenuScreen = new MenuScreen(_menuName);

	// Return the newly created menu
	return newMenuScreen;
}

void MenuManager::AddMenuScreenToList(MenuScreen* _menuScreenToAdd)
{
	// Add the menu screen to the list of menu screens in mMenuList
	mMenuList.push_back(_menuScreenToAdd);
}