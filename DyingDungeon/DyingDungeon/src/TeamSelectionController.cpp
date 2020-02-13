#include "TeamSelectionController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "TeamManager.h"
#include "CharacterFactory.h"
#include "HeroComponent.h"
#include "Material.h"
#include "TowerManager.h"
#include "SaveLoad.h"

CLASS_DEFINITION(Odyssey::Component, TeamSelectionController)
TeamSelectionController::TeamSelectionController(Odyssey::Application* application)
{
	mApplication = application;
}

std::shared_ptr<Odyssey::Component> TeamSelectionController::clone() const
{
	return std::make_shared<TeamSelectionController>(*this);
}

void TeamSelectionController::initialize()
{
	// Reset bools and ints
	changedTheScene = false;
	mEnterBattle = false;
	mBuildIndex = 0;

	// Create the models and info popups
	CreateModelsAndPopups();

	// Register callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->registerCallback("onMouseClick", this, &TeamSelectionController::EnterBattle);

	// Register callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot3Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot3Index);

	// Register the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup1);
	GameUIManager::getInstance().GetShowInfoButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup2);
	GameUIManager::getInstance().GetShowInfoButtons()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup3);

	// Set the player positions
	mPlayerPositions.clear();
	mPlayerPositions.resize(3);
	mPlayerPositions[0] = DirectX::XMVectorSet(-5.0f, 0.0f, 10.0f, 1.0f); // First Character Selected
	mPlayerPositions[1] = DirectX::XMVectorSet(0.0f, 0.0f, 10.0f, 1.0f); // Second Character Selected
	mPlayerPositions[2] = DirectX::XMVectorSet(5.0f, 0.0f, 10.0f, 1.0f); // Third Character Selected

	// Set the HUD positions
	mHudPositions.clear();
	mHudPositions.resize(3);
	mHudPositions[0] = DirectX::XMFLOAT2(10.0f, 600.0f); // First Character HUD
	mHudPositions[1] = DirectX::XMFLOAT2(470.0f, 600.0f); // Second Character HUD
	mHudPositions[2] = DirectX::XMFLOAT2(910.0f, 600.0f); // Third Character HUD

	// Set the HP positions
	mHpPopupPositions.clear();
	mHpPopupPositions.resize(3);
	mHpPopupPositions[0] = DirectX::XMFLOAT2(325.0f, 350.0f); // First Character HP popup
	mHpPopupPositions[1] = DirectX::XMFLOAT2(615.0f, 350.0f); // Second Character HP popup
	mHpPopupPositions[2] = DirectX::XMFLOAT2(905.0f, 350.0f); // Third Character HP popup

	// Clear the player team from Team Manager before adding in new characters
	TeamManager::getInstance().ClearPlayerTeam();
	if (SaveLoad::Instance().LoadLoadOut("LoadOut_Test"))
	{
		mSlot1CharacterList[mSlot1Index]->setVisible(false);
		mSlot1Index = SaveLoad::Instance().GetLoadOut("LoadOut_Test").index[0];
		mSlot1CharacterList[mSlot1Index]->setVisible(true);
		ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());

		mSlot2CharacterList[mSlot2Index]->setVisible(false);
		mSlot2Index = SaveLoad::Instance().GetLoadOut("LoadOut_Test").index[1];
		mSlot2CharacterList[mSlot2Index]->setVisible(true);
		ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());
		
		mSlot3CharacterList[mSlot3Index]->setVisible(false);
		mSlot3Index = SaveLoad::Instance().GetLoadOut("LoadOut_Test").index[2];
		mSlot3CharacterList[mSlot3Index]->setVisible(true);
		ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());
	}
	//SaveLoad::Instance().LoadLoadOut();
}

void TeamSelectionController::update(double deltaTime)
{
	if (mEnterBattle && !changedTheScene)
	{
		changedTheScene = true;
		RedAudioManager::Instance().Stop("BackgroundMenu");
		RedAudioManager::Instance().Loop("TorchBurningQuietly");
		RedAudioManager::Instance().SetVolume("TorchBurningQuietly", 200);
		RedAudioManager::Instance().Loop("BackgroundBattle");

		// Set up the tower manager with how many levels we want
		mCurrentTower->getComponent<TowerManager>()->SetUpTowerManager(2);

		// Change the scene to the game
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Scene One"));
	}
	else if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::M))
	{
		if (!RedAudioManager::Instance().isMuted())
		{
			RedAudioManager::Instance().Mute();
		}
		else
		{
			RedAudioManager::Instance().Unmute();
		}
	}
	else if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::A) && Odyssey::InputManager::getInstance().getKeyPress(KeyCode::S))
	{
		GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
		unsigned int indecies[3] = { mSlot1Index, mSlot2Index, mSlot3Index };
		SaveLoad::Instance().AddLoadOut("LoadOut_Test", ids, indecies);
		SaveLoad::Instance().SaveLoadOut();
	}
}

