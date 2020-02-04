#pragma once
#include "Component.h"
#include "Rectangle2D.h"
#include "Application.h"

class TeamSelectionController : public Odyssey::Component
{
	CLASS_DECLARATION(TeamSelectionController)
public:
	TeamSelectionController(Odyssey::Application* application);
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestory();
	// TODO: REFACTOR THIS LATER
	void setupHovers();
public:
	Odyssey::Rectangle2D* mRect;

	// Functions

	// Setters

	// Set the list of scenes that the characters will need to be added to
	void SetGameScenes(std::vector<std::shared_ptr<Odyssey::Scene>> _listOfScenes) { mListOfGameScenes = _listOfScenes; }

	// Set current tower manager
	void SetTowerManager(std::shared_ptr<Odyssey::Entity> _towerManager) {mCurrentTower = _towerManager; }

	// Set turn indicator model
	void SetTurnIndicator(std::shared_ptr<Odyssey::Entity> _turnIndicatorModel) { mTurnIndicatorModel = _turnIndicatorModel; }

	// Set the slot 1 of characters for team selection
	void SetSlot1OfCharacters(std::vector<std::shared_ptr<Odyssey::Entity>> _listOfCharacters) {
		mSlot1CharacterList = _listOfCharacters;
		int debug = 0;
	}
	void SetSlot2OfCharacters(std::vector<std::shared_ptr<Odyssey::Entity>> _listOfCharacters) { mSlot2CharacterList = _listOfCharacters; }
	void SetSlot3OfCharacters(std::vector<std::shared_ptr<Odyssey::Entity>> _listOfCharacters) { mSlot3CharacterList = _listOfCharacters; }

private:
	Odyssey::Application* mApplication;
	Odyssey::UICanvas* mPaladin;
	Odyssey::UICanvas* mMage;
	// This will let us know when the user has a clicked the enter battle button
	bool mEnterBattle = true;
	// Only allow to change scene once
	bool changedTheScene = false;
	// Building Index will keep track of how many charcters have been built
	int mBuildIndex = 0;

	// Game scenes that the players will need to get added to
	std::vector<std::shared_ptr<Odyssey::Scene>> mListOfGameScenes;

	// List of all of the positions that the character's will spawn at in battle
	std::vector<DirectX::XMVECTOR> mPlayerPositions;

	// List of all of the hud and health popup positions for each character
	std::vector<DirectX::XMFLOAT2> mHudPositions;
	std::vector<DirectX::XMFLOAT2> mHpPopupPositions;

	// Hold the tower manager object
	std::shared_ptr<Odyssey::Entity> mCurrentTower;
	// Hold the turn indicator model
	std::shared_ptr<Odyssey::Entity> mTurnIndicatorModel;

	// These vectors will hold the character list for each slot
	std::vector<std::shared_ptr<Odyssey::Entity>> mSlot1CharacterList;
	std::vector<std::shared_ptr<Odyssey::Entity>> mSlot2CharacterList;
	std::vector<std::shared_ptr<Odyssey::Entity>> mSlot3CharacterList;

	// Character Slot Indexes
	int mSlot1Index = 0;
	int mSlot2Index = 0;
	int mSlot3Index = 0;

private: // Functions
	// TODO: REFACTOR THIS LATER
	void onPaladinEnter();
	void onPaladinExit();
	void onMageEnter();
	void onMageExit();
	void setupCharacterHover(Odyssey::UICanvas* canvas, std::wstring character);

	// Callback function for the enter battle button
	void EnterBattle();
	void CreateCharacterBasedOnName(std::wstring _name);

	// Callback functions for the arrows
	void DecreaseSlot1Index();
	void IncreaseSlot1Index();
	void DecreaseSlot2Index();
	void IncreaseSlot2Index();
	void DecreaseSlot3Index();
	void IncreaseSlot3Index();
};
