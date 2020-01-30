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

	// Get the deminsion of the original black square
	DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(1)->getDimensions();
	//Set the images black to the default black square
	GameUIManager::getInstance().GetTeamMemberSlot(0)->setSprite(L"assets/images/blackSquare.png", deminsion.x, deminsion.y);
	GameUIManager::getInstance().GetTeamMemberSlot(1)->setSprite(L"assets/images/blackSquare.png", deminsion.x, deminsion.y);
	GameUIManager::getInstance().GetTeamMemberSlot(2)->setSprite(L"assets/images/blackSquare.png", deminsion.x, deminsion.y);

	// Reset bools and ints
	changedTheScene = false;
	teamCount = 0;
	teamIsFull = false;

	// Register callbacks
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseClick", this, &TeamSelectionController::AddPaladinImage);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseClick", this, &TeamSelectionController::AddMageImage);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseEnter", this, &TeamSelectionController::onPaladinEnter);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseEnter", this, &TeamSelectionController::onMageEnter);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseExit", this, &TeamSelectionController::onPaladinExit);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseExit", this, &TeamSelectionController::onMageExit);

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

	// Create the tower manger object
	CreateTheTowerManager();

	// Clear the player team from Team Manager before adding in new characters
	TeamManager::getInstance().GetPlayerTeam().clear();
}

void TeamSelectionController::update(double deltaTime)
{
	if (teamIsFull && !changedTheScene)
	{
		changedTheScene = true;
		RedAudioManager::Instance().Stop("BackgroundMenu");
		RedAudioManager::Instance().Loop("BackgroundBattle");

		// Unregister the select images
		GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->unregisterCallback("onMouseEnter");
		GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->unregisterCallback("onMouseEnter");
		GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->unregisterCallback("onMouseExit");
		GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->unregisterCallback("onMouseExit");

		// Set up the tower manager with the enemy and player teams
		mCurrentTower->getComponent<TowerManager>()->SetUpTowerManager(TeamManager::getInstance().GetPlayerTeam(), TeamManager::getInstance().GetEnemyTeam(), 2, mTurnIndicatorModel);
		mListOfGameScenes[0]->addEntity(mCurrentTower);

		// Change the scene to the game
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Game"));
	}
	if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::M))
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
	// Register callbacks
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->unregisterCallback("onMouseEnter");
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->unregisterCallback("onMouseEnter");
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->unregisterCallback("onMouseExit");
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->unregisterCallback("onMouseExit");

	// Register callbacks for the arrows
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

void TeamSelectionController::AddPaladinImage()
{
	// Make sure the team is not full before adding a character
	if (!teamIsFull)
	{
		// Get the deminsion of the original black square
		DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->getDimensions();
		//Set the new image to the slot
		GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->setSprite(L"assets/images/PaladinPortrait.jpg", deminsion.x, deminsion.y);

		// Create the paladin and add it to the game scene
		DirectX::XMVECTOR position = mPlayerPositions[teamCount];
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
		std::shared_ptr<Odyssey::Entity> paladinCharacter = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Paladin, L"Paladin", position, rotation, mHudPositions[teamCount], true, mHpPopupPositions[teamCount], mListOfGameScenes[0]);

		// Add the paladin to all other game scenes, we add it into the first scene because we are passing it in the function
		for (int i = 1; i < mListOfGameScenes.size(); i++)
		{
			mListOfGameScenes[i]->addEntity(paladinCharacter);
		}

		// Add the new character to the player list in Team Manager
		TeamManager::getInstance().AddCharacterToPlayerTeam(paladinCharacter);

		// Increase the team count
		teamCount++;
		// Check if the team is full now
		if (teamCount == 3)
		{
			teamIsFull = true;
		}
	}
}

