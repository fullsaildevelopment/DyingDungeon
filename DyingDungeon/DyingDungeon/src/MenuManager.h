#pragma once
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

	// GameObjects


	// Ints


	// Floats


	// Bools


private: // Functions

};