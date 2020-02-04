#include "TeamSelectionController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "TeamManager.h"
#include "CharacterFactory.h"
#include "Material.h"
#include "TowerManager.h"

CLASS_DEFINITION(Odyssey::Component, TeamSelectionController)
TeamSelectionController::TeamSelectionController(Odyssey::Application* application)
{
	mApplication = application;
	mPaladin = nullptr;
	mMage = nullptr;
	mRect = nullptr;
}

void TeamSelectionController::initialize()
{
	//RedAudioManager::Instance().Loop("BackgroundMenu");
	//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
	// Reset bools and ints
	changedTheScene = false;
	mEnterBattle = false;
	mBuildIndex = 0;

	// Register callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->registerCallback("onMouseClick", this, &TeamSelectionController::EnterBattle);

	// Register callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot3Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot3Index);

	// Set the player positions
	mPlayerPositions.clear();
	mPlayerPositions.resize(3);
	mPlayerPositions[0] = DirectX::XMVectorSet(4.0f, 0.0f, 4.5f, 1.0f); // First Character Selected
	mPlayerPositions[1] = DirectX::XMVectorSet(0.0f, -0.6f, 4.5f, 1.0f); // Second Character Selected
	mPlayerPositions[2] = DirectX::XMVectorSet(-4.0f, 0.0f, 4.5f, 1.0f); // Third Character Selected

	// Set the HUD positions
	mHudPositions.clear();
	mHudPositions.resize(3);
	mHudPositions[0] = DirectX::XMFLOAT2(10.0f, 600.0f); // First Character HUD
	mHudPositions[1] = DirectX::XMFLOAT2(470.0f, 600.0f); // Second Character HUD
	mHudPositions[2] = DirectX::XMFLOAT2(910.0f, 600.0f); // Third Character HUD

	// Set the HP positions
	mHpPopupPositions.clear();
	mHpPopupPositions.resize(3);
	mHpPopupPositions[0] = DirectX::XMFLOAT2(350.0f, 400.0f); // First Character HP popup
	mHpPopupPositions[1] = DirectX::XMFLOAT2(640.0f, 400.0f); // Second Character HP popup
	mHpPopupPositions[2] = DirectX::XMFLOAT2(930.0f, 400.0f); // Third Character HP popup

	// Clear the player team from Team Manager before adding in new characters
	TeamManager::getInstance().ClearPlayerTeam();
}

void TeamSelectionController::update(double deltaTime)
{
	if (mEnterBattle && !changedTheScene)
	{
		changedTheScene = true;
		RedAudioManager::Instance().Stop("BackgroundMenu");
		RedAudioManager::Instance().Loop("BackgroundBattle");

		// Set up the tower manager with the enemy and player teams
		mCurrentTower->getComponent<TowerManager>()->SetUpTowerManager(TeamManager::getInstance().GetPlayerTeam(), TeamManager::getInstance().GetEnemyTeam(), 2, mTurnIndicatorModel);

		// Change the scene to the game
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Game"));
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
}

void TeamSelectionController::onDestory()
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
}

void TeamSelectionController::setupHovers()
{
	if (mPaladin == nullptr)
	{
		mPaladin = mEntity->addComponent<Odyssey::UICanvas>();
		setupCharacterHover(mPaladin, L"Paladin");
	}

	if (mMage == nullptr)
	{
		mMage = mEntity->addComponent<Odyssey::UICanvas>();
		setupCharacterHover(mMage, L"Mage");
	}
}

void TeamSelectionController::onPaladinEnter()
{
	mPaladin->setActive(true);
	mMage->setActive(false);
}

void TeamSelectionController::onPaladinExit()
{
	mPaladin->setActive(false);
	mMage->setActive(false);
}

void TeamSelectionController::onMageEnter()
{
	mMage->setActive(true);
	mPaladin->setActive(false);
}

void TeamSelectionController::onMageExit()
{
	mMage->setActive(false);
	mPaladin->setActive(false);
}

