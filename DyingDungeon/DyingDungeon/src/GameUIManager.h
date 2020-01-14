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

	// Get the rectangle buttons
	Odyssey::Rectangle2D* GetResumeButton() { return mResumeBackground; }
	Odyssey::Rectangle2D* GetOptionsButton() { return mOptionsBackground; }
	Odyssey::Rectangle2D* GetMainMenuButton() { return mMainMenuBackground; }

	//Setters
	void SetPauseMenu(std::shared_ptr<Odyssey::Entity> _pauseMenu) { mPauseMenu = _pauseMenu; }

	void SetScreenWidthAndHeight(UINT _width, UINT _height) { screenWidth = _width; screenHeight = _height; }
	
private: // Varibales

	// Canvases
	std::shared_ptr<Odyssey::Entity> mPauseMenu;

	// Pause Menu Items
	Odyssey::Rectangle2D* mBlackBackground;
	Odyssey::Rectangle2D* mSmallerBlackBackground;
	Odyssey::Text2D* mPauseTitle;
	Odyssey::Rectangle2D* mResumeBackground;
	Odyssey::Text2D* mResumeText;
	Odyssey::Rectangle2D* mOptionsBackground;
	Odyssey::Text2D* mOptionsText;
	Odyssey::Rectangle2D* mMainMenuBackground;
	Odyssey::Text2D* mMainMenuText;

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
