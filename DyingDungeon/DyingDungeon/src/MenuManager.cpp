#include "MenuManager.h"

//MenuManager::MenuManager()
//{
//
//}

MenuManager::~MenuManager()
{

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