void TeamSelectionController::onDestroy()
{
	// Unregister callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->unregisterCallback("onMouseClick");

	// Unregister callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->unregisterCallback("onMouseClick");

	// Unregister the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[2]->unregisterCallback("onMouseClick");
}

void TeamSelectionController::CreateModelsAndPopups()
{
	// Set up variables
	float xOffset = -5.0f;
	float yHeight = -2.0f;
	float zDepth = 8.0f;
	Odyssey::Entity* character;
	Odyssey::Entity* infoPopup = mEntity->getScene()->createEntity();
	Odyssey::Entity* prefab;
	Odyssey::UICanvas* infoPopupCanvas;
	DirectX::XMVECTOR position = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 140.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 uiPosition = { 120.0f, 145.0f };

	// Make Slot 1 Characters
	bool makeSlot1Characters = true;
	if (makeSlot1Characters)
	{
		// Paladin
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Mage
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Mage);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Bard
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Bard);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Warrior
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Warrior);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Monk
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Monk);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
	}

	// Update variables for the slot 2 characters
	xOffset = 0.0f;
	position = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	uiPosition = { 490.0f, 145.0f };

	// Make Slot 2 Characters
	bool makeSlot2Characters = true;
	if (makeSlot2Characters)
	{
		// Mage
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Mage);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Bard
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Bard);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Warrior
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Warrior);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Monk
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Monk);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Paladin
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
	}

	// Update variables for the slot 3 characters
	xOffset = 5.0f;
	position = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	rotation = DirectX::XMVectorSet(0.0f, 220.0f, 0.0f, 1.0f);
	uiPosition = { 860.0f, 145.0f };

	// Make Slot 3 Characters
	bool makeSlot3Characters = true;
	if (makeSlot3Characters)
	{
		// Bard
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Bard);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Warrior
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Warrior);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Monk
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Monk);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Paladin
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Mage
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Mage);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		character->setVisible(false);
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
	}
}

void TeamSelectionController::EnterBattle()
{
	RedAudioManager::Instance().StopGroup("BackgroundMenu");
	RedAudioManager::Instance().Loop("TorchBurningQuietly");
	RedAudioManager::Instance().SetVolume("TorchBurningQuietly", 200);
	RedAudioManager::Instance().Loop("BackgroundBattle");

	GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
	unsigned int indecies[3] = { mSlot1Index, mSlot2Index, mSlot3Index };
	SaveLoad::Instance().AddLoadOut("LoadOut_Test", ids, indecies);
	SaveLoad::Instance().SaveLoadOut();

	// Make a list of enums to hold based what character it is
	std::vector<CharacterFactory::CharacterOptions> enumList;
  
	// Check what characters are shown on the screen for the slot 1
	for (int i = 0; i < mSlot1CharacterList.size(); i++)
	{
		// Check if the character is visible or not
		if (mSlot1CharacterList[i]->isVisible())
		{
			// Get the visible character type
			CreateCharacterBasedOnName(mSlot1CharacterList[i]->getComponent<Character>()->GetName());

			// Turn off the info popup before entering battle
			mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(false);
		}
	}

	// Check what characters are shown on the screen for the slot 1
	for (int i = 0; i < mSlot2CharacterList.size(); i++)
	{
		// Check if the character is visible or not
		if (mSlot2CharacterList[i]->isVisible())
		{
			// Get the visible character type
			CreateCharacterBasedOnName(mSlot2CharacterList[i]->getComponent<Character>()->GetName());

			// Turn off the info popup before entering battle
			mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(false);
		}
	}

	// Check what characters are shown on the screen for the slot 1
	for (int i = 0; i < mSlot3CharacterList.size(); i++)
	{
		// Check if the character is visible or not
		if (mSlot3CharacterList[i]->isVisible())
		{
			// Get the visible character type
			CreateCharacterBasedOnName(mSlot3CharacterList[i]->getComponent<Character>()->GetName());

			// Turn off the info popup before entering battle
			mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(false);
		}
	}

	// Allow the user to enter the battle scene after selecting their characters
	mEnterBattle = true;
}

