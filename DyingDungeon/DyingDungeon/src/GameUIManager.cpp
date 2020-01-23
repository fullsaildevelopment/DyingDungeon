#include "GameUIManager.h"
#include "RedAudioManager.h"
#include "UICanvas.h"

#define BackgroundBigOpacity 0.5f
#define BackgroundSmallOpacity 0.8f

GameUIManager& GameUIManager::getInstance()
{
	static GameUIManager instance;
	return instance;
}

void GameUIManager::CreateBattleLog(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the battle log object
	mBattleLog = std::make_shared<Odyssey::Entity>();
	mBattleLog->addComponent<Odyssey::UICanvas>();
	// Get the canvas
	Odyssey::UICanvas* battleLogCanvas = mBattleLog->getComponent<Odyssey::UICanvas>();

	// Setup init values
	UINT width = battleTextWidth = screenWidth / 4;
	UINT height = battleTextHeight = screenHeight / 2;
	

	DirectX::XMFLOAT2 position = { 0.0f, static_cast<float>(screenHeight) / 3.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color

	// Setup text properties
	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 20.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	properties.fontName = L"Constantia";

	// Create the battle log text
	mBattleLogText = battleLogCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"I am battle log, I will show you what is going on the the game", properties);
	mBattleLogVec.push_back(mBattleLogText);
	mBattleLogText = battleLogCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2{ 0.0f, (static_cast<float>(screenHeight) / 3.0f) - 50.0f }, color, width, height, L"I am blank", properties);
	mBattleLogVec.push_back(mBattleLogText);


	// Add the entity to the game scene
	_sceneToAddTo->addEntity(mBattleLog);
}


void GameUIManager::SetBattleLogText(std::wstring newText, bool concat)
{
	if (concat)
	{
		mBattleLogVec[0]->setText(mBattleLogVec[0]->getText().append(L" "));
		mBattleLogVec[0]->setText(mBattleLogVec[0]->getText().append(newText));
		return;
	}


	for (int i = mBattleLogVec.size() - 1; i > 0; i--)
	{
		if (i - 1 >= 0)
			mBattleLogVec[i]->setText(mBattleLogVec[i-1]->getText());
	}
	
	mBattleLogVec[0]->setText(newText);

}

void GameUIManager::ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive)
{
	// Set the passed in entity's canvas to active or deactived based on the bool _isActive.
	_canvas->setActive(_isActive);
}

// This is where I will design and add all the elements into the main menu screen
void GameUIManager::CreateMainMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	//Create the pause menu pointer
	mMainMenu = std::make_shared<Odyssey::Entity>();
	mMainMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the main menu
	Odyssey::UICanvas* mainMenuCanvas = mMainMenu->getComponent<Odyssey::UICanvas>();

	// Setup init values
	UINT width = screenWidth;
	UINT height = screenHeight;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	// Set up text properties
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";

	// Create title text
	height = 100;
	mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"The Dying Dungeon", properties);

	// Create new game text 
	properties.bold = false;
	properties.fontSize = 40.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	position.x += 200.0f;
	position.y += 250.0f;
	width = 200;
	height = 40;
	mainMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f), width, height);
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mNewGameText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"New Game", properties);

	// Setup team logo
	mTeamLogo = mainMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/GreatApeStudiosLogo.png", screenWidth, screenHeight);

	// Add the pause menu to the main scene
	_sceneToAddTo->addEntity(mMainMenu);
}

// This is where I will design and add all the elements into the tower select screen
void GameUIManager::CreateTowerSelectMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the tower select menu pointer
	mTowerSelectMenu = std::make_shared<Odyssey::Entity>();
	mTowerSelectMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* towerSelectMenuCanvas = mTowerSelectMenu->getComponent<Odyssey::UICanvas>();

	// Initialize variables
	UINT width = screenWidth; // Width
	UINT height = 100; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	Odyssey::TextProperties properties;

	// Create tower select title
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 100.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";
	mTowerSelectTitle = towerSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Select A Tower To Enter", properties);

	// Create door to click on
	width = 195;
	height = 231;
	position = { 100.0f, (static_cast<float>(screenHeight) / 2.0f) - (height / 2.0f) };
	mDoorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/MedievalDoor.png", width, height);

	// Add the pause menu to the game scene most likely
	_sceneToAddTo->addEntity(mTowerSelectMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mTowerSelectMenu->getComponent<Odyssey::UICanvas>(), false);
}

