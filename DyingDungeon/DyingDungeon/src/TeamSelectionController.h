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
public:
	// Functions

	// Setters

	// Add tower managers
	void AddTowerManager(Odyssey::Entity* _towerManager) { mCurrentTowers.push_back(_towerManager); }
	void SetIsBossScene(bool _isBossScene) { mIsBossScene = _isBossScene; }

private:
	Odyssey::Application* mApplication = nullptr;
	// This will let us know when the user has a clicked the enter battle button
	bool mEnterBattle = true;
	// Only allow to change scene once
	bool changedTheScene = false;
	// Building Index will keep track of how many charcters have been built
	int mBuildIndex = 0;

	// holds whether or not it's the boss scene
	bool mIsBossScene = false;

	// Game scenes that the players will need to get added to
	std::vector<Odyssey::Scene*> mListOfGameScenes;

	// List of all of the positions that the character's will spawn at in battle
	std::vector<DirectX::XMVECTOR> mPlayerPositions;

	// Hold the tower manager object
	std::vector<Odyssey::Entity*> mCurrentTowers;

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

	//REFACTOR
	Odyssey::Entity* loadingScreen = nullptr;

private: // Functions

	// Create the prefab models with the info popups
	void CreateModelsAndPopups();
	// Don't display some of the character
	void TurnOffModels();

	// Callback function for the enter battle button
	void EnterBattle();
	void AddCharacterTypeToPlayerTeam(std::wstring _characterName);

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

	void GoBackToTowerSelect();
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
