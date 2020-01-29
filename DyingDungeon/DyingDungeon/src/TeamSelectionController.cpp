#include "TeamSelectionController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "TeamManager.h"
#include "CharacterFactory.h"

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
	GameUIManager::getInstance().GetTeamMemberSlot(1)->setSprite(L"assets/images/blackSquare.png", deminsion.x, deminsion.y);
	GameUIManager::getInstance().GetTeamMemberSlot(2)->setSprite(L"assets/images/blackSquare.png", deminsion.x, deminsion.y);
	GameUIManager::getInstance().GetTeamMemberSlot(3)->setSprite(L"assets/images/blackSquare.png", deminsion.x, deminsion.y);

	changedTheScene = false;

	teamCount = 0;
	teamIsFull = false;

	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseClick", this, &TeamSelectionController::AddPaladinImage);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseClick", this, &TeamSelectionController::AddMageImage);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseEnter", this, &TeamSelectionController::onPaladinEnter);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseEnter", this, &TeamSelectionController::onMageEnter);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseExit", this, &TeamSelectionController::onPaladinExit);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseExit", this, &TeamSelectionController::onMageExit);
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

		// Change the scene to the game
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Game"));
	}
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
		// Increase the team count
		teamCount++;

		// Get the deminsion of the original black square
		DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->getDimensions();
		//Set the new image to the slot
		GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->setSprite(L"assets/images/PaladinPortrait.jpg", deminsion.x, deminsion.y);

		// Create the paladin and add it to the game scene
		DirectX::XMVECTOR position = DirectX::XMVectorSet(0.0f, 0.3f, 0.0f, 1.0f);
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
		std::shared_ptr<Odyssey::Entity> paladinCharacter = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Paladin, L"Paladin", position, rotation, DirectX::XMFLOAT2{ 0.0f, 0.0f }, false, mListOfGameScenes[0]);

		// Add the paladin to all other game scenes, we add it into the first scene because we are passing it in the function
		for (int i = 1; i < mListOfGameScenes.size(); i++)
		{

		}


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
		// Increase the team count
		teamCount++;

		// Get the deminsion of the original black square
		DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->getDimensions();
		//Set the new image to the slot
		GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->setSprite(L"assets/images/MagePortrait.jpg", deminsion.x, deminsion.y);

		// Create the mage and add it to the game scene
		DirectX::XMVECTOR position = DirectX::XMVectorSet(0.0f, 0.3f, 0.0f, 1.0f);
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
		std::shared_ptr<Odyssey::Entity> mageCharacter = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Mage, L"Mage", position, rotation, DirectX::XMFLOAT2{ 0.0f, 0.0f }, false, mListOfGameScenes[0]);

		// Add the paladin to all other game scenes, we add it into the first scene because we are passing it in the function
		for (int i = 1; i < mListOfGameScenes.size(); i++)
		{

		}

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