void TeamSelectionController::CreateCharacterBasedOnName(std::wstring _name)
{
	CharacterFactory::CharacterOptions heroType;

	if (_name == L"Paladin")
		heroType = CharacterFactory::CharacterOptions::Paladin;
	else if (_name == L"Mage")
		heroType = CharacterFactory::CharacterOptions::Mage;
	else if (_name == L"Bard")
		heroType = CharacterFactory::CharacterOptions::Bard;
	else if (_name == L"Warrior")
		heroType = CharacterFactory::CharacterOptions::Warrior;
	else if (_name == L"Monk")
		heroType = CharacterFactory::CharacterOptions::Monk;

	// This is the gGameScene from DyingDungeon.cpp
	Odyssey::Scene* gameScene = mListOfGameScenes[0];

	// Create the character and add it to the game scene
	DirectX::XMVECTOR position = mPlayerPositions[mBuildIndex];
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Odyssey::Entity* newCharacter = CharacterFactory::getInstance().CreateCharacter(heroType, _name, position, rotation, mHudPositions[mBuildIndex], true, mHpPopupPositions[mBuildIndex], gameScene);

	// Add the new character to the player list in Team Manager
	TeamManager::getInstance().AddCharacterToPlayerTeam(newCharacter);

	// Increase build index
	mBuildIndex++;
}

void TeamSelectionController::AddCharacterTypeToPlayerTeam(Character* _characterType)
{
	// TODO: Add enum to player to create list based on the name
	if (_characterType->GetName() == L"Paladin")
	
	// Increase the build index fo rthe next player when adding him to the list
	mBuildIndex++;
}

// Change the slot 1 character
void TeamSelectionController::DecreaseSlot1Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot1CharacterInfoPopupList[mSlot1Index]->isActive();
	// Set the old info popup to false
	mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(false);

	// Decrease the slot index
	mSlot1Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot1Index < 0)
		mSlot1Index = mSlot1CharacterList.size() - 1;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());
}

// Change the slot 1 character
void TeamSelectionController::IncreaseSlot1Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot1CharacterInfoPopupList[mSlot1Index]->isActive();
	// Set the old info popup to false
	mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(false);

	// Increase the slot index
	mSlot1Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot1Index > mSlot1CharacterList.size() - 1)
		mSlot1Index = 0;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());
}

// Change the slot 2 character
void TeamSelectionController::DecreaseSlot2Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot2CharacterInfoPopupList[mSlot2Index]->isActive();
	// Set the old info popup to false
	mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(false);

	// Decrease the slot index
	mSlot2Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot2Index < 0)
		mSlot2Index = mSlot2CharacterList.size() - 1;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());
}

// Change the slot 2 character
void TeamSelectionController::IncreaseSlot2Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot2CharacterInfoPopupList[mSlot2Index]->isActive();
	// Set the old info popup to false
	mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(false);

	// Increase the slot index
	mSlot2Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot2Index > mSlot2CharacterList.size() - 1)
		mSlot2Index = 0;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());
}

// Change the slot 3 character
void TeamSelectionController::DecreaseSlot3Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot3CharacterInfoPopupList[mSlot3Index]->isActive();
	// Set the old info popup to false
	mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(false);

	// Decrease the slot index
	mSlot3Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot3Index < 0)
		mSlot3Index = mSlot3CharacterList.size() - 1;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());
}

// Change the slot 3 character
void TeamSelectionController::IncreaseSlot3Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot3CharacterInfoPopupList[mSlot3Index]->isActive();
	// Set the old info popup to false
	mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(false);

	// Increase the slot index
	mSlot3Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot3Index > mSlot3CharacterList.size() - 1)
		mSlot3Index = 0;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());
}

// Toggle slot 1 stat popup
void TeamSelectionController::ToggleShowInfoPopup1()
{
	// Find which character is visiable
	for (int i = 0; i < mSlot1CharacterList.size(); i++)
	{
		// Check if the character is visible
		if (mSlot1CharacterList[i]->isVisible())
		{
			mSlot1CharacterInfoPopupList[i]->setActive(!mSlot1CharacterInfoPopupList[i]->isActive());
			break;
		}
	}
}

// Toggle slot 2 stat popup
void TeamSelectionController::ToggleShowInfoPopup2()
{
	// Find which character is visiable
	for (int i = 0; i < mSlot2CharacterList.size(); i++)
	{
		// Check if the character is visible
		if (mSlot2CharacterList[i]->isVisible())
		{
			mSlot2CharacterInfoPopupList[i]->setActive(!mSlot2CharacterInfoPopupList[i]->isActive());
			break;
		}
	}
}

// Toggle slot 2 stat popup
void TeamSelectionController::ToggleShowInfoPopup3()
{
	// Find which character is visiable
	for (int i = 0; i < mSlot3CharacterList.size(); i++)
	{
		// Check if the character is visible
		if (mSlot3CharacterList[i]->isVisible())
		{
			mSlot3CharacterInfoPopupList[i]->setActive(!mSlot3CharacterInfoPopupList[i]->isActive());
			break;
		}
	}
}

// Change the name of the new character displaying
void TeamSelectionController::ChangeSlotName(int _slotIndex, std::wstring _newName)
{
	GameUIManager::getInstance().GetNameSlots()[_slotIndex]->setText(_newName);
}

