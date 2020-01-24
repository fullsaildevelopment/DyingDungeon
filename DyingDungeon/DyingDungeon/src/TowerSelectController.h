#pragma once
#include "Component.h"
#include "Rectangle2D.h"
#include "Application.h"
#include "Rectangle2D.h"

class TowerSelectController : public Odyssey::Component
{
	CLASS_DECLARATION(TowerSelectController)
public:
	TowerSelectController(Odyssey::Application* application);
	virtual void initialize();
	virtual void update(double deltaTime);
public:
	Odyssey::Rectangle2D* mRect;
private:
	Odyssey::Application* mApplication;

	// Bools
	bool mDoorIsClosed = true;
	bool mDoOpenDoorAnimation = false;
	bool mDoCloseDoorAnimation = false;

	// Ints
	int mDoorImageIndex = 1;

	// Double
	double totalTime = 0.0f;

private: //Functions
	void GoToTeamSelection();
	void ChangeDoorState();

	void SetNextDoorImage();
};

