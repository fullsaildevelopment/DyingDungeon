#pragma once
#include <string>

class MenuScreen
{

public: // Constructors
	MenuScreen(std::string _menuName);
	virtual ~MenuScreen();

public: // Functions

	//Getters
	std::string GetMenuName() { return mMenuName; }

	//Setters
	void SetMenuName(std::string _newMenuName) { mMenuName = _newMenuName; }

private: // Varibales

	// Vectors


	// GameObjects


	// Strings
	std::string mMenuName;

	// Ints


	// Floats


	// Bools


private: // Functions

};