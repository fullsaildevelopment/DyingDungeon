#pragma once
#include "Component.h"
#include "Rectangle2D.h"
#include "Application.h"
#include "Rectangle2D.h"
#include "Sprite2D.h"

class TowerSelectController : public Odyssey::Component
{
	CLASS_DECLARATION(TowerSelectController)
public:
	TowerSelectController(Odyssey::Application* application);
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
public:
	Odyssey::Rectangle2D* mRect;
private:
	Odyssey::Application* mApplication;

	struct Door
	{
		// The door image
		Odyssey::Sprite2D* doorImage;

		// Bools
		bool mDoorIsClosed = true;
		bool mDoOpenDoorAnimation = false;
		bool mDoCloseDoorAnimation = false;

		// Ints
		int mDoorImageIndex = 1;

		// Double
		double totalTime = 0.0f;
	};

	// Keep a list of the doors
	std::vector<Door> mDoorList;


private: //Functions
	void GoToTeamSelection();
	void GoToScene2();

	// Change the door states on mouse enter and exit events
	void ChangeDoor1State();
	void ChangeDoor2State();
	void ChangeDoor3State();
	void ChangeDoor4State();
	void ChangeDoor5State();

	// Door Animation functions
	void Door1Animation(double _deltaTime);
	void Door2Animation(double _deltaTime);
	void Door3Animation(double _deltaTime);
	void Door4Animation(double _deltaTime);
	void Door5Animation(double _deltaTime);

	void SetNextDoorImage(Door _doorToChange);
};

