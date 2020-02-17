#pragma once
#include "Component.h"
#include "Rectangle2D.h"
#include "Application.h"

class TeamSelectionController : public Odyssey::Component
{
	CLASS_DECLARATION(TeamSelectionController)
public:
	TeamSelectionController(Odyssey::Application* application);
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
	// TODO: REFACTOR THIS LATER
	void setupHovers();
public:
	Odyssey::Rectangle2D* mRect;

	// Functions

	// Setters

	// Set the list of scenes that the characters will need to be added to
	void SetGameScenes(std::vector<Odyssey::Scene*> _listOfScenes) { mListOfGameScenes = _listOfScenes; }

	// Set current tower manager
	void SetTowerManager(Odyssey::Entity* _towerManager) {mCurrentTower = _towerManager; }

	// Set turn indicator model
	void SetTurnIndicator(Odyssey::Entity* _turnIndicatorModel) { mTurnIndicatorModel = _turnIndicatorModel; }

	// Set the slot 1 of characters for team selection
	void SetSlot1OfCharacters(std::vector<Odyssey::Entity*> _listOfCharacters) { mSlot1CharacterList = _listOfCharacters; }
	// Set the slot 2 of characters for team selection
	void SetSlot2OfCharacters(std::vector<Odyssey::Entity*> _listOfCharacters) { mSlot2CharacterList = _listOfCharacters; }
	// Set the slot 3 of characters for team selection
	void SetSlot3OfCharacters(std::vector<Odyssey::Entity*> _listOfCharacters) { mSlot3CharacterList = _listOfCharacters; }

	// Set the slot 
	void SetSlot1CharacterInfoPopups(std::vector<Odyssey::UICanvas*> _listOfPopupCanvases) { mSlot1CharacterInfoPopupList = _listOfPopupCanvases; }
	void SetSlot2CharacterInfoPopups(std::vector<Odyssey::UICanvas*> _listOfPopupCanvases) { mSlot2CharacterInfoPopupList = _listOfPopupCanvases; }
	void SetSlot3CharacterInfoPopups(std::vector<Odyssey::UICanvas*> _listOfPopupCanvases) { mSlot3CharacterInfoPopupList = _listOfPopupCanvases; }

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
	std::vector<Odyssey::Scene*> mListOfGameScenes;

	// List of all of the positions that the character's will spawn at in battle
	std::vector<DirectX::XMVECTOR> mPlayerPositions;

	// List of all of the hud and health popup positions for each character
	std::vector<DirectX::XMFLOAT2> mHudPositions;
	std::vector<DirectX::XMFLOAT2> mHpPopupPositions;

	// Hold the tower manager object
	Odyssey::Entity* mCurrentTower;
	// Hold the turn indicator model
	Odyssey::Entity* mTurnIndicatorModel;

	// These vectors will hold the character list for each slot
	std::vector<Odyssey::Entity*> mSlot1CharacterList;
	std::vector<Odyssey::Entity*> mSlot2CharacterList;
	std::vector<Odyssey::Entity*> mSlot3CharacterList;

	// These vectors will hold the characters info popup
	std::vector<Odyssey::UICanvas*> mSlot1CharacterInfoPopupList;
	std::vector<Odyssey::UICanvas*> mSlot2CharacterInfoPopupList;
	std::vector<Odyssey::UICanvas*> mSlot3CharacterInfoPopupList;

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

	// Callback functions for the show info buttons
	void ToggleShowInfoPopup1();
	void ToggleShowInfoPopup2();
	void ToggleShowInfoPopup3();

	//Callback functions for the loadout buttons
	void ShowLoadLoadoutMenu();
	void HideLoadLoadoutMenu();

	void SaveLoadout1();
	void SaveLoadout2();
	void SaveLoadout3();

	void LoadLoadout1();
	void LoadLoadout2();
	void LoadLoadout3();

	void ConfermationSave1();

	void ConfermationSave2();

	void ConfermationSave3();

	void ConfermationNo();

	// Change the name in the slots
	void ChangeSlotName(int _slotIndex, std::wstring _name);
	void onHoverLoadout1();
	void onExitHoverLoadout1();
	void onHoverLoadout2();
	void onExitHoverLoadout2();
	void onHoverLoadout3();
	void onExitHoverLoadout3();
	void ShowSaveLoadoutMenu();
};
