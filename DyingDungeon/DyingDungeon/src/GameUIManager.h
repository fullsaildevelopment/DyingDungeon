#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Rectangle2D.h"
#include "Text2D.h"

class GameUIManager
{
public: // Singleton pattern
		/**
		 *	Get the singleton instance of the game ui manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the game ui manager.
		 */
	static GameUIManager& getInstance();
	~GameUIManager() { }
private: // Singleton pattern
	GameUIManager() { }

public: // Functions

	// Toggle canvases on and off
	void ToggleCanvas(std::shared_ptr<Odyssey::Entity> _canvas, bool _isActive);

	// Creation of the pause menu
	void CreatePauseMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	//Getters
	std::shared_ptr<Odyssey::Entity> GetPauseMenu() { return mPauseMenu; }

	//Setters
	void SetPauseMenu(std::shared_ptr<Odyssey::Entity> _pauseMenu) { mPauseMenu = _pauseMenu; }

	void SetScreenWidthAndHeight(UINT _width, UINT _height) { screenWidth = _width; screenHeight = _height; }
	
private: // Varibales

	// Canvases
	std::shared_ptr<Odyssey::Entity> mPauseMenu;

	// Pause Menu Items
	Odyssey::Rectangle2D* blackBackground;
	Odyssey::Rectangle2D* smallerBlackBackground;
	Odyssey::Text2D* pauseTitle;

	// Vectors

	// Queues

	// Entities

	// Ints
	UINT screenWidth = 0;
	UINT screenHeight = 0;
	// Floats

	// Bools

private: // Functions
	void ToggleTitle();
};