void TeamSelectionController::setupCharacterHover(Odyssey::UICanvas* canvas, std::wstring character)
{
	DirectX::XMFLOAT4 themeColor;
	std::wstring portrait;
	std::wstring level = L"Lvl 1";
	std::wstring title;
	std::wstring subtitle;
	std::wstring description;
	std::wstring skill1Name;
	std::wstring skill1Icon;
	std::wstring skill2Name;
	std::wstring skill2Icon;
	std::wstring skill3Name;
	std::wstring skill3Icon;
	std::wstring skill4Name;
	std::wstring skill4Icon;
	std::wstring health;
	std::wstring mana;
	std::wstring attack;
	std::wstring defense;
	std::wstring speed;

	DirectX::XMFLOAT4 paladinTheme = DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f);
	std::wstring paladinPortrait = L"assets/images/PaladinPortrait.jpg";
	std::wstring paladinTitle = L"Paladin";
	std::wstring paladinSubTitle = L"Divine Protector";
	std::wstring paladinDescription = L"Description: The paladin is a guardian of divine power and a protector of the righteous. Utilize the paladin's skills to protect your team and bring evil to bear.";
	std::wstring paladinSkill1Name = L"Basic Attack";
	std::wstring paladinSkill1Icon = L"assets/images/Paladin_Skill_1.png";
	std::wstring paladinSkill2Name = L"Judgement";
	std::wstring paladinSkill2Icon = L"assets/images/Paladin_Skill_2.png";
	std::wstring paladinSkill3Name = L"Shield of Light";
	std::wstring paladinSkill3Icon = L"assets/images/Paladin_Skill_3.png";
	std::wstring paladinSkill4Name = L"Blessing of Light";
	std::wstring paladinSkill4Icon = L"assets/images/Paladin_Skill_4.png";
	std::wstring paladinHealth = L"Health: 150";
	std::wstring paladinMana = L"Mana: 100";
	std::wstring paladinAttack = L"Attack: 15";
	std::wstring paladinDefense = L"Defense: 30";
	std::wstring paladinSpeed = L"Speed: 35";

	DirectX::XMFLOAT4 mageTheme = DirectX::XMFLOAT4(31.0f, 255.0f, 203.0f, 1.0f);
	std::wstring magePortrait = L"assets/images/MagePortrait.jpg";
	std::wstring mageTitle = L"Mage";
	std::wstring mageSubTitle = L"Elementalist";
	std::wstring mageDescription = L"Descrption: The mage possesses an ancient knowledge of the elemental forces. These skills can be used to inflict tremendous damage on all enemies in your path.";
	std::wstring mageSkill1Name = L"Basic Attack";
	std::wstring mageSkill1Icon = L"assets/images/Mage_Skill_1.png";
	std::wstring mageSkill2Name = L"Wind Slash";
	std::wstring mageSkill2Icon = L"assets/images/Mage_Skill_2.png";
	std::wstring mageSkill3Name = L"Firestorm";
	std::wstring mageSkill3Icon = L"assets/images/Mage_Skill_3.png";
	std::wstring mageSkill4Name = L"Lightning Bolt";
	std::wstring mageSkill4Icon = L"assets/images/Mage_Skill_4.png";
	std::wstring mageHealth = L"Health: 100";
	std::wstring mageMana = L"Mana: 150";
	std::wstring mageAttack = L"Attack: 50";
	std::wstring mageDefense = L"Defense: 10";
	std::wstring mageSpeed = L"Speed: 40";

	float x = 970.0f;
	float y = 450.0f;
	UINT width = 300;
	UINT height = 260;
	UINT pad = 7;

	Odyssey::TextProperties titleText;
	titleText.bold = true;
	titleText.italic = false;
	titleText.fontSize = 20.0f;
	titleText.textAlignment = Odyssey::TextAlignment::Left;
	titleText.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	titleText.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	properties.fontName = L"Tw Cen MT Condensed";

	if (character == L"Paladin")
	{
		themeColor = paladinTheme;
		portrait = paladinPortrait;
		title = paladinTitle;
		subtitle = paladinSubTitle;
		description = paladinDescription;
		skill1Name = paladinSkill1Name;
		skill1Icon = paladinSkill1Icon;
		skill2Name = paladinSkill2Name;
		skill2Icon = paladinSkill2Icon;
		skill3Name = paladinSkill3Name;
		skill3Icon = paladinSkill3Icon;
		skill4Name = paladinSkill4Name;
		skill4Icon = paladinSkill4Icon;
		health = paladinHealth;
		mana = paladinMana;
		attack = paladinAttack;
		defense = paladinDefense;
		speed = paladinSpeed;
	}
	else if (character == L"Mage")
	{
		themeColor = mageTheme;
		portrait = magePortrait;
		title = mageTitle;
		subtitle = mageSubTitle;
		description = mageDescription;
		skill1Name = mageSkill1Name;
		skill1Icon = mageSkill1Icon;
		skill2Name = mageSkill2Name;
		skill2Icon = mageSkill2Icon;
		skill3Name = mageSkill3Name;
		skill3Icon = mageSkill3Icon;
		skill4Name = mageSkill4Name;
		skill4Icon = mageSkill4Icon;
		health = mageHealth;
		mana = mageMana;
		attack = mageAttack;
		defense = mageDefense;
		speed = mageSpeed;
	}

	// Background and separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 45), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 110), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);

	// Title
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), portrait, 45, 45);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 45 + pad, y + pad), themeColor, 150, 50, title, titleText);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 120 + pad, y + 3 + pad), DirectX::XMFLOAT4(150.0f, 150.0f, 150.0f, 1.0f), 150, 50, subtitle, properties);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 240 + pad, y + pad), themeColor, 150, 50, level, titleText);

	// Description
	properties.fontSize = 12.0f;
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 45 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, description, properties);

	// Skills block
	properties.fontSize = 14.0f;
	properties.italic = false;
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 110 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, L"Skills: ", properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 10, y + 130 + pad), skill1Icon, 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 40, y + 130 + pad), themeColor, width - (2 * pad), 100, skill1Name, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 10, y + 160 + pad), skill2Icon, 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 40, y + 160 + pad), themeColor, width - (2 * pad), 100, skill2Name, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 10, y + 190 + pad), skill3Icon, 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 40, y + 190 + pad), themeColor, width - (2 * pad), 100, skill3Name, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 10, y + 220 + pad), skill4Icon, 25, 25);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 40, y + 220 + pad), themeColor, width - (2 * pad), 100, skill4Name, properties);

	// Stats block
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 175, y + 110 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, L"Stats: ", properties);
	properties.fontSize = 12.0f;
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 180, y + 130 + pad), L"assets/images/Meat.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 210, y + 130 + pad), DirectX::XMFLOAT4(50.0f, 255.0f, 50.0f, 1.0f), width - (2 * pad), 100, health, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 180, y + 155 + pad), L"assets/images/mp.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 210, y + 155 + pad), DirectX::XMFLOAT4(50.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, mana, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 180, y + 180 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 210, y + 180 + pad), DirectX::XMFLOAT4(255.0f, 50.0f, 50.0f, 1.0f), width - (2 * pad), 100, attack, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 180, y + 205 + pad), L"assets/images/Shield.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 210, y + 205 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, defense, properties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 180, y + 230 + pad), L"assets/images/Speed.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 210, y + 230 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 50.0f, 1.0f), width - (2 * pad), 100, speed, properties);
	canvas->setActive(false);
}

