#pragma once
#include "MenuScreen.h"
#include <vector>
#include "Application.h"

class MenuManager
{

public: // Constructors
	static MenuManager& GetInstance();
	~MenuManager() { }

private:
	MenuManager() { }

public: // Functions
	void initialize(Odyssey::Application* application);
	//Getters
	void loadScene(std::string sceneName);

	//Setters
private: // Varibales
	Odyssey::Application* mApplication;

	// Vectors
	std::vector<MenuScreen*> mMenuList;

	// GameObjects


	// Ints


	// Floats


	// Bools


private: // Functions
	MenuScreen* CreateMenuScreen(std::string _menuName);
	void AddMenuScreenToList(MenuScreen* _menuScreenToAdd);

};