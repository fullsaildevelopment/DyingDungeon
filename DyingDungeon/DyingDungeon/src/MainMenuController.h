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
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
private:
	void EnterTowerSelectScreen();
	// TODO: M3B1 ONLY REFACTOR LATER
	void ExitGame();
	// TODO: M3B1 ONLY END
public:
	Odyssey::Rectangle2D* mRect = nullptr;
private:
	Odyssey::Application* mApplication = nullptr;

	// Paladin Character
	Odyssey::Entity* mPaladinCharacter = nullptr;

	// Bools
	bool mAnimatingLaser = true;
};