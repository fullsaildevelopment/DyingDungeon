#pragma once
#include "Component.h"
#include "UICanvas.h"
#include "Text2D.h"

class GameManagerUI : public Odyssey::Component
{
	CLASS_DECLARATION(GameManagerUI);

public: // Constructors
	GameManagerUI() = default;
	virtual ~GameManagerUI();

	std::shared_ptr<Odyssey::UICanvas> mPauseMenu;

public: // Functions

	// Necessary
	virtual void initialize();
	virtual void update(double deltaTime);

	//Getters

	//Setters
	
private: // Varibales

	// Vectors

	// Queues

	// Entities

	// Ints

	// Floats

	// Bools

	// Canvases

private: // Functions

};