void TeamSelectionController::EnterBattle()
{
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

	// This is the gGameScene from DyingDungeon.cpp
	std::shared_ptr<Odyssey::Scene> gameScene = mListOfGameScenes[0];

	// Create the paladin and add it to the game scene
	DirectX::XMVECTOR position = mPlayerPositions[mBuildIndex];
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	std::shared_ptr<Odyssey::Entity> newCharacter = CharacterFactory::getInstance().CreateCharacter(heroType, _name, position, rotation, mHudPositions[mBuildIndex], true, mHpPopupPositions[mBuildIndex], gameScene);

	// Add the paladin to all other game scenes, we add it into the first scene because we are passing it in the function
	for (int i = 1; i < mListOfGameScenes.size(); i++)
	{
		mListOfGameScenes[i]->addEntity(newCharacter);
	}

	// Add the new character to the player list in Team Manager
	TeamManager::getInstance().AddCharacterToPlayerTeam(newCharacter);

	// Increase build index
	mBuildIndex++;
}

void TeamSelectionController::DecreaseSlot1Index()
{
	// Disable the current character that is visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(false);

	// Decrease the slot index
	mSlot1Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot1Index < 0)
		mSlot1Index = mSlot1CharacterList.size() - 1;

	// Enable the new current character that will need to be visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(true);
}

void TeamSelectionController::IncreaseSlot1Index()
{
	// Disable the current character that is visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(false);

	// Increase the slot index
	mSlot1Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot1Index > mSlot1CharacterList.size() - 1)
		mSlot1Index = 0;

	// Enable the new current character that will need to be visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(true);
}

void TeamSelectionController::DecreaseSlot2Index()
{
	// Disable the current character that is visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(false);

	// Decrease the slot index
	mSlot2Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot2Index < 0)
		mSlot2Index = mSlot2CharacterList.size() - 1;

	// Enable the new current character that will need to be visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(true);
}

void TeamSelectionController::IncreaseSlot2Index()
{
	// Disable the current character that is visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(false);

	// Increase the slot index
	mSlot2Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot2Index > mSlot2CharacterList.size() - 1)
		mSlot2Index = 0;

	// Enable the new current character that will need to be visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(true);
}

void TeamSelectionController::DecreaseSlot3Index()
{
	// Disable the current character that is visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(false);

	// Decrease the slot index
	mSlot3Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot3Index < 0)
		mSlot3Index = mSlot3CharacterList.size() - 1;

	// Enable the new current character that will need to be visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(true);
}

void TeamSelectionController::IncreaseSlot3Index()
{
	// Disable the current character that is visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(false);

	// Increase the slot index
	mSlot3Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot3Index > mSlot3CharacterList.size() - 1)
		mSlot3Index = 0;

	// Enable the new current character that will need to be visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(true);
}
