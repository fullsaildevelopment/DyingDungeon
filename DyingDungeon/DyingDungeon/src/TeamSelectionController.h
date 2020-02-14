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

	// Set current tower manager
	void SetTowerManager(Odyssey::Entity* _towerManager) {mCurrentTower = _towerManager; }

private:
	Odyssey::Application* mApplication;
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

	// Create the prefab models with the info popups
	void CreateModelsAndPopups();

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

	// Change the name in the slots
	void ChangeSlotName(int _slotIndex, std::wstring _name);
};