void GameUIManager::CreateTeamSelectMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the tower select menu pointer
	mTeamSelectMenu = std::make_shared<Odyssey::Entity>();
	mTeamSelectMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* teamSelectMenuCanvas = mTeamSelectMenu->getComponent<Odyssey::UICanvas>();
}

// This is where I will design and add all elements into the pause menu canvas
void GameUIManager::CreatePauseMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	//Create the pause menu pointer
	mPauseMenu = std::make_shared<Odyssey::Entity>();
	mPauseMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* pauseMenuCanvas = mPauseMenu->getComponent<Odyssey::UICanvas>();

	// Create the rectangle object
	UINT width = 1920; // Width
	UINT height = 1080; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f }; // Color
	// Add the rectangle to the pause menu canvas
	mBlackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Make the rectangle have 50% transparency
	mBlackBackground->setOpacity(BackgroundBigOpacity);

	// Create the pause menu smaller black rectangle
	width = 640;
	height = 360;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	// Add the rectangle to the pause menu canvas
	mSmallerBlackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mSmallerBlackBackground->setOpacity(BackgroundSmallOpacity);

	// Create Pause Title
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";
	mPauseTitle = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, 640, 60, L"Paused", properties);

	// Resume Button
	width /= 2.5f;
	height = 50;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 50.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the reusume background to the canvas
	mResumeBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Resume text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 30.0f;
	mResumeText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"RESUME", properties);

	// Options Button
	position.y += 75.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the options background to the canvas
	mOptionsBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Options text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mOptionsText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"OPTIONS", properties);

	// Main Menu Button
	position.y += 75.0f;
	color = { 180.0f, 30.0f, 30.0f, 1.0f };
	// Add the main menu background to the canvas
	mMainMenuBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Main menu text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mMainMenuText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"MAIN MENU", properties);

	// Add the pause menu to the game scene most likely
	_sceneToAddTo->addEntity(mPauseMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mPauseMenu->getComponent<Odyssey::UICanvas>(), false);

	// Make options menu
	CreateOptionsMenu(_sceneToAddTo);
}

// This is where I will design and add all elements into the options menu canvas
void GameUIManager::CreateOptionsMenu(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Set options menu pointer and add a canvas
	mOptionsMenu = std::make_shared<Odyssey::Entity>();
	mOptionsMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the options menu
	Odyssey::UICanvas* optionsMenuCanvas = mOptionsMenu->getComponent<Odyssey::UICanvas>();

	// Set init values
	// Create the rectangle object
	UINT width = 1920; // Width
	UINT height = 1080; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f }; // Color
	// Add the rectangle to the options menu canvas
	mBlackBackground = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Make the rectangle have 50% transparency
	mBlackBackground->setOpacity(BackgroundBigOpacity);

	// Create the pause menu smaller black rectangle
	width = 640;
	height = 360;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	DirectX::XMFLOAT2 originalPosition = position;
	// Add the rectangle to the options menu canvas
	mSmallerBlackBackground = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mSmallerBlackBackground->setOpacity(BackgroundSmallOpacity);

	// Create options title
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";
	mOptionsTitle = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, 60, L"Options", properties);

	// Adjust volume text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 20.0f;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 75.0f;
	mVolumeText = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Adjust Volume", properties);
	// Volume bar
	width /= 2.0f;
	height = 25;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 45.0f;
	// Create the volume bar background
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f), color, width + 5.0f, height + 5.0f);
	color = { 70.0f, 70.0f, 70.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Add the volume bar to the canvas
	color = { 50.0f, 180.0f, 255.0f, 1.0f };
	mVolumeBar = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// TODO :: FILL THE RECTANGLE BASED ON THE VOLUME LEVEL
	mVolumeBar->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume()) / 1000.0f);

	// Create the plus and minus symbols that the user can click on to adjust the volume
	UINT imageWidth = 32;
	UINT imageHeight = 32;
	position.x -= static_cast<float>(imageWidth) * 1.5f;
	position.y -= 4.0f;
	// Minus Symbol
	mMinusImage = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMinusImage->registerCallback("onMouseClick", this, &GameUIManager::DecreaseVolume);
	// Plus Symbol
	position.x += (static_cast<float>(imageWidth) * 1.5f) + width + (static_cast<float>(imageWidth) / 2.0f);
	mPlusImage = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mPlusImage->registerCallback("onMouseClick", this, &GameUIManager::IncreaseVolume);

	// Create the options back button
	position = originalPosition;
	width = 100.0f;
	height = 30.0f;
	position.y += 360.0f - height;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	// Add the back button text
	mBackButtonText = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Back", properties);
	// Have the back button go to the pause menu
	mBackButtonText->registerCallback("onMouseClick", this, &GameUIManager::OptionsBackButton);

	// Add the options menu to the game scene most likely
	_sceneToAddTo->addEntity(mOptionsMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mOptionsMenu->getComponent<Odyssey::UICanvas>(), false);
}

