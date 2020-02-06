#pragma once
#include "Component.h"
#include "Rectangle2D.h"
#include "Application.h"
#include "Rectangle2D.h"

class MainMenuController : public Odyssey::Component
{
	CLASS_DECLARATION(MainMenuController)
public:
	MainMenuController(Odyssey::Application* application);
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
private:
	void EnterTowerSelectScreen();
	// TODO: M3B1 ONLY REFACTOR LATER
	void ExitGame();
	// TODO: M3B1 ONLY END
public:
	Odyssey::Rectangle2D* mRect;
private:
	Odyssey::Application* mApplication;

	// Bools
	bool mAnimatingLaser = true;
};