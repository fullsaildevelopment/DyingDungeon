#pragma once
#include "Component.h"
#include "Rectangle2D.h"
#include "Application.h"
#include "Rectangle2D.h"

class TeamSelectionController : public Odyssey::Component
{
	CLASS_DECLARATION(TeamSelectionController)
public:
	TeamSelectionController(Odyssey::Application* application);
	virtual void initialize();
	virtual void update(double deltaTime);
	// TODO: REFACTOR THIS LATER
	void setupHovers();
public:
	Odyssey::Rectangle2D* mRect;

	// Functions
	void SetGameScenes(std::vector<std::shared_ptr<Odyssey::Scene>> _listOfScenes) { mListOfGameScenes = _listOfScenes; }

private:
	Odyssey::Application* mApplication;
	Odyssey::UICanvas* mPaladin;
	Odyssey::UICanvas* mMage;
	// Set the number of members on your team currently
	int teamCount = 0;
	// This will let us know when the user has a full team
	bool teamIsFull = true;

	// Only allow to change scene once
	bool changedTheScene = false;

	// Game scenes that the players will need to get added to
	std::vector<std::shared_ptr<Odyssey::Scene>> mListOfGameScenes;

private: // Functions
	void AddPaladinImage();
	void AddMageImage();
	// TODO: REFACTOR THIS LATER
	void onPaladinEnter();
	void onPaladinExit();
	void onMageEnter();
	void onMageExit();
	void setupCharacterHover(Odyssey::UICanvas* canvas, std::wstring character);
};
