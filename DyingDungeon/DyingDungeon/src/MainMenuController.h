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
public:
	Odyssey::Rectangle2D* mRect;
private:
	Odyssey::Application* mApplication;
};