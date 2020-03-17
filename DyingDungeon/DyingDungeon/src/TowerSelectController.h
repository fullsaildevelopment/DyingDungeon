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

	// Set the current tower manager
	void SetTowerManager(Odyssey::Entity* _towerManager) { mCurrentTower = _towerManager; }

public:
	Odyssey::Rectangle2D* mRect = nullptr;
private:
	Odyssey::Application* mApplication = nullptr;

	// Current info popup objects
	Odyssey::Entity* mLevelInfoPopups[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

	struct Door
	{
		// The door image
		Odyssey::Sprite2D* doorImage = nullptr;

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

	// Current Tower Manager Object
	Odyssey::Entity* mCurrentTower = nullptr;

private: //Functions
	void GoToTeamSelectionWithLevel1();
	void GoToTeamSelectionWithLevel2();
	void GoToTeamSelectionWithLevel3();
	void GoToTeamSelectionWithLevel4();
	void GoToTeamSelectionWithLevel5();

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
	void GoBackToMainMenu();
	// Change tower info elements
	void ChangeTowerInfoElements(Odyssey::Entity* _newPrefab, int _levelNum);
};