void TeamSelectionController::AddMageImage()
{
	if (!teamIsFull)
	{
		// Get the deminsion of the original black square
		DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->getDimensions();
		//Set the new image to the slot
		GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->setSprite(L"assets/images/MagePortrait.jpg", deminsion.x, deminsion.y);

		// Create the mage and add it to the game scene
		DirectX::XMVECTOR position = mPlayerPositions[teamCount];
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
		std::shared_ptr<Odyssey::Entity> mageCharacter = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Mage, L"Mage", position, rotation, mHudPositions[teamCount], true, mHpPopupPositions[teamCount],mListOfGameScenes[0]);

		// Add the mage to all other game scenes, we add it into the first scene because we are passing it in the function
		for (int i = 1; i < mListOfGameScenes.size(); i++)
		{
			mListOfGameScenes[i]->addEntity(mageCharacter);
		}

		// Add the new character to the player list in Team Manager
		TeamManager::getInstance().AddCharacterToPlayerTeam(mageCharacter);

		// Increase the team count
		teamCount++;
		// Check if the team is full now
		if (teamCount == 3)
		{
			teamIsFull = true;
		}
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

// This where I will create the brand new tower
void TeamSelectionController::CreateTheTowerManager()
{
	// Create the current tower entity
	mCurrentTower = std::make_shared<Odyssey::Entity>();
	mCurrentTower->addComponent<TowerManager>();
	mCurrentTower->getComponent<TowerManager>()->UI = mGameEntity->getComponents<Odyssey::UICanvas>()[0];
	mCurrentTower->getComponent<TowerManager>()->Rewards = mGameEntity->getComponents<Odyssey::UICanvas>()[1];
	// TODO: REFACTOR LATER
	mCurrentTower->getComponent<TowerManager>()->scene = mListOfGameScenes[0].get();

	// Create the turn indicator circle
	mTurnIndicatorModel = std::make_shared<Odyssey::Entity>();
	mTurnIndicatorModel->addComponent<Odyssey::Transform>();
	mTurnIndicatorModel->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	mTurnIndicatorModel->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::FileManager::getInstance().importModel(mTurnIndicatorModel, "assets/models/TurnIndicator.dxm", false);
	DirectX::XMFLOAT4 turnIndicatorColor = { 0.0f, 0.0f, 255.0f, 1.0f };
	mTurnIndicatorModel->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor(turnIndicatorColor);
	mTurnIndicatorModel->setStatic(false);
	// Add the turn indicator to the game scene
	mListOfGameScenes[0]->addEntity(mTurnIndicatorModel);

	// Skeleton #1
	DirectX::XMVECTOR charPosition = DirectX::XMVectorSet(7.5f, 0.3f, -5.0f, 1.0f);
	DirectX::XMVECTOR charRotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 hudPosition = { 10.0f, 10.0f };
	DirectX::XMFLOAT2 hpPopupPosition = { 300.0f, 200.0f };
	std::shared_ptr<Odyssey::Entity> characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, L"Skeleton Un", charPosition, charRotation, hudPosition, true, hpPopupPosition, mListOfGameScenes[0]);
	TeamManager::getInstance().AddCharacterToEnemyTeam(characterToAdd);

	// Skeleton #2
	charPosition = DirectX::XMVectorSet(3.0f, -0.6f, -5.0f, 1.0f);
	hudPosition.x += 329.7f;
	hpPopupPosition.x += 200.0f;
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, L"Skeleton Deux", charPosition, charRotation, hudPosition, true, hpPopupPosition, mListOfGameScenes[0]);
	TeamManager::getInstance().AddCharacterToEnemyTeam(characterToAdd);

	// Skeleton #3
	charPosition = DirectX::XMVectorSet(-3.0f, -0.6f, -5.0f, 1.0f);
	hudPosition.x += 329.7f;
	hpPopupPosition.x += 200.0f;
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, L"Skeleton Trois", charPosition, charRotation, hudPosition, true, hpPopupPosition, mListOfGameScenes[0]);
	TeamManager::getInstance().AddCharacterToEnemyTeam(characterToAdd);

	// Skeleton #4
	charPosition = DirectX::XMVectorSet(-7.5f, 0.3f, -5.0f, 1.0f);
	hudPosition.x += 329.7f;
	hpPopupPosition.x += 200.0f;
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, L"Skeleton Quatre", charPosition, charRotation, hudPosition, true, hpPopupPosition, mListOfGameScenes[0]);
	TeamManager::getInstance().AddCharacterToEnemyTeam(characterToAdd);

	// Ganfaul
	charPosition = DirectX::XMVectorSet(0.0f, 0.3f, -5.0f, 1.0f);
	hudPosition.x -= 329.7f;
	characterToAdd = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Ganfaul, L"Ganfaul", charPosition, charRotation, hudPosition, true, hpPopupPosition, mListOfGameScenes[0]);
	characterToAdd->setActive(false);
	// Assign the boss character for the tower
	mCurrentTower->getComponent<TowerManager>()->SetBossCharacter(characterToAdd);
}