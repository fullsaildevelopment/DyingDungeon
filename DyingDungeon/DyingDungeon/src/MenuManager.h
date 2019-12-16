#pragma once
#include "MenuScreen.h"
#include <vector>

class MenuManager
{

public: // Constructors
	MenuManager() = default;
	virtual ~MenuManager();

public: // Functions


	//Getters


	//Setters


private: // Varibales

	// Vectors
	std::vector<MenuScreen*> mMenuList;

	// GameObjects


	// Ints


	// Floats


	// Bools


private: // Functions
	MenuScreen* CreateMenuScreen(std::string _menuName);

};