// This function will get called when you click on the back button on the options menu in pause
void GameUIManager::OptionsBackButton()
{
	// Turn off the options menu's canvas
	mOptionsMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	
	// Turn on the pause menu's canvas
	mPauseMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
}

// Create the character's UI Portrait
void GameUIManager::CreateCharacterPortrait(float anchorX, float anchorY, LPCWSTR _imageName, std::shared_ptr<Odyssey::Entity> _gameObject, Character* owner)
{
	// Create the character's hud entity and add a canvas to it.
	// Get the canvas that was added
	Odyssey::UICanvas* characterHudCanvas = owner->getEntity()->addComponent<Odyssey::UICanvas>();
	
	// Create Hero UI if the character is a hero
	if (owner->IsHero())
	{
		Odyssey::TextProperties properties;
		properties.bold = false;
		properties.italic = false;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
		properties.fontName = L"Tw Cen MT Condensed";

		// Get the position of the anchor points
		DirectX::XMFLOAT2 position = { anchorX, anchorY };
		// Save the original position for the future when creating new items
		DirectX::XMFLOAT2 originalPosition = position;
		// Set the image width and height
		// Set the bar width and height for the Rectangle2Ds
		UINT imageWidth = 340;
		UINT imageHeight = 146;
		UINT barWidth = 246;
		UINT barHeight = 23;
		DirectX::XMFLOAT4 color = { 116.0f, 71.0f, 201.0f, 1.0f };

		// Create the base ui template
		characterHudCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/HeroUILayout2.0.png", imageWidth, imageHeight);

		// Create the character's image
		imageWidth = 64;
		imageHeight = 64;
		characterHudCanvas->addElement<Odyssey::Sprite2D>(position, _imageName, imageWidth, imageHeight);

		// Create the xp/name bar
		//TODO::Create XP Bars In The Character Class To Assign
		position.x += imageWidth;
		if (owner)
			characterHudCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);
		else
			characterHudCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);

		// Create the text element of the character's name
		color = { 0.0f, 0.0f, 0.0f, 1.0f };
		position.x += 7.5f;
		// Convert name to wstring
		size_t cSize = strlen(owner->GetName().c_str()) + 1;
		size_t convertedChars = 0;
		wchar_t* characterName = new wchar_t[cSize];
		mbstowcs_s(&convertedChars, characterName, cSize, owner->GetName().c_str(), _TRUNCATE);
		// Add the name to the canvas
		properties.bold = true;
		characterHudCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, characterName, properties);
		properties.bold = false;
		// Delete the newed pointer
		delete characterName;
		characterName = nullptr;

		// Create the character's level number text next to the XP bar
		position.x += barWidth;
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		characterHudCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"99", properties);

		// Create the big HP text
		position.x -= barWidth + 7.5f;
		position.y += barHeight;
		properties.fontSize = 49.0f;
		properties.textAlignment = Odyssey::TextAlignment::Center;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
		owner->mBigHpText = characterHudCanvas->addElement<Odyssey::Text2D>(position, mBigHealthTextColor, 80, 95, std::to_wstring((int)owner->GetHP()), properties);

		// TODO: Add the text elements for Attack, Defense, and Speed;
		position = originalPosition;
		position.x += 30.0f;
		position.y += 62.5f;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		// Attack number
		characterHudCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(static_cast<int>(owner->GetAtk() * 100.0f)), properties);
		// Defense number
		position.y += 17.5f;
		characterHudCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(static_cast<int>(owner->GetDef() * 100.0f)), properties);
		// Speed number
		position.y += 17.5f;
		characterHudCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring((int)owner->GetSpeed()), properties);

		// Health and Mana bars
		// Set the position to the origanal top left position
		position = originalPosition;
		barHeight = 14;
		barWidth = 340;
		position.y += imageHeight + 53.5f;
		if (owner)
		{
			// Create and assign the health bar
			owner->pHealthBar = characterHudCanvas->addElement<Odyssey::Rectangle2D>(position, mHealthBarColor, barWidth, barHeight);
			owner->pHealthBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
			// Create the text for the health numbers of the character
			color = { 255.0f, 255.0f, 255.0f, 1.0f };
			properties.fontSize = 10.5f;
			position.x += 5.0f;
			owner->mHpText = characterHudCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring((int)owner->GetHP()) + L"/" + std::to_wstring((int)owner->GetMaxHP()), properties);
			// Create and assign the mana bar
			position.x -= 5.0f;
			position.y += barHeight;
			owner->pManaBar = characterHudCanvas->addElement<Odyssey::Rectangle2D>(position, mManaBarColor, barWidth, barHeight);
			owner->pManaBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
			// Create the text for the mana numbers of the character
			color = { 255.0f, 255.0f, 255.0f, 1.0f };
			position.x += 5.0f;
			owner->mMpText = characterHudCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring((int)owner->GetMana()) + L"/" + std::to_wstring((int)owner->GetMaxMana()), properties);
		}
		else
		{
			Odyssey::Rectangle2D* rect = characterHudCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + 50), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 50, 10);
			rect->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
			rect = characterHudCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + 62), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 50, 10);
			rect->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		}

		// Position where the turn number will be located
		position = originalPosition;
		position.x += 5.0f;
		properties.fontSize = 14.0f;
		// Assign the character's turn order text
		if (owner)
			owner->pTurnNumber = characterHudCanvas->addElement<Odyssey::Text2D>(position, mTurnOrderColor, 32, 32, L"1", properties);
	}
	// Create the Enemy UI if the character is NOT a hero
	else
	{
		Odyssey::TextProperties properties;
		properties.bold = false;
		properties.italic = false;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		properties.fontName = L"Tw Cen MT Condensed";

		// Get the position of the anchor points
		DirectX::XMFLOAT2 position = { anchorX, anchorY };
		// Save the original position for the future when creating new items
		DirectX::XMFLOAT2 originalPosition = position;
		// Set the image width and height
		// Set the bar width and height for the Rectangle2Ds
		UINT imageWidth = 228;
		UINT imageHeight = 43;
		UINT barWidth = 185;
		UINT barHeight = 12;
		DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f };

		// Add in the enemy hud template
		characterHudCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/EnemyHUDTemplate.jpg", imageWidth, imageHeight);

		// Add in the enemy's portrait picture
		imageWidth = 43;
		imageHeight = 43;
		characterHudCanvas->addElement<Odyssey::Sprite2D>(position, _imageName, imageWidth, imageHeight);

		// Add in the enemy's health bar
		position.x += imageWidth;
		owner->pHealthBar = characterHudCanvas->addElement<Odyssey::Rectangle2D>(position, mHealthBarColor, barWidth, barHeight);
		owner->pHealthBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		// Add big health text
		position.x += barWidth;
		properties.fontSize = 25.0f;
		properties.textAlignment = Odyssey::TextAlignment::Center;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
		owner->mBigHpText = characterHudCanvas->addElement<Odyssey::Text2D>(position, mBigHealthTextColor, 43, 43, std::to_wstring((int)owner->GetHP()), properties);
		// Create but don't show the mini hp text
		owner->mHpText = characterHudCanvas->addElement<Odyssey::Text2D>(position, mBigHealthTextColor, 43, 43, std::to_wstring((int)owner->GetHP()), properties);
		owner->mHpText->setVisible(false);

		// Add in the enemy's mana bar
		owner->pManaBar = characterHudCanvas->addElement<Odyssey::Rectangle2D>(position, mManaBarColor, barWidth, barHeight);
		owner->pManaBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		owner->pManaBar->setVisible(false);
		owner->mMpText = characterHudCanvas->addElement<Odyssey::Text2D>(position, mBigHealthTextColor, 43, 43, std::to_wstring((int)owner->GetHP()), properties);
		owner->mMpText->setVisible(false);

		// Position where the turn number will be located
		position = originalPosition;
		position.x += 5.0f;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		// Assign the character's turn order text
		owner->pTurnNumber = characterHudCanvas->addElement<Odyssey::Text2D>(position, mTurnOrderColor, 32, 32, L"1", properties);
	}
}

void GameUIManager::DecreaseVolume()
{
	// Decrease volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume() - 50);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume()) / 1000.0f;
	mVolumeBar->setFill(volumeRatio);
}

void GameUIManager::IncreaseVolume()
{
	// Increase volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume() + 50);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume()) / 1000.0f;
	mVolumeBar->setFill(volumeRatio);
}