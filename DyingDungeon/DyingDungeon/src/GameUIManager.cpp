#include "GameUIManager.h"
#include "RedAudioManager.h"
#include "UICanvas.h"
#include "CharacterHUDElements.h"
#include "SkillHUDElements.h"
#include "HeroComponent.h"
#include "EventManager.h"
#include "CombatEvents.h"
#include "TeamManager.h"

// TODO: REFACTOR LATER
#include "SkillHoverComponent.h"

#define BackgroundBigOpacity 0.5f
#define BackgroundSmallOpacity 0.8f

GameUIManager& GameUIManager::getInstance()
{
	static GameUIManager instance;
	return instance;
}

void GameUIManager::initialize(Odyssey::Application* _application)
{
	// Set application
	mApplication = _application;

	// Create the entity to create new UI prefabs
	Odyssey::Entity* newUIPrefab;

	// Create the pause menu prefab
	newUIPrefab = CreatePauseMenuPrefab();
	// Add the new pause menu to the prefab map
	mUIObjectsPrefabMap[UIObject::PauseMenu] = newUIPrefab;

	// Create the hero clickable UI elements
	newUIPrefab = CreateClickableUIPrefab(DirectX::XMFLOAT2(300.0f, 300.0f), true);
	// Add the new clickable ui elements to the prefab map
	mClickableUIPrefabMap[ClickableCharacterUI::HeroLeft] = newUIPrefab;

	// Create the hero clickable UI elements
	newUIPrefab = CreateClickableUIPrefab(DirectX::XMFLOAT2(590.0f, 300.0f), true);
	// Add the new clickable ui elements to the prefab map
	mClickableUIPrefabMap[ClickableCharacterUI::HeroMiddle] = newUIPrefab;

	// Create the hero clickable UI elements
	newUIPrefab = CreateClickableUIPrefab(DirectX::XMFLOAT2(900.0f, 300.0f), true);
	// Add the new clickable ui elements to the prefab map
	mClickableUIPrefabMap[ClickableCharacterUI::HeroRight] = newUIPrefab;

	// Create the enemy clickable UI elements
	newUIPrefab = CreateClickableUIPrefab(DirectX::XMFLOAT2(450.0f, 150.0f), false);
	// Add the new clickable ui elements to the prefab map
	mClickableUIPrefabMap[ClickableCharacterUI::EnemyLeft] = newUIPrefab;

	// Create the enemy clickable UI elements
	newUIPrefab = CreateClickableUIPrefab(DirectX::XMFLOAT2(600.0f, 150.0f), false);
	// Add the new clickable ui elements to the prefab map
	mClickableUIPrefabMap[ClickableCharacterUI::EnemyMiddle] = newUIPrefab;

	// Create the enemy clickable UI elements
	newUIPrefab = CreateClickableUIPrefab(DirectX::XMFLOAT2(750.0f, 150.0f), false);
	// Add the new clickable ui elements to the prefab map
	mClickableUIPrefabMap[ClickableCharacterUI::EnemyRight] = newUIPrefab;
}

void GameUIManager::CreateBattleLog(Odyssey::Scene* _sceneToAddTo)
{
	// Create the battle log object
	mBattleLog = _sceneToAddTo->createEntity();
	mBattleLog->addComponent<Odyssey::UICanvas>();
	// Get the canvas
	mCombatLogCanvas = mBattleLog->getComponent<Odyssey::UICanvas>();

	// Setup init values
	UINT width = battleTextWidth = (screenWidth / 4) + 50;
	UINT height = battleTextHeight = (screenHeight / 16);

	DirectX::XMFLOAT2 position = { 10.0f, (static_cast<float>(screenHeight) / 3.0f) + 280.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 0.0f, 0.0f, 1.0f }; // Color
	
	// Create background rectangle
	position.x -= 5.0f;
	position.y -= 65.0f;
	Odyssey::Rectangle2D* tempBackground = mCombatLogCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), 140, 95);
	tempBackground->setOpacity(0.65f);

	position.x += 5.0f;
	position.y += 65.0f;

	// Setup text properties
	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 12.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";

	position.x += 2.0f;

	float spriteSize = 25.0f;
	float heightDifference = spriteSize + 5.0f;
	float widthDifference = spriteSize + 5.0f;

	Odyssey::Sprite2D* iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatCasterIcons.push_back(iconPointer);
	position.x += widthDifference;
	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatSkillIcons.push_back(iconPointer);
	position.x += widthDifference;
	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatTargetIcons.push_back(iconPointer);
	//Combat Text
	position.x += widthDifference;
	mBattleLogText = mCombatLogCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"", properties);
	mBattleLogVec.push_back(mBattleLogText);

	position.x -= widthDifference * 3.0f;
	position.y -= heightDifference;

	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatCasterIcons.push_back(iconPointer);
	position.x += widthDifference;
	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatSkillIcons.push_back(iconPointer);
	position.x += widthDifference;
	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatTargetIcons.push_back(iconPointer);
	//Combat Text
	position.x += widthDifference;
	mBattleLogText = mCombatLogCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"", properties);
	mBattleLogVec.push_back(mBattleLogText);

	position.x -= widthDifference * 3.0f;
	position.y -= heightDifference;

	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatCasterIcons.push_back(iconPointer);
	position.x += widthDifference;
	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatSkillIcons.push_back(iconPointer);
	position.x += widthDifference;
	iconPointer = mCombatLogCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Blank.png", 25, 25);
	mCombatTargetIcons.push_back(iconPointer);
	//Combat Text
	position.x += widthDifference;
	mBattleLogText = mCombatLogCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"", properties);
	mBattleLogVec.push_back(mBattleLogText);

	mCombatLogCanvas->setActive(false);
}

void GameUIManager::SetBattleLogText(std::wstring newText, bool concat)
{
	if (concat)
	{
		mBattleLogVec[0]->setText(mBattleLogVec[0]->getText().append(L" "));
		mBattleLogVec[0]->setText(mBattleLogVec[0]->getText().append(newText));
		return;
	}
  
	for (int i = static_cast<int>(mBattleLogVec.size()) - 1; i > 0; i--)
	{
		if (i - 1 >= 0)
			mBattleLogVec[i]->setText(mBattleLogVec[i-1]->getText());
	}
	
	mBattleLogVec[0]->setText(newText);
}

// Be able to turn a canvas on and off
void GameUIManager::ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive)
{
	// Set the passed in entity's canvas to active or deactived based on the bool _isActive.
	_canvas->setActive(_isActive);
}

// This is where I will design and add all the elements into the main menu screen
void GameUIManager::CreateMainMenuCanvas(Odyssey::Scene* _sceneToAddTo)
{
	//Create the pause menu pointer
	mMainMenu = _sceneToAddTo->createEntity();
	mMainMenuOptions = _sceneToAddTo->createEntity();
	mMainMenuVolume = _sceneToAddTo->createEntity();
	mMainControls = _sceneToAddTo->createEntity();
	mMainMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the main menu
	Odyssey::UICanvas* mainMenuCanvas = mMainMenu->getComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* mainMenuVolumeCanvas = mMainMenuVolume->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* mainMenuOptionsCanvas = mMainMenuOptions->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* mainMenuControlsCanvas = mMainControls->addComponent<Odyssey::UICanvas>();

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
	properties.fontName = L"Tw Cen MT Condensed";

	// Create title text
	height = 100;
	mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"The Dying Dungeon", properties);

	// Create new game text 
	properties.bold = false;
	properties.fontSize = 40.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	position.x += 200.0f;
	position.y += 250.0f;
	width = 200;
	height = 50;
	// TODO: MAYBE COMMENT THIS IN? LOOKS DUMB
	//mainMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f), width, height);
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mNewGameText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"New Game", properties);
	position.y += 70.0f;
	width += 70;
	mStatsText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Game Statistics", properties);

	// TODO: M3B1 ONLY REFACTOR LATER
	position.y += 70.0f;
	width += 70;
	mMainOptionsText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, 60, L"Options", properties);
	position.y += 70.0f;
	width += 70;
	mCreditsText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Credits", properties);
	position.y += 70.0f;
	width += 70;
	mExitGameText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Exit Game", properties);
	// TODO: M3B1 ONLY END

	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	// Create the pause menu smaller black rectangle
	width = 640;
	height = 380;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	// Add the rectangle to the pause menu canvas
	mainMenuOptionsCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height)->setOpacity(BackgroundSmallOpacity);

	properties.textAlignment = Odyssey::TextAlignment::Center;
	width /= (UINT)2.5f;
	height = 50;

	properties.fontSize = 50.0f;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };

	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 160.0f;
	mainMenuOptionsCanvas->addElement<Odyssey::Text2D>(position, color, width, height + 20, L"Options", properties);

	position.y += 115.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the reusume background to the canvas
	mOptionsVolume = mainMenuOptionsCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 30.0f;
	mainMenuOptionsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Sound", properties);

	position.y += 75.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the options background to the canvas
	mOptionsControls = mainMenuOptionsCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Options text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mainMenuOptionsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Controls", properties);

	position.y += 75.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the options background to the canvas
	mOptionsBackButton = mainMenuOptionsCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Options text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mainMenuOptionsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Back", properties);

	properties.fontSize = 25.0f;
	mainMenuControlsCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/KeyGuide.png", screenWidth, screenHeight);
	mMainControlsBack = mainMenuControlsCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(0.0f, 0.0f), color, 100, 300, L"Back", properties);

	properties.textAlignment = Odyssey::TextAlignment::Left;

	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	// Create the pause menu smaller black rectangle
	width = 640;
	height = 380;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	// Add the rectangle to the pause menu canvas
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height)->setOpacity(BackgroundSmallOpacity);
	
	// Adjust volume text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 20.0f;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	//position.y -= 10.0f;
	position.x += (width / 2.0f) - 40.0f;
	mVolumeText = mainMenuVolumeCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Volume", properties);
	position.y += 350.0f;
	mOptionsBack = mainMenuVolumeCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Back", properties);
	// Volume bar
	width /= 2.0f;
	height = 25;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 95.0f;
	// Create the volume bar background
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f), color, width + 5.0f, height + 5.0f);
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f + 75.0f), color, width + 5.0f, height + 5.0f);
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f + 150.0f), color, width + 5.0f, height + 5.0f);
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f + 225.0f), color, width + 5.0f, height + 5.0f);
	color = { 70.0f, 70.0f, 70.0f, 1.0f };
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), color, width, height);
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), color, width, height);
	mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), color, width, height);
	// Add the volume bar to the canvas
	color = { 50.0f, 180.0f, 255.0f, 1.0f };
	mMainVolumeBar[0] = mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mMainVolumeBar[1] = mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), color, width, height);
	mMainVolumeBar[2] = mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), color, width, height);
	mMainVolumeBar[3] = mainMenuVolumeCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), color, width, height);
	// TODO :: FILL THE RECTANGLE BASED ON THE VOLUME LEVEL
	mMainVolumeBar[0]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::None)) / 1000.0f);
	mMainVolumeBar[3]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Dialog)) / 1000.0f);
	mMainVolumeBar[1]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Background)) / 1000.0f);
	mMainVolumeBar[2]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::SFX)) / 1000.0f);

	// Create the plus and minus symbols that the user can click on to adjust the volume
	UINT imageWidth = 32;
	UINT imageHeight = 32;
	position.x -= static_cast<float>(imageWidth) * 1.5f;
	position.y -= 4.0f;

	// Minus Symbol
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mainMenuVolumeCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y), color, 70, 25, L"Master ", properties);
	mainMenuVolumeCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y + 75.0f), color, 70, 25, L"Music ", properties);
	mainMenuVolumeCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y + 150.0f), color, 70, 25, L"SFX ", properties);
	mainMenuVolumeCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y + 225.0f), color, 70, 30, L"Dialog ", properties);

	mMainMinusImage[0] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMainMinusImage[1] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMainMinusImage[2] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMainMinusImage[3] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), L"assets/images/minusSymbol.png", imageWidth, imageHeight);

	// Plus Symbol
	position.x += (static_cast<float>(imageWidth) * 1.5f) + width + (static_cast<float>(imageWidth) / 2.0f);
	mMainPlusImage[0] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mMainPlusImage[1] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mMainPlusImage[2] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mMainPlusImage[3] = mainMenuVolumeCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), L"assets/images/plusSymbol.png", imageWidth, imageHeight);

	mMainSaveVolumeConfermation = _sceneToAddTo->createEntity();
	Odyssey::UICanvas* saveVolumeSettingsConfermationCanvas = mMainSaveVolumeConfermation->addComponent<Odyssey::UICanvas>();

	position = { (screenWidth / 2.0f) - (static_cast<float>(width / 2) / 2.0f), (screenHeight / 2.0f) - (static_cast<float>(height * 6) / 2.0f) };

	saveVolumeSettingsConfermationCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width / 2, height * 6)->setOpacity(1.0f);
	/*position.y -= 50.0f;
	position.x -= 320.0f;*/
	position.y += 10.0f;
	//saveVolumeSettingsConfermationCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width / 2, height * 2, L"Are you sure you want to overrid?", properties)->setFontSize(40.0f);
	//mSaveVolumeConfermationButtonsMain[1] = saveVolumeSettingsConfermationCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 130.0f, position.y + 130.0f), L"assets/images/TeamSelectionImages/SmallBoard.png", width / 10, height * 2);
	//properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	//saveVolumeSettingsConfermationCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 130.0f, position.y + 130.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width / 10, height * 2, L"Yes", properties);
	//mSaveVolumeConfermationButtonsMain[0] = saveVolumeSettingsConfermationCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 370.0f, position.y + 130.0f), L"assets/images/TeamSelectionImages/SmallBoard.png", width / 10, height * 2);
	//saveVolumeSettingsConfermationCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 370.0f, position.y + 130.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width / 10, height * 2, L"No", properties);

	mMainSaveVolumeConfermation->setActive(false);

	mMainMenuOptions->setVisible(false);
	mMainMenuOptions->setActive(false);

	mMainMenuVolume->setActive(false);
	
	mMainControls->setActive(false);
	mMainControls->setVisible(false);

	// Setup team logo
	mApeBackground = mainMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/GreatApeStudios_Background_Head.png", screenWidth, screenHeight);
	// Set the laser image to animate
	mAnimatedLaser = mainMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/GreatApeStudiosBlast.png", screenWidth, screenHeight);
}

// This is where I will design and add all the elements into the tower select screen
void GameUIManager::CreateTowerSelectMenuCanvas(Odyssey::Scene* _sceneToAddTo)
{
	// Create the tower select menu pointer
	mTowerSelectMenu = _sceneToAddTo->createEntity();
	mTowerSelectMenu->addComponent<Odyssey::UICanvas>();
	mTowerInfoCanvas = mTowerSelectMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* towerSelectMenuCanvas = mTowerSelectMenu->getComponent<Odyssey::UICanvas>();

	// Initialize variables
	UINT width = screenWidth; // Width
	UINT height = 50; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	Odyssey::TextProperties properties;

	// Create tower select title
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 50.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";
	mTowerSelectTitle = towerSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Select A Tower To Enter", properties);

	// Create door to click on
	width = 195;
	height = 231;
	position = { 50.83f, (static_cast<float>(screenHeight) - static_cast<float>(height))};
	Odyssey::Sprite2D* doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x +=  width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x += width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x += width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x += width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	properties.fontSize = 35.0f;
	properties.bold = false;
	mTowerBackButton = towerSelectMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(10.0f, 10.0f), color, 100, 60, L"Back", properties);
	properties.fontSize = 50.0f;
	properties.bold = true;

	// Start creating the tower info popup
	width = 500;
	height = 300;
	position.x = (static_cast<float>(screenWidth) / 2.0f) - (static_cast<float>(width) / 2.0f);
	position.y = 100.0f;
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	Odyssey::Rectangle2D* rect = mTowerInfoCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	rect->setOpacity(BackgroundSmallOpacity);

	// Add in tower level text
	height = 35;
	// Give a little padding
	position.x += 5.0f;
	position.y += 5.0f;
	// Set text elements
	properties.fontSize = 25.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Tower Level: 1", properties);

	// Add in enemies text and icons
	position.y += static_cast<float>(height) + 10.0f;
	float previousX = position.x;
	width = 120;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Enemies:", properties);
	// Add in the icons
	position.x += static_cast<float>(width);
	width = 64;
	height = 64;
	mTowerInfoCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/SkeletonPortrait.png", width, height);
	position.x += static_cast<float>(width) + 10.0f;
	mTowerInfoCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/GanfaulPortrait.jpg", width, height);

	// Set up rewards text
	position.x = previousX;
	position.y += static_cast<float>(height) + 5.0f;
	width = 300;
	height = 35;
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Rewards: 900 XP", properties);

	// Set up description
	position.y += static_cast<float>(height) + 20.0f;
	width = 300;
	height = 25;
	properties.fontSize = 15.0f;
	properties.italic = true;
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Description:", properties);
	position.y += static_cast<float>(height) + 5.0f;
	width = 475;
	height = 275;
	properties.fontSize = 12.0f;
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"A grand pair of worn statues in a overcast mountain top marks the entrance to this dungeon. Beyond the pair of worn statues lies a grand, humid room. It's covered in remains, ash and ash. Your torch allows you to see carved out openings filled with pottery, worn and ravished by time itself.", properties);
	
	// Setup Tutorial Button
	position.x = screenWidth - 180;
	position.y = 10;
	mTutorialButton = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", 165, 62);
	position.y += 10;
	position.x += 27;
	properties.fontSize = 30;
	properties.italic = false;
	towerSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Tutorial", properties);

	// Disable the tower info canvas
	mTowerInfoCanvas->setActive(false);
}

void GameUIManager::CreateTeamSelectMenuCanvas(Odyssey::Scene* _sceneToAddTo)
{
	// Create the tower select menu pointer
	mTeamSelectMenu = _sceneToAddTo->createEntity();
	
	// Get canvas component of the pause menu
	Odyssey::UICanvas* teamSelectMenuCanvas = mTeamSelectMenu->addComponent<Odyssey::UICanvas>();// mTeamSelectMenu->getComponent<Odyssey::UICanvas>();

	// Initialize variables
	UINT width = screenWidth; // Width
	UINT height = 50; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	Odyssey::TextProperties properties;

	// Create team select title
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 50.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Select 3 Team Members", properties);
	properties.fontSize = 35.0f;
	properties.bold = false;
	mTeamSelectBackButton = teamSelectMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(10.0f, 10.0f), color, 100, 60, L"Back", properties);

	//Create the save loadout button
	position.y += 65.0f;
	position.x -= 140.0f;
	properties.fontSize = 35.0f;
	properties.bold = true;
	mSaveLoadoutButton = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 510.0f, position.y), L"assets/images/TeamSelectionImages/SmallBoard.png", width/5, height);
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height, L"Save Loadout", properties);
	
	//Create the load loadout button
	position.x += 280.0f;
	mLoadLoadoutButton = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 510.0f, position.y), L"assets/images/TeamSelectionImages/SmallBoard.png", width/5, height);
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height, L"Load Loadout", properties);

	//Adding loadout pop-up menu
	mLoadoutMenu = _sceneToAddTo->createEntity();
	Odyssey::UICanvas* loadoutMenuCanvas = mLoadoutMenu->addComponent<Odyssey::UICanvas>();

	loadoutMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), screenWidth, screenHeight)->setOpacity(0.5f);
	position.y += 90.0f;
	position.x += 370.0f;
	loadoutMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width/5, height*7 + 10)->setOpacity(0.8f);

	position.y += 40.0f;
	position.x -= 510.0f;
	for (int i = 0; i < 3; i++)
	{
		mLoadoutButtons[i] = loadoutMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 530.0f, position.y), L"assets/images/TeamSelectionImages/SmallBoard.png", width / 6, height);
		loadoutMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height, std::wstring(L"Loadout " + std::to_wstring(i + 1)), properties);
		
		mLoadoutPortraitBackground[i] = loadoutMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 780.0f, position.y - static_cast<float>(height / 8)), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width / 6, (height + height/4));
		mLoadoutPortraitBackground[i]->setOpacity(0.8f);
		mLoadoutPortraitBackground[i]->setVisible(false);

		mLoadoutPortraits[i][0] = loadoutMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 720.0f + static_cast<float>(width / 18), position.y + static_cast<float>(height / 16)), L"assets/images/PaladinPortrait.jpg", width / 28, width / 28);
		mLoadoutPortraits[i][0]->setVisible(false);
		
		mLoadoutPortraits[i][1] = loadoutMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 720.0f + (static_cast<float>(width / 18)*2.0f), position.y + static_cast<float>(height / 16)), L"assets/images/PaladinPortrait.jpg", width / 28, width / 28);
		mLoadoutPortraits[i][1]->setVisible(false);
		
		mLoadoutPortraits[i][2] = loadoutMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 720.0f + (static_cast<float>(width / 18)*3.0f), position.y + static_cast<float>(height / 16)), L"assets/images/PaladinPortrait.jpg", width / 28, width / 28);
		mLoadoutPortraits[i][2]->setVisible(false);

		position.y += 100.0f;
	}
	position.y -= 40.0f;
	mCancelLoadoutButton = loadoutMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width, height, L"Cancel", properties);

	mLoadoutMenu->setActive(false);

	//Adding confermation pop-up
	mSaveLoadoutConfermation = _sceneToAddTo->createEntity();
	Odyssey::UICanvas* saveLoadoutConfermationCanvas = mSaveLoadoutConfermation->addComponent<Odyssey::UICanvas>();

	position = { (screenWidth / 2.0f) - (static_cast<float>(width/2) / 2.0f), (screenHeight / 2.0f) - (static_cast<float>(height*6) / 2.0f) };

	saveLoadoutConfermationCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width/2, height*6)->setOpacity(1.0f);
	/*position.y -= 50.0f;
	position.x -= 320.0f;*/
	position.y += 10.0f;
	saveLoadoutConfermationCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width/2, height*2, L"Are you sure you want to override this loadout?", properties)->setFontSize(40.0f);
	mSaveConfermationButtons[1] = saveLoadoutConfermationCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 130.0f, position.y + 130.0f), L"assets/images/TeamSelectionImages/SmallBoard.png", width / 10, height*2);
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	saveLoadoutConfermationCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 130.0f, position.y + 130.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width / 10, height * 2, L"Yes", properties);
	mSaveConfermationButtons[0] = saveLoadoutConfermationCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 370.0f, position.y + 130.0f), L"assets/images/TeamSelectionImages/SmallBoard.png", width / 10, height*2);
	saveLoadoutConfermationCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 370.0f, position.y + 130.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width / 10, height * 2, L"No", properties);

	mSaveLoadoutConfermation->setActive(false);

	// Create the confirm button to enter battle
	width = 227;
	height = 85;
	float padding = 20.0f;
	position = { (static_cast<float>(screenWidth - width) - padding), padding };
	// Create button background
	mEnterBattleButton = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height);
	// Create confrim text
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Enter Battle", properties);

	// Make the arrow sprites that the user will click to change characters
	width = 64;
	height = 64;
	std::wstring arrowForwardFilepath = L"assets/images/TeamSelectionImages/ArrowRight.png";
	std::wstring arrowBackFilepath = L"assets/images/TeamSelectionImages/ArrowLeft.png";
	Odyssey::Sprite2D* newArrow;
	// Create the arrows from left to right, back1, forward1, back2, etc...
	position = { 106.0f, static_cast<float>(screenHeight) - 100.0f };
	newArrow = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, arrowBackFilepath, width, height);
	mTeamSelectionArrows.push_back(newArrow);
	position = { 370.0f, static_cast<float>(screenHeight) - 100.0f };
	newArrow = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, arrowForwardFilepath, width, height);
	mTeamSelectionArrows.push_back(newArrow);
	position = { 476.0f, static_cast<float>(screenHeight) - 100.0f };
	newArrow = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, arrowBackFilepath, width, height);
	mTeamSelectionArrows.push_back(newArrow);
	position = { 740.0f, static_cast<float>(screenHeight) - 100.0f };
	newArrow = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, arrowForwardFilepath, width, height);
	mTeamSelectionArrows.push_back(newArrow);
	position = { 846.0f, static_cast<float>(screenHeight) - 100.0f };
	newArrow = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, arrowBackFilepath, width, height);
	mTeamSelectionArrows.push_back(newArrow);
	position = { 1110.0f, static_cast<float>(screenHeight) - 100.0f };
	newArrow = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, arrowForwardFilepath, width, height);
	mTeamSelectionArrows.push_back(newArrow);

	// Set the name slots in between the arrows
	width = 200;
	height = 64;
	// Create the background of the text
	position = { 170.0f, static_cast<float>(screenHeight) - 100.0f };
	teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height);
	// Create the name slot
	mNameSlots.push_back(teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Paladin", properties));
	// Create the background of the text
	position = { (static_cast<float>(screenWidth) / 2.0f) - (static_cast<float>(width) / 2.0f), static_cast<float>(screenHeight) - 100.0f };
	teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height);
	// Create the name slot
	mNameSlots.push_back(teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Mage", properties));
	// Create the background of the text
	position = { 910.0f, static_cast<float>(screenHeight) - 100.0f };
	teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height);
	// Create the name slot
	mNameSlots.push_back(teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Bard", properties));

	// Set the "Show Info" info buttons for the characters when selecting them
	width = 100;
	height = 32;
	properties.fontSize = 17.5f;
	// Create the background of the text
	position = { 220.0f, static_cast<float>(screenHeight) - 135.0f };
	mShowInfoButtons.push_back(teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height));
	// Create the Show Info text
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Show Info", properties);
	// Create the background of the text
	position = { (static_cast<float>(screenWidth) / 2.0f) - (static_cast<float>(width) / 2.0f), static_cast<float>(screenHeight) - 135.0f };
	mShowInfoButtons.push_back(teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height));
	// Create the Show Info text
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Show Info", properties);
	// Create the background of the text
	position = { 960.0f, static_cast<float>(screenHeight) - 135.0f };
	mShowInfoButtons.push_back(teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TeamSelectionImages/SmallBoard.png", width, height));
	// Create the Show Info text
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Show Info", properties);
}

// This is where I will design and add all elements into the pause menu canvas
void GameUIManager::CreatePauseMenuCanvas(Odyssey::Scene* _sceneToAddTo)
{
	//Create the pause menu pointer
	mPauseMenu = _sceneToAddTo->createEntity();
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
	properties.fontName = L"Tw Cen MT Condensed";
	mPauseTitle = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, 640, 60, L"Paused", properties);

	// Resume Button
	width /= (UINT)2.5f;
	height = 50;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 75.0f;//change back to 50.0f later
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
	mOptionsVolumeBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Options text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mOptionsVolumeText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"VOLUME", properties);

	position.y += 75.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the options background to the canvas
	mOptionsControlsBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Options text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mOptionsControlText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"CONTROLS", properties);

	// Main Menu Button
	position.y += 75.0f;
	color = { 180.0f, 30.0f, 30.0f, 1.0f };
	// Add the main menu background to the canvas
	mMainMenuBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Main menu text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mMainMenuText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"MAIN MENU", properties);

	mOptionsControlsImage = pauseMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/KeyGuide.png", screenWidth, screenHeight);
	mOptionsControlsImage->setVisible(false);

	properties.textAlignment = Odyssey::TextAlignment::Left;
	mOptionsControlBackText = pauseMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(0.0f, 0.0f), color, width, height, L"Back", properties);
	mOptionsControlBackText->setVisible(false);
	properties.textAlignment = Odyssey::TextAlignment::Center;

	// Turn off the canvas when creating it
	ToggleCanvas(mPauseMenu->getComponent<Odyssey::UICanvas>(), false);

	// Make options menu
	CreateOptionsMenu(_sceneToAddTo);
}

void GameUIManager::CreateStatsMenuCanvas(Odyssey::Scene* _sceneToAddTo)
{
	//Create the pause menu pointer
	mStatsMenu = _sceneToAddTo->createEntity();
	mStatsMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the main menu
	Odyssey::UICanvas* statsMenuCanvas = mStatsMenu->getComponent<Odyssey::UICanvas>();

	// Setup init values
	//UINT width = screenWidth;
	//UINT height = screenHeight;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT2 graphPosition = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 31.0f, 255.0f, 244.0f, 1.0f }; // Color
	// Set up text properties
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 42.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";

	UINT graphBackgroundWidth = 750;
	UINT graphBackgroundHeight = 450;

	position.x = (screenWidth / 2.0f) - (static_cast<float>(graphBackgroundWidth) / 2.0f);
	position.y = (screenHeight / 2.0f) - (static_cast<float>(graphBackgroundHeight) / 2.0f) + 30.0f;

	//Menu Set-up
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.fontSize = 20.0f;
	position.y += 40.0f;

	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y - 30.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), (graphBackgroundWidth/15), 5);
	statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 60.0f, position.y - 45.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 12), L"Active Character", properties);
	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 225.0f, position.y - 30.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth - 225, 5);

	mStatsBannerBackground[0] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(140.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight/6));
	mStatsPortraitBackground[0] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 30.0f, position.y - 7.0f), DirectX::XMFLOAT4(100.0f, 0.0f, 0.0f, 1.0f), (graphBackgroundHeight / 5), (graphBackgroundHeight/5));
	mStatsPortraits[0] = statsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 40.0f, position.y + 3.0f), L"assets/images/Dying_Dungeon_Logo_sharp.png", (graphBackgroundHeight / 5) - 20, (graphBackgroundHeight/5) - 20);
	properties.fontSize = 17.0f;
	mStatsHeaderBlocks[0] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 180.0f, position.y - 5.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 12), L"Turn Action\tMove       \tBuff/Debuff\tBuff/Debuff Value", properties);
	mActiveCharacterTurnAction = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 205.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"None", properties);
	mActiveCharacterMove = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 295.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"No Action", properties);
	mActiveCharacterBuffDebuff = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 465.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"Stat Down", properties);
	mActiveCharacterBuffDebuffValue = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 640.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	properties.fontSize = 20.0f;

	
	position.y += 50.0f + (graphBackgroundHeight / 6);
	//100/140 = 0.7143f
	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y - 30.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 255.0f, 1.0f), (graphBackgroundWidth / 15), 5);
	statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 60.0f, position.y - 45.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 255.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 12), L"Targets", properties);
	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 135.0f, position.y - 30.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 255.0f, 1.0f), graphBackgroundWidth - 135, 5);

	mStatsBannerBackground[1] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 140.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 6));
	mStatsPortraitBackground[1] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 30.0f, position.y - 7.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 100.0f, 1.0f), (graphBackgroundHeight / 5), (graphBackgroundHeight / 5));
	mStatsPortraits[1] = statsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 40.0f, position.y + 3.0f), L"assets/images/Dying_Dungeon_Logo_sharp.png", (graphBackgroundHeight / 5) - 20, (graphBackgroundHeight / 5) - 20);

	properties.fontSize = 17.0f;
	mStatsHeaderBlocks[1] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 180.0f, position.y - 5.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 12), L"Damage       Buff/Debuff         Buff/Debuff Value       Damage Mitigated", properties);
	mTargetDamage[0] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 195.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	mTargetCharacterBuffDebuff[0] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 285.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"Stat Down", properties);
	mTargetCharacterBuffDebuffValue[0] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 455.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	mTargetCharacterDamageMitigated[0] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 640.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	properties.fontSize = 20.0f;
	
	position.y += 20.0f + (graphBackgroundHeight / 6);
	mStatsBannerBackground[2] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 140.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 6));
	mStatsPortraitBackground[2] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 30.0f, position.y - 7.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 100.0f, 1.0f), (graphBackgroundHeight / 5), (graphBackgroundHeight / 5));
	mStatsPortraits[2] = statsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 40.0f, position.y + 3.0f), L"assets/images/Dying_Dungeon_Logo_sharp.png", (graphBackgroundHeight / 5) - 20, (graphBackgroundHeight / 5) - 20);

	properties.fontSize = 17.0f;
	mStatsHeaderBlocks[2] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 180.0f, position.y - 5.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 12), L"Damage       Buff/Debuff         Buff/Debuff Value       Damage Mitigated", properties);
	mTargetDamage[1] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 195.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	mTargetCharacterBuffDebuff[1] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 285.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"Stat Down", properties);
	mTargetCharacterBuffDebuffValue[1] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 455.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	mTargetCharacterDamageMitigated[1] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 640.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	properties.fontSize = 20.0f;

	position.y += 20.0f + (graphBackgroundHeight / 6);
	mStatsBannerBackground[3] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 140.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 6));
	mStatsPortraitBackground[3] = statsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x + 30.0f, position.y - 7.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 100.0f, 1.0f), (graphBackgroundHeight / 5), (graphBackgroundHeight / 5));
	mStatsPortraits[3] = statsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x + 40.0f, position.y + 3.0f), L"assets/images/Dying_Dungeon_Logo_sharp.png", (graphBackgroundHeight / 5) - 20, (graphBackgroundHeight / 5) - 20);

	properties.fontSize = 17.0f;
	mStatsHeaderBlocks[3] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 180.0f, position.y - 5.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight / 12), L"Damage       Buff/Debuff         Buff/Debuff Value       Damage Mitigated", properties);
	mTargetDamage[2] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 195.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	mTargetCharacterBuffDebuff[2] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 285.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"Stat Down", properties);
	mTargetCharacterBuffDebuffValue[2] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 455.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	mTargetCharacterDamageMitigated[2] = statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x + 640.0f, position.y + 20.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, (graphBackgroundHeight / 12), L"00.00", properties);
	properties.fontSize = 20.0f;

	graphPosition.x = position.x = (screenWidth / 2.0f) - (static_cast<float>(graphBackgroundWidth) / 2.0f);
	graphPosition.y = position.y = (screenHeight / 2.0f) - (static_cast<float>(graphBackgroundHeight) / 2.0f) + 30.0f;

	/*position.x -= 3;*/
	position.y -= 50.0f;
	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight/12));
	statsMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight/12), L"No Round Data", properties);
	position.y -= 50.0f;
	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight/12));
	statsMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, (graphBackgroundHeight/12), L"No Level Data", properties);


	//Button Set-up
	position.y = static_cast<float>(graphBackgroundHeight) - 30.0f;

	properties.fontSize = 30.0f;

	mStatsBackButtonText = statsMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, graphBackgroundHeight, L"Back", properties);
	mStatsBackButtonText->setVisible(true);
	position.x -= 60.0f;
	position.y -= (static_cast<float>(graphBackgroundHeight) / 2.0f) + 90.0f;
	mStatsPrevButtonTurn = statsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Arrow_L.png", graphBackgroundWidth / 12, graphBackgroundWidth / 12);
	position.y -= 60.0f;
	mStatsPrevButtonLevel = statsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Arrow_L.png", graphBackgroundWidth / 12, graphBackgroundWidth / 12);
	position.y += 60.0f;
	position.x += static_cast<float>(graphBackgroundWidth) + 60.0f;
	mStatsNextButtonTurn = statsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Arrow_R.png", graphBackgroundWidth / 12, graphBackgroundWidth / 12);
	position.y -= 60.0f;
	mStatsNextButtonLevel = statsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Arrow_R.png", graphBackgroundWidth / 12, graphBackgroundWidth / 12);

	/*position.y += 270.0f;
	position.x -= (static_cast<float>(graphBackgroundWidth)/2.0f) + 500.0f;
	mStatsPrevButtonTurn = statsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Arrow_L.png", graphBackgroundWidth/6, graphBackgroundWidth / 6);
	position.x += static_cast<float>(graphBackgroundWidth) + 120.0f;
	mStatsNextButtonTurn = statsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Arrow_R.png", graphBackgroundWidth / 6, graphBackgroundWidth / 6);*/
	
	ToggleCanvas(mStatsMenu->getComponent<Odyssey::UICanvas>(), false);

}

void GameUIManager::CreateCreditsMenuCanvas(Odyssey::Scene* _sceneToAddTo)
{
	// Create the credits menu entity and add a canvas to it
	mCreditsMenu = _sceneToAddTo->createEntity();
	Odyssey::UICanvas* creditsCanvas = mCreditsMenu->addComponent<Odyssey::UICanvas>();

	// Set up text properties
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 42.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";

	// Setup the background
	UINT width = 900;
	UINT height = 600;
	UINT pad = 25;
	DirectX::XMFLOAT2 position = { 190, 60 }; // Position
	DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, BackgroundBigOpacity }; // Color
	creditsCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);

	// Setup the title
	color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	height = 50;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Credits & Contact Info", properties);


	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;

	// Setup the first credit - 1st Row
	color = DirectX::XMFLOAT4(31.0f, 255.0f, 203.0f, 1.0f);
	position.y += 75;
	position.x += pad;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 18.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Max Nastri - Odyssey Engine Creator", properties);
	position.y += 35;
	position.x += pad;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Email: Max.Nastri@gmail.com", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Phone: (941) 303-9809", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Portfolio: GameDevPortfolio.net", properties);

	// Setup the first credit - 2nd Row
	color = DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f);
	position.y += 75;
	position.x -= pad;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 18.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Tristen Maturski - Lead Gameplay Developer", properties);
	position.y += 35;
	position.x += pad;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Email: Maturski62@gmail.com", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Phone: (413) 672-1013", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Portfolio: TheDyingDungeon.home.blog", properties);

	// Setup the second credit - 1st Row
	color = DirectX::XMFLOAT4(31.0f, 255.0f, 75.0f, 1.0f);
	position.y = 135;
	position.x += 425;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 18.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Lane Langford - AI/Gameplay Developer", properties);
	position.y += 35;
	position.x += pad;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Email: LaneLangford.2020@gmail.com", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Phone: (352) 275-7723", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Portfolio: TheDyingDungeon.home.blog", properties);

	// Setup the second credit - 2nd Row
	color = DirectX::XMFLOAT4(75.0f, 100.0f, 255.0f, 1.0f);
	position.y += 75;
	position.x -= pad;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 18.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Bryce Monkus - Lead AI Developer", properties);
	position.y += 35;
	position.x += pad;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Email: Brycemnks@gmail.com", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Phone: (704) 441-5402", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Portfolio: TheDyingDungeon.home.blog", properties);

	// Setup the third credit - 3rd Row
	color = DirectX::XMFLOAT4(255.0f, 35.0f, 35.0f, 1.0f);
	position.y += 75;
	position.x = position.x - pad - 190;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 18.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Jeffrey Ouma - Audio/Tools Developer", properties);
	position.y += 35;
	position.x += pad;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Email: Jwaostar@gmail.com", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Phone: (703) 419-5296", properties);
	position.y += 30;
	creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Portfolio: TheDyingDungeon.home.blog", properties);

	// Setup the back button
	position = DirectX::XMFLOAT2(220.0f, 610.0f);
	color = DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f);
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 24.0f;
	mCreditsBackText = creditsCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Back", properties);

	ToggleCanvas(mCreditsMenu->getComponent<Odyssey::UICanvas>(), false);
}

void GameUIManager::ShowStatsMenu()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	mStatsMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	mStatMenuCurrentLevel = 1;
	mStatMenuCurrentTurn = 1;
	mStatMenuCurrentRound = 1;
	mStatsPrevButtonTurn->registerCallback("onMouseClick", this, &GameUIManager::StatsMenuPrevTurn);
	mStatsNextButtonTurn->registerCallback("onMouseClick", this, &GameUIManager::StatsMenuNextTurn);
	mStatsBackButtonText->registerCallback("onMouseClick", this, &GameUIManager::HideStatsMenu);
	mStatsPrevButtonLevel->registerCallback("onMouseClick", this, &GameUIManager::StatsMenuPrevLevel);
	mStatsNextButtonLevel->registerCallback("onMouseClick", this, &GameUIManager::StatsMenuNextLevel);
	//mStatsNextButtonRound->registerCallback("onMouseClick", this, &GameUIManager::StatsMenuNextRound);
	//mStatsPrevButtonRound->registerCallback("onMouseClick", this, &GameUIManager::StatsMenuPrevRound);
	UpdateStatsMenu();
}

void GameUIManager::ShowCreditsMenu()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	mCreditsMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	mCreditsBackText->registerCallback("onMouseClick", &GameUIManager::getInstance(), &GameUIManager::HideCreditsMenu);
}

void GameUIManager::HideCreditsMenu()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	mCreditsMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	mCreditsBackText->unregisterCallback("onMouseClick");
}

void GameUIManager::ShowMainOptions()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	mMainMenuOptions->setActive(true);
	mMainMenuOptions->setVisible(true);
	mOptionsBackButton->registerCallback("onMouseClick", this, &GameUIManager::HideMainOptions);
	mOptionsVolume->registerCallback("onMouseClick", this, &GameUIManager::ShowMainVolume);
	mOptionsControls->registerCallback("onMouseClick", this, &GameUIManager::ShowControlsGuide);
}

void GameUIManager::HideMainOptions()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	mMainMenuOptions->setActive(false);
	mMainMenuOptions->setVisible(false);
	mOptionsBackButton->unregisterCallback("onMouseClick");
}

void GameUIManager::ShowMainVolume()
{
	mMainMenuOptions->setActive(false);
	mMainMenuVolume->setActive(true);
	mMainMenuOptions->setVisible(true);
	mOptionsBack->registerCallback("onMouseClick", this, &GameUIManager::HideMainVolume);
	mMainPlusImage[0]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseMasterVolume);
	mMainPlusImage[1]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseBackgroundVolume);
	mMainPlusImage[2]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseSFXVolume);
	mMainPlusImage[3]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseDialogVolume);

	mMainMinusImage[0]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseMasterVolume);
	mMainMinusImage[1]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseBackgroundVolume);
	mMainMinusImage[2]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseSFXVolume);
	mMainMinusImage[3]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseDialogVolume);
}

void GameUIManager::HideMainVolume()
{
	mMainMenuVolume->setActive(false);
	mMainMenuVolume->setVisible(false);

	mOptionsBack->unregisterCallback("onMouseClick");
	mMainPlusImage[0]->unregisterCallback("onMouseClick");
	mMainPlusImage[1]->unregisterCallback("onMouseClick");
	mMainPlusImage[2]->unregisterCallback("onMouseClick");
	mMainPlusImage[3]->unregisterCallback("onMouseClick");

	mMainMinusImage[0]->unregisterCallback("onMouseClick");
	mMainMinusImage[1]->unregisterCallback("onMouseClick");
	mMainMinusImage[2]->unregisterCallback("onMouseClick");
	mMainMinusImage[3]->unregisterCallback("onMouseClick");

	ShowMainOptions();
}

void GameUIManager::StatsMenuPrevTurn()
{
	if (StatTracker::Instance().GetLevelSize() > 0)
	{
		if (mStatMenuCurrentTurn > 1)
		{
			mStatMenuCurrentTurn--;
			UpdateStatsMenu();
		}
	}
}

void GameUIManager::StatsMenuNextTurn()
{
	if (StatTracker::Instance().GetLevelSize() > 0) {
		if (mStatMenuCurrentTurn < StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns.size())
		{
			mStatMenuCurrentTurn++;
			UpdateStatsMenu();
		}
		else if (((int)mStatMenuCurrentTurn + 1) > StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns.size())
		{
			mStatMenuCurrentTurn = 1;
			mStatMenuCurrentRound = 1;
			mStatMenuCurrentLevel++;
			if (mStatMenuCurrentLevel > StatTracker::Instance().GetLevelSize())
			{
				mStatMenuCurrentLevel = 1;
			}
			UpdateStatsMenu();
		}
	}
}

void GameUIManager::StatsMenuPrevLevel()
{
	if (StatTracker::Instance().GetLevelSize() > 0)
	{
		if (mStatMenuCurrentLevel > 1)
		{
			mStatMenuCurrentLevel--;
			mStatMenuCurrentTurn = 1;
			mStatMenuCurrentRound = 1;
			UpdateStatsMenu();
		}
	}
}

void GameUIManager::StatsMenuNextLevel()
{
	if (StatTracker::Instance().GetLevelSize() > 0)
	{
		if (mStatMenuCurrentLevel < StatTracker::Instance().GetLevelSize())
		{
			mStatMenuCurrentLevel++;
			mStatMenuCurrentTurn = 1;
			mStatMenuCurrentRound = 1;
			UpdateStatsMenu();
		}
		else if (((int)mStatMenuCurrentLevel + 1) > StatTracker::Instance().GetLevelSize())
		{
			mStatMenuCurrentLevel = 1;
			mStatMenuCurrentTurn = 1;
			mStatMenuCurrentRound = 1;
			UpdateStatsMenu();
		}
	}
}

void GameUIManager::StatsMenuNextRound()
{
	if (StatTracker::Instance().GetLevelSize() > 0)
	{
		if (mStatMenuCurrentRound < StatTracker::Instance().GetRoundCount(mStatMenuCurrentLevel))
		{
			mStatMenuCurrentRound++;
			mStatMenuCurrentTurn = StatTracker::Instance().GetFirstTurn(mStatMenuCurrentLevel, mStatMenuCurrentRound);
			UpdateStatsMenu();
		}
		else if ((mStatMenuCurrentRound + 1) > StatTracker::Instance().GetRoundCount(mStatMenuCurrentLevel))
		{
			mStatMenuCurrentTurn = 1;
			mStatMenuCurrentRound = 1;
			mStatMenuCurrentLevel++;
			if (mStatMenuCurrentLevel > StatTracker::Instance().GetLevelSize())
			{
				mStatMenuCurrentLevel = 1;
			}
			UpdateStatsMenu();
		}
	}
}

void GameUIManager::StatsMenuPrevRound()
{
	if (StatTracker::Instance().GetLevelSize() > 0)
	{
		if (mStatMenuCurrentRound > 1)
		{
			mStatMenuCurrentRound--;
			UpdateStatsMenu();
		}
	}
}

void GameUIManager::HideStatsMenu()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	mStatsMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	mStatsPrevButtonTurn->unregisterCallback("onMouseClick");
	mStatsNextButtonTurn->unregisterCallback("onMouseClick");
	//mStatsBackButtonText->unregisterCallback("onMouseClick");
	mStatsPrevButtonLevel->unregisterCallback("onMouseClick");
	mStatsNextButtonLevel->unregisterCallback("onMouseClick");
	//mStatsNextButtonRound->unregisterCallback("onMouseClick");
	//mStatsPrevButtonRound->unregisterCallback("onMouseClick");
}

void GameUIManager::ShowControlsGuide()
{
	mMainMenuOptions->setActive(false);

	mMainControls->setActive(true);
	mMainControls->setVisible(true);

	mMainControlsBack->registerCallback("onMouseClick", this, &GameUIManager::HideControlsGuide);

	mOptionsBackButton->unregisterCallback("onMouseClick");
	mOptionsVolume->unregisterCallback("onMouseClick");
	mOptionsControls->unregisterCallback("onMouseClick");
}

void GameUIManager::HideControlsGuide()
{
	mMainControls->setActive(false);
	mMainControls->setVisible(false);
	mMainControlsBack->unregisterCallback("onMouseClick");
	ShowMainOptions();
}

void GameUIManager::CreateTutorialCanvas(Odyssey::Scene* _sceneToAddTo)
{
	tutorialCounter = 0;

	mTutorialEntity = _sceneToAddTo->createEntity();
	mTutorialEntity->addComponent<Odyssey::UICanvas>();
	mTutorialCanvas = mTutorialEntity->getComponent<Odyssey::UICanvas>();

	DirectX::XMFLOAT2 position = { 0, 0 };
	UINT imageWidth = screenWidth;
	UINT imageHeight = screenHeight;

	mHighlightTutorial = mTutorialCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TutorialLevel/HighlightPlayer.png", imageWidth, imageHeight);
	mHighlightTutorial->setOpacity(.8f);

	mHighlightTutorialText = mTutorialCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TutorialLevel/ExplainPlayer.png", imageWidth, imageHeight);

	mHighlightTutorialContinueText = mTutorialCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/TutorialLevel/ExplainClickToContinue.png", imageWidth, imageHeight);

	mTutorialCanvas->setActive(false);
}

void GameUIManager::AssignCharacterHudElements(Character* _newCharacter, Odyssey::Entity* _newHud)
{
	// Get the hud elements to assign
	CharacterHUDElements* hudElements = _newHud->getComponent<CharacterHUDElements>();

	// Assign all of the elements
	if (_newCharacter->IsHero())
	{
		hudElements->ChangeAttackNumber(std::to_wstring((int)_newCharacter->GetAtk()));
		hudElements->ChangeDefenseNumber(std::to_wstring((int)_newCharacter->GetDef()));
		hudElements->ChangeSpeedNumber(std::to_wstring((int)_newCharacter->GetSpeed()));
		hudElements->ChangeManaNumber(std::to_wstring((int)_newCharacter->GetMana()) + L"/" + std::to_wstring((int)_newCharacter->GetMaxMana()));
		
		// Update the skills
		hudElements->ChangeSkill(_newCharacter->GetSkills()[0].get(), 1);
		hudElements->ChangeSkill(_newCharacter->GetSkills()[1].get(), 2);
		hudElements->ChangeSkill(_newCharacter->GetSkills()[2].get(), 3);
		hudElements->ChangeSkill(_newCharacter->GetSkills()[3].get(), 4);

		// Update skill background colors
		hudElements->ChangeSkillBackgroundColors(_newCharacter->GetThemeColor());
	}

	hudElements->ChangePortrait(_newCharacter->GetPortraitPath());
	hudElements->ChangeCharacterName(_newCharacter->GetName());
	hudElements->ChangeHealthNumber(std::to_wstring((int)_newCharacter->GetHP()) + L"/" + std::to_wstring((int)_newCharacter->GetMaxHP()));
}

void GameUIManager::SetupClickableCharacterUI()
{
	for (int i = 0; i < mClickableUIList.size(); i++)
	{
		Odyssey::Rectangle2D* rect = mClickableUIList[i]->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>();
		
		if (i == 0)
		{
			rect->registerCallback("onMouseClick", this, &GameUIManager::Character1ClickableCallback);
		}
		else if (i == 1)
		{
			rect->registerCallback("onMouseClick", this, &GameUIManager::Character2ClickableCallback);
		}
		else if (i == 2)
		{
			rect->registerCallback("onMouseClick", this, &GameUIManager::Character3ClickableCallback);
		}
		else if (i == 3)
		{
			rect->registerCallback("onMouseClick", this, &GameUIManager::Character4ClickableCallback);
		}
		else if (i == 4)
		{
			rect->registerCallback("onMouseClick", this, &GameUIManager::Character5ClickableCallback);
		}
		else if (i == 5)
		{
			rect->registerCallback("onMouseClick", this, &GameUIManager::Character6ClickableCallback);
		}

		// Assign on hover and on exit callbacks
		rect->registerCallback("onMouseEnter", this, &GameUIManager::CharacterEnterHoverCallback);
		rect->registerCallback("onMouseExit", this, &GameUIManager::CharacterExitHoverCallback);
	}
}

void GameUIManager::UpdateStatsMenu()
{
	if (StatTracker::Instance().GetLevelSize() > 0) {
		/*if (StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns.size() < mStatMenuCurrentTurn) 
		{
			mStatMenuCurrentTurn = 1;
			mStatMenuCurrentLevel++;
			if (mStatMenuCurrentLevel > StatTracker::Instance().GetLevelSize())
			{
				mStatMenuCurrentLevel = 1;
			}
		}*/
		
		Odyssey::UICanvas* statsMenuCanvas = mStatsMenu->getComponent<Odyssey::UICanvas>();

		std::string characterName = StatTracker::Instance().GetCharacterName(mStatMenuCurrentLevel, mStatMenuCurrentTurn);
		std::string actionName = StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].actionName;

		std::wstring _turnText;

		/*_turnText.append(L"Turn " + std::to_wstring(mStatMenuCurrentTurn) + L": " + std::wstring(characterName.begin(), characterName.end()) + L"\n\n" +
						 L"Action: " + std::wstring(actionName.begin(), actionName.end()) + L"\n\n" +
						 L"Targets: ");*/
		std::vector<std::string> _targetNames = StatTracker::Instance().GetTargetList(mStatMenuCurrentLevel, mStatMenuCurrentTurn);
		/*for (int i = 0; i < _targetNames.size(); i++)
		{
			if(i > 0)
			{
				_turnText.append(L"                 ");
			}
			_turnText.append(std::wstring(_targetNames[i].begin(), _targetNames[i].end()) + L"\n");
		}

		_turnText.append(L"\n");*/

		std::wstring buffordebuff;
		switch (StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].effect)
		{
		case EFFECTTYPE::None:
			buffordebuff = L"None";
			break;
		case EFFECTTYPE::Bleed:
			buffordebuff = L"Bleed";
			break;
		case EFFECTTYPE::Regen:
			buffordebuff = L"Regen";
			break;
		case EFFECTTYPE::StatUp:
			buffordebuff = L"StatUp";
			break;
		case EFFECTTYPE::StatDown:
			buffordebuff = L"StatDown";
			break;
		case EFFECTTYPE::Stun:
			buffordebuff = L"Stun";
			break;
		case EFFECTTYPE::Shield:
			buffordebuff = L"Shield";
			break;
		case EFFECTTYPE::Provoke:
			buffordebuff = L"Provoke";
			break;
		case EFFECTTYPE::Clense:
			buffordebuff = L"Clense";
			break;
		default:
			buffordebuff = L"None";
			break;
		}

		mActiveCharacterBuffDebuff->setText(buffordebuff);

		for (int n = 1; n <= 3; n++)
		{
			mTargetDamage[3 - n]->setVisible(true);
			mTargetCharacterBuffDebuff[3 - n]->setVisible(true);
			mTargetCharacterBuffDebuffValue[3 - n]->setVisible(true);
			mTargetCharacterDamageMitigated[3 - n]->setVisible(true);
			mStatsBannerBackground[3 - n + 1]->setVisible(true);
			mStatsPortraitBackground[3 - n + 1]->setVisible(true);
			mStatsPortraits[3 - n + 1]->setVisible(true);
			mStatsHeaderBlocks[3 - n + 1]->setVisible(true);
			mTargetCharacterBuffDebuff[3 - n]->setText(buffordebuff);

		}

		std::wstring portraitPath = StatTracker::Instance().GetCharacterPortrait(mStatMenuCurrentLevel, StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].unique_id);
		mStatsPortraits[0]->setSprite(portraitPath, mStatsPortraits[0]->getDimensions().x, mStatsPortraits[0]->getDimensions().y);
		StatTracker::Action testAction = StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].actionType;
		switch (testAction)
		{
		case StatTracker::Action::Attack:
		{
			//_turnText.append(L"\tDamage: " + StatTracker::Instance().FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value) /*+ "\n\tAttack Modifier: " + FormatToPercentage(m_levels[i].turns[j].attackModifier)*/ + L"\n\n");
			//for (int j = 0; j < _targetNames.size(); j++)
			//{
			//	_turnText.append(L"\tTarget " + std::to_wstring(j + 1) + L": " + std::wstring(_targetNames[j].begin(), _targetNames[j].end()) + L"\n\tDamage Mitigated: " + StatTracker::Instance().FormatToPercentageW(StatTracker::Instance().roundf(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets[j].second, 2)) + L"\n\t    Danage Taken: " + StatTracker::Instance().FormatToPercentageW((StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value - (StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value * StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets[j].second))) + L"\n\n");
			//}
			mActiveCharacterTurnAction->setText(L"Attack");
			mActiveCharacterMove->setText(Converter::ConvertCharToWStr(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].actionName.c_str()));
			//std::vector<float> buffdebuffValue = StatTracker::Instance().GetTargetDebuffBuffValues(mStatMenuCurrentLevel, mStatMenuCurrentTurn);
			for (int j = 0; j < StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets.size(); j++) {
				portraitPath = StatTracker::Instance().GetCharacterPortrait(mStatMenuCurrentLevel, StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets[j].first.unique_id);
				mStatsPortraits[j + 1]->setSprite(portraitPath, mStatsPortraits[j + 1]->getDimensions().x, mStatsPortraits[j + 1]->getDimensions().y);
				mTargetDamage[j]->setVisible(true);
				mTargetCharacterBuffDebuff[j]->setVisible(true);
				mTargetCharacterBuffDebuffValue[j]->setVisible(true);
				mTargetCharacterDamageMitigated[j]->setVisible(true);
				mTargetDamage[j]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value, 2));
				mTargetCharacterDamageMitigated[j]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets[j].second, 2));
			}
			for (int k = 0; k < StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets.size(); k++)
			{
				mTargetCharacterBuffDebuffValue[k]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets[k].second * 100.0f, 2) + L"%");
			}
			for (int o = 0; o < StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].debuffedTargets.size(); o++)
			{
				mTargetCharacterBuffDebuffValue[o]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].debuffedTargets[o].second*100.0f, 2) + L"%");
			}

			int temp = (3 - static_cast<int>(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets.size()));
			for (int m = 1; m <= temp; m++)
			{
				mTargetDamage[3 - m]->setVisible(false);
				mTargetCharacterBuffDebuff[3 - m]->setVisible(false);
				mTargetCharacterBuffDebuffValue[3 - m]->setVisible(false);
				mTargetCharacterDamageMitigated[3 - m]->setVisible(false);
				mStatsBannerBackground[3 - m + 1]->setVisible(false);
				mStatsPortraitBackground[3 - m + 1]->setVisible(false);
				mStatsPortraits[3 - m + 1]->setVisible(false);
				mStatsHeaderBlocks[3 - m + 1]->setVisible(false);
			}
		}
			break;
		case StatTracker::Action::Defend:
		{
			mActiveCharacterTurnAction->setText(L"Defend");
			mActiveCharacterMove->setText(Converter::ConvertCharToWStr(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].actionName.c_str()));
			if (StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].effect == EFFECTTYPE::Shield)
			{
				mActiveCharacterBuffDebuff->setText(L"Shield");
				//statsMenuCanvas->getElements<Odyssey::Text2D>()[5]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value, 2));
			}
			else
			{
				mActiveCharacterBuffDebuff->setText(L"Heal");
				//statsMenuCanvas->getElements<Odyssey::Text2D>()[5]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value, 2));
			}
			//_turnText.append(StatTracker::Instance().FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value));
			for (int j = 0; j < StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets.size(); j++) {
				portraitPath = StatTracker::Instance().GetCharacterPortrait(mStatMenuCurrentLevel, StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets[j].first.unique_id);
				mStatsPortraits[j + 1]->setSprite(portraitPath, mStatsPortraits[j + 1]->getDimensions().x, mStatsPortraits[j + 1]->getDimensions().y);
				mTargetDamage[j]->setVisible(true);
				mTargetCharacterBuffDebuff[j]->setVisible(true);
				mTargetCharacterBuffDebuffValue[j]->setVisible(true);
				mTargetCharacterDamageMitigated[j]->setVisible(true);
				mTargetDamage[j]->setText(L"00.00");
				mTargetCharacterDamageMitigated[j]->setText(L"00.00");
				mTargetCharacterBuffDebuffValue[j]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets[j].second, 2));
			}
			int temp = (3 - static_cast<int>(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets.size()));
			for (int m = 1; m <= temp; m++)
			{
				mTargetDamage[3 - m]->setVisible(false);
				mTargetCharacterBuffDebuff[3 - m]->setVisible(false);
				mTargetCharacterBuffDebuffValue[3 - m]->setVisible(false);
				mTargetCharacterDamageMitigated[3 - m]->setVisible(false);
				mStatsBannerBackground[3 - m + 1]->setVisible(false);
				mStatsPortraitBackground[3 - m + 1]->setVisible(false);
				mStatsPortraits[3 - m + 1]->setVisible(false);
				mStatsHeaderBlocks[3 - m + 1]->setVisible(false);
			}
		}
			break;
		case StatTracker::Action::Aid:
		{
			mActiveCharacterTurnAction->setText(L"Aid");
			mActiveCharacterMove->setText(Converter::ConvertCharToWStr(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].actionName.c_str()));
			if (StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].effect == EFFECTTYPE::Shield)
			{
				mActiveCharacterBuffDebuff->setText(L"Shield");
				//statsMenuCanvas->getElements<Odyssey::Text2D>()[5]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value, 2));
			}
			else
			{
				mActiveCharacterBuffDebuff->setText(L"Heal");
				//statsMenuCanvas->getElements<Odyssey::Text2D>()[5]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value, 2));
			}
			for (int j = 0; j < StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets.size(); j++) {
				portraitPath = StatTracker::Instance().GetCharacterPortrait(mStatMenuCurrentLevel, StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].targets[j].first.unique_id);
				mStatsPortraits[j + 1]->setSprite(portraitPath, mStatsPortraits[j + 1]->getDimensions().x, mStatsPortraits[j + 1]->getDimensions().y);
				mTargetDamage[j]->setVisible(true);
				mTargetCharacterBuffDebuff[j]->setVisible(true);
				mTargetCharacterBuffDebuffValue[j]->setVisible(true);
				mTargetCharacterDamageMitigated[j]->setVisible(true);
				mTargetDamage[j]->setText(L"00.00");
				mTargetCharacterDamageMitigated[j]->setText(L"00.00");
				mTargetCharacterBuffDebuffValue[j]->setText(Converter::FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets[j].second, 2));
			}
			int temp = (3 - static_cast<int>(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].buffedTargets.size()));
			for (int m = 1; m <= temp; m++)
			{
				mTargetDamage[3 - m]->setVisible(false);
				mTargetCharacterBuffDebuff[3 - m]->setVisible(false);
				mTargetCharacterBuffDebuffValue[3 - m]->setVisible(false);
				mTargetCharacterDamageMitigated[3 - m]->setVisible(false);
				mStatsBannerBackground[3 - m + 1]->setVisible(false);
				mStatsPortraitBackground[3 - m + 1]->setVisible(false);
				mStatsPortraits[3 - m + 1]->setVisible(false);
				mStatsHeaderBlocks[3 - m + 1]->setVisible(false);
			}
			//_turnText.append(StatTracker::Instance().FormatToPercentageW(StatTracker::Instance().GetLevel(mStatMenuCurrentLevel - 1).turns[mStatMenuCurrentTurn - 1].value));

		}
			break;
		default:
			int test = 0;
			mActiveCharacterMove->setText(L"None");
			mActiveCharacterTurnAction->setText(L"None");
			mActiveCharacterMove->setText(L"None");
			mActiveCharacterBuffDebuff->setText(L"None");
			mActiveCharacterBuffDebuffValue->setText(L"00.00");

			for (int n = 1; n <= 3; n++)
			{
				mTargetDamage[3 - n]->setVisible(false);
				mTargetCharacterBuffDebuff[3 - n]->setVisible(false);
				mTargetCharacterBuffDebuffValue[3 - n]->setVisible(false);
				mTargetCharacterDamageMitigated[3 - n]->setVisible(false);
				mStatsBannerBackground[3 - n + 1]->setVisible(false);
				mStatsPortraitBackground[3 - n + 1]->setVisible(false);
				mStatsPortraits[3 - n + 1]->setVisible(false);
				mStatsHeaderBlocks[3 - n + 1]->setVisible(false);
				mTargetCharacterBuffDebuff[3 - n]->setText(buffordebuff);

			}
			break;
		}
		/*size_t target_size = StatTracker::Instance().GetTargetList(mStatMenuCurrentLevel, mStatMenuCurrentTurn).size();
		if (target_size <= 2) 
		{
			statsMenuCanvas->getElements<Odyssey::Text2D>()[0]->setFontSize(22.0f);
		}
		else if(target_size == 3)
		{
			statsMenuCanvas->getElements<Odyssey::Text2D>()[0]->setFontSize(17.0f);
		}
		else 
		{
			statsMenuCanvas->getElements<Odyssey::Text2D>()[0]->setFontSize(12.0f);
		}
		statsMenuCanvas->getElements<Odyssey::Text2D>()[0]->setText(_turnText);*/
		statsMenuCanvas->getElements<Odyssey::Text2D>()[23]->setText(L"Level " + std::to_wstring(mStatMenuCurrentLevel));
		statsMenuCanvas->getElements<Odyssey::Text2D>()[22]->setText(L"Turn " + std::to_wstring(mStatMenuCurrentTurn));
	}
}

// This is where I will design and add all elements into the options menu canvas
void GameUIManager::CreateOptionsMenu(Odyssey::Scene* _sceneToAddTo)
{
	// Set options menu pointer and add a canvas
	mOptionsMenu = _sceneToAddTo->createEntity();
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
	height = 420;
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
	properties.fontName = L"Tw Cen MT Condensed";
	mOptionsTitle = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, 60, L"Options", properties);

	// Adjust volume text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 20.0f;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 105.0f;
	mVolumeText = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Adjust Volume", properties);
	// Volume bar
	width /= 2;
	height = 25;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 75.0f;
	// Create the volume bar background
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f), color, width + 5.0f, height + 5.0f);
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f + 75.0f), color, width + 5.0f, height + 5.0f);
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f + 150.0f), color, width + 5.0f, height + 5.0f);
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f + 225.0f), color, width + 5.0f, height + 5.0f);
	color = { 70.0f, 70.0f, 70.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), color, width, height);
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), color, width, height);
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), color, width, height);
	// Add the volume bar to the canvas
	color = { 50.0f, 180.0f, 255.0f, 1.0f };
	mVolumeBar[0] = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mVolumeBar[1] = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), color, width, height);
	mVolumeBar[2] = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), color, width, height);
	mVolumeBar[3] = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), color, width, height);
	// TODO :: FILL THE RECTANGLE BASED ON THE VOLUME LEVEL
	mVolumeBar[0]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::None)) / 1000.0f);
	mVolumeBar[3]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Dialog)) / 1000.0f);
	mVolumeBar[1]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Background)) / 1000.0f);
	mVolumeBar[2]->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::SFX)) / 1000.0f);

	// Create the plus and minus symbols that the user can click on to adjust the volume
	UINT imageWidth = 32;
	UINT imageHeight = 32;
	position.x -= static_cast<float>(imageWidth) * 1.5f;
	position.y -= 4.0f;

	// Minus Symbol
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y), color, 70, 25, L"Master ", properties);
	optionsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y + 75.0f), color, 70, 25, L"Music ", properties);
	optionsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y + 150.0f), color, 70, 25, L"SFX ", properties);
	optionsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - 80.0f, position.y + 225.0f), color, 70, 30, L"Dialog ", properties);

	mMinusImage[0] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMinusImage[1] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMinusImage[2] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMinusImage[3] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	
	mMinusImage[0]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseMasterVolume);
	mMinusImage[1]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseBackgroundVolume);
	mMinusImage[2]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseSFXVolume);
	mMinusImage[3]->registerCallback("onMouseClick", this, &GameUIManager::DecreaseDialogVolume);

	// Plus Symbol
	position.x += (static_cast<float>(imageWidth) * 1.5f) + width + (static_cast<float>(imageWidth) / 2.0f);
	mPlusImage[0] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mPlusImage[1] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 75.0f), L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mPlusImage[2] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 150.0f), L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mPlusImage[3] = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(position.x, position.y + 225.0f), L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	
	mPlusImage[0]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseMasterVolume);
	mPlusImage[1]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseBackgroundVolume);
	mPlusImage[2]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseSFXVolume);
	mPlusImage[3]->registerCallback("onMouseClick", this, &GameUIManager::IncreaseDialogVolume);

	// Create the options back button
	position = originalPosition;
	width = 100;
	height = 30;
	position.y += 400.0f - height;
	position.x += 320;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	// Add the back button text
	mBackButtonText = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Back", properties);
	// Have the back button go to the pause menu
	mBackButtonText->registerCallback("onMouseClick", this, &GameUIManager::OptionsBackButton);

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

// Create hero character portrait
void GameUIManager::CreateHeroHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition)
{
	// Create CharacterHUD object
	CharacterHUDElements* newHUD = _gameObjectToAddTo->addComponent<CharacterHUDElements>();
	// Set the canvas 
	newHUD->SetCanvas(_gameObjectToAddTo->addComponent<Odyssey::UICanvas>());
	// Get the canvas for the rest of the function
	Odyssey::UICanvas* pCanvas = newHUD->GetCanvas();

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";

	// Get the position of the anchor points
	DirectX::XMFLOAT2 position = { _hudPosition.x, _hudPosition.y };
	// Save the original position for the future when creating new items
	DirectX::XMFLOAT2 originalPosition = position;
	// Set the image width and height
	// Set the bar width and height for the Rectangle2Ds
	UINT imageWidth = 359;
	UINT imageHeight = 109;
	UINT barWidth = 252;
	UINT barHeight = 21;
	DirectX::XMFLOAT4 color = { 116.0f, 71.0f, 201.0f, 1.0f };

	// Create the base ui template
	pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/HeroUILayout4.0.png", imageWidth, imageHeight);

	// Create the character's image
	imageWidth = 71;
	imageHeight = 68;
	position.x += 2.0f;
	position.y += 1.0f;
	newHUD->SetPortrait(pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Gordon.jpg", imageWidth, imageHeight));

	// Create the xp/name bar
	//TODO::Create XP Bars In The Character Class To Assign
	position.x += imageWidth + 2.0f;
	newHUD->SetXpBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight));

	// Create the text element of the character's name
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	position.x += 7.5f;
	// Add the name to the canvas
	properties.bold = true;
	newHUD->SetCharacterName(pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, L"Setup Name", properties));
	properties.bold = false;

	// Create the character's level number text next to the XP bar
	position.x += (float)barWidth;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	newHUD->SetLevelNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"01", properties));

	// Add the text elements for Attack, Defense, and Speed;
	position = originalPosition;
	position.x += 100.0f;
	position.y += 22.0f;
	properties.fontSize = 12.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	// Attack number
	newHUD->SetAttackNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"0", properties));
	// Defense number
	position.y += 16.5f;
	newHUD->SetDefenseNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"0", properties));
	// Speed number
	position.y += 16.5f;
	newHUD->SetSpeedNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"0", properties));

	// Health and Mana bars
	// Set the position to the origanal top left position
	position = originalPosition;
	barHeight = 17;
	barWidth = 356;
	position.x += 2.0f;
	position.y += (float)imageHeight + 3.5f;
	// Create and assign the health bar
	newHUD->SetHealthBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, mHealthBarColor, barWidth, barHeight));
	newHUD->GetHealthBar()->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));

	// Create and assing shield bar
	color = { 116.0f, 71.0f, 201.0f, 1.0f };
	newHUD->SetShieldBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight));
	newHUD->GetShieldBar()->setFill(0.0f);

	// Create the text for the health numbers of the character
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.fontSize = 10.5f;
	position.x += 5.0f;
	newHUD->SetHealthNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring(0) + L"/" + std::to_wstring(0), properties));
	
	// Create and assign the mana bar
	position.x -= 5.0f;
	position.y += (float)barHeight + 1.5f;
	newHUD->SetManaBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, mManaBarColor, barWidth, barHeight));
	newHUD->GetManaBar()->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	// Create the text for the mana numbers of the character
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	position.x += 5.0f;
	newHUD->SetManaNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring(0) + L"/" + std::to_wstring(0), properties));


	// Position where the turn number will be located
	position = originalPosition;
	position.x += 5.0f;
	properties.fontSize = 14.0f;
	// Assign the character's turn order text
	newHUD->SetTurnNumber(pCanvas->addElement<Odyssey::Text2D>(position, mTurnOrderColor, 32, 32, L"1", properties));

	// Set up Skill Icons
	SetupSkillIcons(_gameObjectToAddTo, _hudPosition);

	// Set up the status effects
	SetupStatusEffects(_gameObjectToAddTo, _hudPosition, true);

	// Set the hud blocker to activate when it's not the hero's turn
	barWidth = 359;
	barHeight = 109;
	color = { 0.0f, 0.0f, 0.0f, 0.5f };
	//Odyssey::Rectangle2D* hudBlocker = pCanvas->addElement<Odyssey::Rectangle2D>(originalPosition, color, barWidth, barHeight);
	newHUD->SetHudBlocker(pCanvas->addElement<Odyssey::Rectangle2D>(originalPosition, color, barWidth, barHeight));
}

// Create enemy character portrait
void GameUIManager::CreateEnemyHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition)
{
	// Create CharacterHUD object
	CharacterHUDElements* newHUD = _gameObjectToAddTo->addComponent<CharacterHUDElements>();
	// Set the canvas 
	newHUD->SetCanvas(_gameObjectToAddTo->addComponent<Odyssey::UICanvas>());
	// Get the canvas for the rest of the function
	Odyssey::UICanvas* pCanvas = newHUD->GetCanvas();

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";

	// Get the position of the anchor points
	DirectX::XMFLOAT2 position = { _hudPosition.x, _hudPosition.y};
	// Save the original position for the future when creating new items
	DirectX::XMFLOAT2 originalPosition = position;
	// Set the image width and height
	// Set the bar width and height for the Rectangle2Ds
	UINT imageWidth = 214;
	UINT imageHeight = 42;
	UINT barWidth = 171;
	UINT barHeight = 12;
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f };

	// Add in the enemy hud template
	pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/EnemyUI/EnemyUILayout4.0.png", imageWidth, imageHeight);

	// Add in the enemy's portrait picture
	imageWidth = 39;
	imageHeight = 39;
	// Only add the portrait image if there is one
	position.x += 2.0f;
	position.y += 2.0f;
	newHUD->SetPortrait(pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Gordon.jpg", imageWidth, imageHeight));

	// Add in the enemy's name bar
	barWidth = 94;
	barHeight = 13;
	position.x += imageWidth;
	position.y += 9.0f;
	position.x += 1.0f;
	DirectX::XMFLOAT4 xpBarColor = { 116.0f, 71.0f, 201.0f, 1.0f };
	newHUD->SetXpBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, xpBarColor, barWidth, barHeight));
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	properties.fontSize = 12;
	properties.bold = true;
	position.x += 2.0f;
	position.y -= 2.0f;
	barHeight = 16;
	newHUD->SetCharacterName(pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, L"Setup Name", properties));
	position.x -= 2.0f;
	properties.bold = false;

	//// Add the text elements for Attack, Defense, and Speed;
	//position = originalPosition;
	//position.x += 100.0f;
	//position.y += 22.0f;
	//properties.fontSize = 12.0f;
	//properties.textAlignment = Odyssey::TextAlignment::Left;
	//properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	//color = { 255.0f, 255.0f, 255.0f, 1.0f };
	//// Attack number
	//newHUD->SetAttackNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(0), properties));
	//// Defense number
	//position.y += 16.5f;
	//newHUD->SetDefenseNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(0), properties));
	//// Speed number
	//position.y += 16.5f;
	//newHUD->SetSpeedNumber(pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(0), properties));

	// Add in the enemy's health bar
	barWidth = 171;
	barHeight = 15;
	position.y += 16.0f;
	barHeight += 1;
	newHUD->SetHealthBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, mHealthBarColor, barWidth, barHeight));
	newHUD->GetHealthBar()->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	// Enemy HP Bar Number
	properties.bold = false;
	newHUD->SetHealthNumber(pCanvas->addElement<Odyssey::Text2D>(position, mTextColor, 100, 43, std::to_wstring(0), properties));
	newHUD->GetHealthNumber()->setVisible(false);

	// Add big health text
	position.x += barWidth;
	properties.fontSize = 25.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;

	// Add in the enemy's mana bar
	newHUD->SetManaBar(pCanvas->addElement<Odyssey::Rectangle2D>(position, mManaBarColor, barWidth, barHeight));
	newHUD->GetManaBar()->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	newHUD->GetManaBar()->setVisible(false);
	newHUD->SetManaNumber(pCanvas->addElement<Odyssey::Text2D>(position, mTextColor, 43, 43, std::to_wstring(0), properties));
	newHUD->GetManaNumber()->setVisible(false);

	// Position where the turn number will be located
	position = originalPosition;
	position.x += 5.0f;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	// Assign the character's turn order text
	newHUD->SetTurnNumber(pCanvas->addElement<Odyssey::Text2D>(position, mTurnOrderColor, 32, 32, L"1", properties));

	// Set up the status effects
	SetupStatusEffects(_gameObjectToAddTo, _hudPosition, false);
}

void GameUIManager::CreateSkillHoverHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition)
{
	// Create CharacterHUD object
	SkillHUDElements* newSkillHUD = _gameObjectToAddTo->addComponent<SkillHUDElements>();
	// Set the canvas 
	newSkillHUD->SetCanvas(_gameObjectToAddTo->addComponent<Odyssey::UICanvas>());
	// Get the canvas for the rest of the function
	Odyssey::UICanvas* canvas = newSkillHUD->GetCanvas();

	// Set color
	DirectX::XMFLOAT4 themeColor = DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f);

	UINT windowWidth = 1280;
	UINT windowHeight = 720;
	float x = _hudPosition.x;
	float y = _hudPosition.y - 130.0f;
	UINT width = 300;
	UINT height = 115;
	UINT pad = 10;

	Odyssey::TextProperties title;
	title.bold = true;
	title.italic = false;
	title.fontSize = 24.0f;
	title.textAlignment = Odyssey::TextAlignment::Center;
	title.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	title.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";

	// Background and Separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 40), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 2);

	// Title Text and Icons
	newSkillHUD->SetSkillName(canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 40, y), themeColor, width - 80, 40, L"Skill Name", title));
	newSkillHUD->SetSkillImage(canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), L"assets/images/Guy.png", 40, 40));
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f), 40, 40);
	newSkillHUD->SetManaCost(canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(0.0f, 122.5f, 122.5f, 1.0f), 40, 40, L"0", title));

	// Description
	newSkillHUD->SetSkillDescription(canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 45), themeColor, width - (2 * pad), 85, L"This is be description", properties));
}

// Create hp popup
Odyssey::UICanvas* GameUIManager::CreateHpPopup(Odyssey::Entity* _objToAddTo)
{
	// Add health popup canvas to entity
	Odyssey::UICanvas* canvas = _objToAddTo->addComponent<Odyssey::UICanvas>();

	// Create the text properties for the health popup text
	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 30.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";

	UINT width = 100;
	UINT height = 200;

	// Make rectangle background
	//canvas->addElement<Odyssey::Rectangle2D>(_hpPopupPosition, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.25f), width, height);

	// Create the health pop text2D
	Odyssey::Text2D* newPopup = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width, height, L"100", properties);
	// Set the opacity to 0 on setup
	newPopup->setOpacity(0.0f);
	// Add new popup to the list
	//mCharacterHpPopupList.push_back(newPopup);

	return canvas;
}

// Set up the skill icons for the HUD
void GameUIManager::SetupSkillIcons(Odyssey::Entity* _hudEntity, DirectX::XMFLOAT2 _hudPosition)
{
	CharacterHUDElements* newHud = _hudEntity->getComponent<CharacterHUDElements>();
	newHud->SetHudPosition(_hudPosition);

	// Set the correct offset position for the skills
	float xAnchor = _hudPosition.x + 134.0f;
	float yAnchor = _hudPosition.y + 24.0f;

	// Skill Background sizes
	float bgPadding = 4.0f;
	UINT bgWidth = 60;
	UINT bgHeight = 54;

	// Skill1 Background
	newHud->SetSkillBackgrounds(newHud->GetCanvas()->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(xAnchor - bgPadding, yAnchor - bgPadding), DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, bgWidth, bgHeight));
	// Don't show the background at the start
	newHud->GetSkillBackgroundList()[0]->setVisible(false);
	// Skill1 Icon
	newHud->SetSkill1(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Guy.png", 52, 45));
	// Increment the icon
	xAnchor += 56.5f;

	// Skill2 Background
	newHud->SetSkillBackgrounds(newHud->GetCanvas()->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(xAnchor - bgPadding, yAnchor - bgPadding), DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, bgWidth, bgHeight));
	// Don't show the background at the start
	newHud->GetSkillBackgroundList()[1]->setVisible(false);
	// Skill2 Icon
	newHud->SetSkill2(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Guy.png", 52, 45));

	// Increment the icon
	xAnchor += 56.5f;

	// Skill3 Background
	newHud->SetSkillBackgrounds(newHud->GetCanvas()->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(xAnchor - bgPadding, yAnchor - bgPadding), DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, bgWidth, bgHeight));
	// Don't show the background at the start
	newHud->GetSkillBackgroundList()[2]->setVisible(false);
	// Skill3 Icon
	newHud->SetSkill3(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Guy.png", 52, 45));

	// Increment the icon
	xAnchor += 56.5f;

	// Skill4 Background
	newHud->SetSkillBackgrounds(newHud->GetCanvas()->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(xAnchor - bgPadding, yAnchor - bgPadding), DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, bgWidth, bgHeight));
	// Don't show the background at the start
	newHud->GetSkillBackgroundList()[3]->setVisible(false);
	// Skill4 Icon
	newHud->SetSkill4(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Guy.png", 52, 45));
}

void GameUIManager::SetupStatusEffects(Odyssey::Entity* _hudEntity, DirectX::XMFLOAT2 _hudPosition, bool _isHero)
{
	// Get the character hud elements component
	CharacterHUDElements* newHud = _hudEntity->getComponent<CharacterHUDElements>();

	// Image variables
	UINT imageWidth = 25;
	UINT imageHeight = 25;
	float imageSpacing = 5.0f;

	if (_isHero)
	{
		// Set some variables
		DirectX::XMFLOAT2 position = _hudPosition;

		// Move the image up above the bar
		position.y -= static_cast<float>(imageHeight) + imageSpacing;
		newHud->SetAttackUpBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/AttackUp.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetAttackDownBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/AttackDown.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetDefenseUpBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/DefenseUp.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetDefenseDownBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/DefenseDown.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetSpeedUpBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/SpeedUp.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetSpeedDownBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/SpeedDown.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetBleedBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Bleed.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetStunBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Stun.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetProvokeBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Provoke.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetRegenBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Regen.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetShieldBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Shield.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
	}
	else
	{
		// Set some variables
		DirectX::XMFLOAT2 position = _hudPosition;
		position.y += 42.0f; // Plus the height of the enemy hud sprite
		DirectX::XMFLOAT2 originalPosition = position;

		newHud->SetAttackUpBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/AttackUp.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetAttackDownBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/AttackDown.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetDefenseUpBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/DefenseUp.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetDefenseDownBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/DefenseDown.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetSpeedUpBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/SpeedUp.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetSpeedDownBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/SpeedDown.png", imageWidth, imageHeight));
		// Move the next set of buffs down a row
		position = originalPosition;
		position.y += (float)imageHeight;
		newHud->SetBleedBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Bleed.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetStunBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Stun.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetProvokeBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Provoke.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetRegenBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Regen.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
		newHud->SetShieldBuff(newHud->GetCanvas()->addElement<Odyssey::Sprite2D>(position, L"assets/images/StatusEffects/Shield.png", imageWidth, imageHeight));
		position.x += static_cast<float>(imageWidth) + imageSpacing;
	}

	// Don't show the buffs on the screen on start
	newHud->GetAttackUpBuff()->setVisible(false);
	newHud->GetAttackDownBuff()->setVisible(false);
	newHud->GetDefenseUpBuff()->setVisible(false);
	newHud->GetDefenseDownBuff()->setVisible(false);
	newHud->GetSpeedUpBuff()->setVisible(false);
	newHud->GetSpeedDownBuff()->setVisible(false);
	newHud->GetBleedBuff()->setVisible(false);
	newHud->GetStunBuff()->setVisible(false);
	newHud->GetProvokeBuff()->setVisible(false);
	newHud->GetRegenBuff()->setVisible(false);
	newHud->GetShieldBuff()->setVisible(false);
}

Odyssey::UICanvas* GameUIManager::SetupInfoPopup(Odyssey::Entity* _objToAddTo, Character* _character, DirectX::XMFLOAT2 _popupPosition)
{
	// Create the new canvas
	Odyssey::UICanvas* popupCanvas = _objToAddTo->addComponent<Odyssey::UICanvas>();

	DirectX::XMFLOAT3 color = _character->getEntity()->getComponent<HeroComponent>()->GetThemeColor();
	DirectX::XMFLOAT4 themeColor = { color.x, color.y, color.z, 1.0f };

	std::wstring portrait = _character->GetPortraitPath();
	std::wstring level = L"Lvl 1";
	std::wstring title = _character->GetName();
	std::wstring subtitle = _character->GetSubName();
	std::wstring description = _character->GetDescription();
	std::wstring skill1Name = _character->GetSkills()[0]->GetSkillName();
	std::wstring skill1Icon = _character->GetSkills()[0]->GetSkillIconPath();
	std::wstring skill2Name = _character->GetSkills()[1]->GetSkillName();
	std::wstring skill2Icon = _character->GetSkills()[1]->GetSkillIconPath();
	std::wstring skill3Name = _character->GetSkills()[2]->GetSkillName();
	std::wstring skill3Icon = _character->GetSkills()[2]->GetSkillIconPath();
	std::wstring skill4Name = _character->GetSkills()[3]->GetSkillName();
	std::wstring skill4Icon = _character->GetSkills()[3]->GetSkillIconPath();
	std::wstring health = L"Health: " + std::to_wstring((int)_character->GetMaxHP());
	std::wstring mana = L"Mana: " + std::to_wstring((int)_character->GetMaxMana());
	std::wstring attack = L"Attack: " + std::to_wstring((int)_character->GetAtk());
	std::wstring defense = L"Defense: " + std::to_wstring((int)_character->GetDef());
	std::wstring speed = L"Speed: " + std::to_wstring((int)_character->GetSpeed());

	float x = _popupPosition.x;
	float y = _popupPosition.y;
	UINT width = 300;
	UINT height = 400;
	UINT pad = 7;
	UINT imageWidth = 0;
	UINT imageHeight = 0;

	Odyssey::TextProperties titleText;
	titleText.bold = true;
	titleText.italic = false;
	titleText.fontSize = 20.0f;
	titleText.textAlignment = Odyssey::TextAlignment::Left;
	titleText.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	titleText.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 16.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";

	// Background and separators
	popupCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(35.0f, 35.0f, 35.0f, 1.0f), width, height);
	popupCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 45), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);
	popupCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 175), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);

	// Title
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), portrait, 45, 45);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 45 + pad, y + pad), themeColor, 150, 50, title, titleText);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 120 + pad, y + 3 + pad), DirectX::XMFLOAT4(150.0f, 150.0f, 150.0f, 1.0f), 150, 50, subtitle, properties);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 240 + pad, y + pad), themeColor, 150, 50, level, titleText);

	// Description
	properties.fontSize = 16.0f;
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 45 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 120, description, properties);

	// Skills block
	properties.fontSize = 14.0f;
	properties.italic = false;
	imageWidth = 40;
	imageHeight = 40;
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 180 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, L"Skills: ", properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 200 + pad), skill1Icon, imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 45, y + 210 + pad), themeColor, width - (2 * pad), 100, skill1Name, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 245 + pad), skill2Icon, imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 45, y + 255 + pad), themeColor, width - (2 * pad), 100, skill2Name, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 290 + pad), skill3Icon, imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 45, y + 300 + pad), themeColor, width - (2 * pad), 100, skill3Name, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 335 + pad), skill4Icon, imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 45, y + 345 + pad), themeColor, width - (2 * pad), 100, skill4Name, properties);

	// Stats block
	imageWidth = 30;
	imageHeight = 30;
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 170, y + 180 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, L"Stats: ", properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 165, y + 200 + pad), L"assets/images/Meat.png", imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 205, y + 205 + pad), DirectX::XMFLOAT4(50.0f, 255.0f, 50.0f, 1.0f), width - (2 * pad), 100, health, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 165, y + 235 + pad), L"assets/images/mp.png", imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 205, y + 240 + pad), DirectX::XMFLOAT4(50.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, mana, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 165, y + 270 + pad), L"assets/images/Sword.png", imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 205, y + 275 + pad), DirectX::XMFLOAT4(255.0f, 50.0f, 50.0f, 1.0f), width - (2 * pad), 100, attack, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 165, y + 305 + pad), L"assets/images/Shield.png", imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 205, y + 310 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), 100, defense, properties);
	popupCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad + 165, y + 340 + pad), L"assets/images/Speed.png", imageWidth, imageHeight);
	popupCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad + 205, y + 345 + pad), DirectX::XMFLOAT4(255.0f, 255.0f, 50.0f, 1.0f), width - (2 * pad), 100, speed, properties);
	popupCanvas->setActive(false);

	// Return the created canvas
	return popupCanvas;
}

void GameUIManager::AddHudToList(Odyssey::Entity* _newHud)
{
	mCharacterHudList.push_back(_newHud);
}

// Add character health bar to update list
void GameUIManager::AddCharacterHpBarsToUpdateList(Character* _currCharacter, float _previousHpAmount, float _newHpAmount)
{
	// Create new bar to update pointer
	std::shared_ptr<AnimatingBar> healthBarToUpdate = std::make_shared<AnimatingBar>();

	// Add its elements
	healthBarToUpdate->pBar = mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetHealthBar();
	healthBarToUpdate->pBarText = mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetHealthNumber();
	healthBarToUpdate->pMaxValue = _currCharacter->GetMaxHP();
	healthBarToUpdate->pCurrValue = _previousHpAmount;
	healthBarToUpdate->pNewValue = _newHpAmount;

	// Set took damage book
	healthBarToUpdate->pTookDamage = true;
	// Check if they were actually granted health
	if (_newHpAmount > _previousHpAmount)
		healthBarToUpdate->pTookDamage = false;

	// Add the health bar to the update list if there is any change
	mUpdateCharacterBarsList.push_back(healthBarToUpdate);
}

// Add character mana bar to update list
void GameUIManager::AddCharacterMpBarsToUpdateList(Character* _currCharacter, float _previousMpAmount, float _newMpAmount)
{
	// Create new bar to update pointer
	std::shared_ptr<AnimatingBar> manaBarToUpdate = std::make_shared<AnimatingBar>();

	// Add its elements
	manaBarToUpdate->pBar = mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetManaBar();
	manaBarToUpdate->pBarText = mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetManaNumber();
	manaBarToUpdate->pMaxValue = _currCharacter->GetMaxMana();
	manaBarToUpdate->pCurrValue = _previousMpAmount;
	manaBarToUpdate->pNewValue = _newMpAmount;

	// Set took damage book
	manaBarToUpdate->pTookDamage = true;
	// Check if they were actually granted mana
	if (_newMpAmount > _previousMpAmount)
	{
		manaBarToUpdate->pTookDamage = false;
	}

	// Add the mana bar to the update list if there is any change
	mUpdateCharacterBarsList.push_back(manaBarToUpdate);
}

// Add character shield bar to update list
void GameUIManager::AddCharacterShieldBarsToUpdateList(Character* _currCharacter, float _previousHpAmount, float _newHpAmount)
{
	// Create new bar to update pointer
	std::shared_ptr<AnimatingBar> barToUpdate = std::make_shared<AnimatingBar>();

	// Add its elements
	barToUpdate->pBar = mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetShieldBar();
	//barToUpdate->pBarText = mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetHealthNumber();
	barToUpdate->pMaxValue = _currCharacter->GetMaxHP();
	barToUpdate->pCurrValue = _previousHpAmount;
	barToUpdate->pNewValue = _newHpAmount;

	// Set took damage book
	barToUpdate->pTookDamage = true;
	// Check if they were actually granted health
	if (_newHpAmount > _previousHpAmount)
		barToUpdate->pTookDamage = false;

	// Add the health bar to the update list if there is any change
	mUpdateCharacterBarsList.push_back(barToUpdate);
}

// Animate the bars
void GameUIManager::UpdateCharacterBars(double _deltaTime)
{
	// Update the health and mana bars that need to be updated
	for (int i = 0; i < mUpdateCharacterBarsList.size(); i++)
	{
		// Set the speed varibale 
		float speed = 45.0f;

		// Get the max amount of bar value
		float maxValue = mUpdateCharacterBarsList[i]->pMaxValue;
		// Get the max amount of bar value
		float currValue = mUpdateCharacterBarsList[i]->pCurrValue;
		// Get the target amount of the bar
		float targetValue = mUpdateCharacterBarsList[i]->pNewValue;

		// Make sure I'm clamping the target value to lerp to
		if (targetValue > maxValue)
			targetValue = maxValue;
		else if (targetValue < 0.0f)
			targetValue = 0.0f;

		// Check if the bar is done animating
		if (mUpdateCharacterBarsList[i]->pTookDamage)
		{
			// See if the current value is less or equal to the target value
			if (currValue <= targetValue)
			{
				// Set the bar to the target fill
				mUpdateCharacterBarsList[i]->pBar->setFill(targetValue / maxValue);
				// Set the text of the bar
				if(mUpdateCharacterBarsList[i]->pBarText != nullptr)
					mUpdateCharacterBarsList[i]->pBarText->setText(std::to_wstring((int)targetValue) + L"/" + std::to_wstring((int)maxValue));
				// Remove the bar from being updated
				mUpdateCharacterBarsList.erase(mUpdateCharacterBarsList.begin() + i);
				continue;
			}
		}
		else
		{
			// See if the current value is greater or equal to the target value
			if (currValue >= targetValue)
			{
				// Set the bar to the target fill
				mUpdateCharacterBarsList[i]->pBar->setFill(targetValue / maxValue);
				// Set the text of the bar
				if(mUpdateCharacterBarsList[i]->pBarText != nullptr)
					mUpdateCharacterBarsList[i]->pBarText->setText(std::to_wstring((int)targetValue) + L"/" + std::to_wstring((int)maxValue));
				// Remove the bar from being updated
				mUpdateCharacterBarsList.erase(mUpdateCharacterBarsList.begin() + i);
				continue;
			}
		}
		
		// Check if we animate bar down or up
		if (mUpdateCharacterBarsList[i]->pTookDamage)
		{
			// Update the bar's currValue
			mUpdateCharacterBarsList[i]->pCurrValue -= (speed * (float)_deltaTime);
		}
		// Else we go up
		else
		{
			// Update the bar's currValue
			mUpdateCharacterBarsList[i]->pCurrValue += (speed * (float)_deltaTime);
		}

		// Clamp the pCurrBalue to the maxValue or 0
		if (mUpdateCharacterBarsList[i]->pCurrValue > maxValue)
			mUpdateCharacterBarsList[i]->pCurrValue = maxValue;
		else if (mUpdateCharacterBarsList[i]->pCurrValue < 0.0f)
			mUpdateCharacterBarsList[i]->pCurrValue = 0.0f;

		// Get the new ratio and set the fill and set the text
		float newRatio = mUpdateCharacterBarsList[i]->pCurrValue / maxValue;
		mUpdateCharacterBarsList[i]->pBar->setFill(newRatio);
		if(mUpdateCharacterBarsList[i]->pBarText != nullptr)
			mUpdateCharacterBarsList[i]->pBarText->setText(std::to_wstring((int)mUpdateCharacterBarsList[i]->pCurrValue) + L"/" + std::to_wstring((int)maxValue));
	}

	//// Get the ratio from current health and max health
	//float healthRatio = _currCharacter->GetHP() / _currCharacter->GetMaxHP();
	//// Get the ratio from current mana and max mana
	//float manaRatio = _currCharacter->GetMana() / _currCharacter->GetMaxMana();
	//
	//// Set the fill of the character's health bar
	//mCharacterHudList[_currCharacter->GetHudIndex()]->pHealthBar->setFill(healthRatio);
	//// Set the fill of the character's mana bar
	//mCharacterHudList[_currCharacter->GetHudIndex()]->pManaBar->setFill(manaRatio);
	//
	//// Set the text for the health bar
	//mCharacterHudList[_currCharacter->GetHudIndex()]->pHealthNumber->setText(std::to_wstring((int)_currCharacter->GetHP()) + L"/" + std::to_wstring((int)_currCharacter->GetMaxHP()));
	//// Set the text for the mana bar
	//mCharacterHudList[_currCharacter->GetHudIndex()]->pManaNumber->setText(std::to_wstring((int)_currCharacter->GetMana()) + L"/" + std::to_wstring((int)_currCharacter->GetMaxMana()));
}

// When the health updates for the character, show the health popups
void GameUIManager::AddHpPopupToUpdateList(Character* _currCharacter, bool _tookDamage, float _changeInHP)
{
	// Get the text element that corresponds with the correct character
	Odyssey::Text2D* textToUpdate = mCharacterHpPopupList[_currCharacter->GetHudIndex()];

	// Set the text to the amount of change in health points
	textToUpdate->setText(std::to_wstring((int)_changeInHP));

	// If the character took damage set the text color to red, else set to green for gaining health
	if (_tookDamage)
		textToUpdate->setColor(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	else
		textToUpdate->setColor(DirectX::XMFLOAT3(0.0f, 255.0f, 0.0f));

	// Set the opacticy to 100%
	textToUpdate->setOpacity(1.0f);

	// Add the Text2D Health Popup to the update list
	mUpdateHpPopupList.push_back(textToUpdate);
}

// This will be called in the tower manager's update
void GameUIManager::UpdateCharacterHealthPopups(double _deltaTime)
{
	// Update the health pop ups that need to be updated
	for (int i = 0; i < mUpdateHpPopupList.size(); i++)
	{
		// Get the current opacity if the text element
		float currentOpacity = mUpdateHpPopupList[i]->getOpacity();
		// Set the speed varibale 
		float speed = 0.25f;
		// Set the new opacity
		mUpdateHpPopupList[i]->setOpacity(currentOpacity - (speed * (float)_deltaTime));

		// Check to see if the opacity is back 0.0f, if so remove it from the list
		if (mUpdateHpPopupList[i]->getOpacity() <= 0.0f)
			mUpdateHpPopupList.erase(mUpdateHpPopupList.begin() + i);
	}
}

// This will be called in the tower manager's update 
void GameUIManager::UpdateCharacterTurnNumber(Character* _currCharacter, int _turnNumber)
{
	// If the turn number is 666, that means he is dead and the text needs to be set to X
	if (_turnNumber == 666)
	{
		mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetTurnNumber()->setText(L"X");
		// Deplete the health bar
		mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetHealthBar()->setFill(0.0f);
		// Set the HP to zero when they die
		_currCharacter->SetHP(0.0f);
	}
	else
		mCharacterHudList[_currCharacter->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetTurnNumber()->setText(std::to_wstring(_turnNumber));
}

void GameUIManager::DecreaseMasterVolume()
{
	// Decrease volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::None) - 50);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::None)) / 1000.0f;
	mVolumeBar[0]->setFill(volumeRatio);
	mMainVolumeBar[0]->setFill(volumeRatio);
}

void GameUIManager::DecreaseBackgroundVolume()
{
	// Decrease volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Background) - 50, RedAudioManager::AudioType::Background);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Background)) / 1000.0f;
	mVolumeBar[1]->setFill(volumeRatio);
	mMainVolumeBar[1]->setFill(volumeRatio);
}

void GameUIManager::DecreaseSFXVolume()
{
	// Decrease volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::SFX) - 50, RedAudioManager::AudioType::SFX);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::SFX)) / 1000.0f;
	mVolumeBar[2]->setFill(volumeRatio);
	mMainVolumeBar[2]->setFill(volumeRatio);
}

void GameUIManager::DecreaseDialogVolume()
{
	// Decrease volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Dialog) - 50, RedAudioManager::AudioType::Dialog);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Dialog)) / 1000.0f;
	mVolumeBar[3]->setFill(volumeRatio);
	mMainVolumeBar[3]->setFill(volumeRatio);
}

void GameUIManager::IncreaseMasterVolume()
{
	// Increase volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::None) + 50);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::None)) / 1000.0f;
	mVolumeBar[0]->setFill(volumeRatio);
	mMainVolumeBar[0]->setFill(volumeRatio);
}

void GameUIManager::IncreaseBackgroundVolume()
{
	// Increase volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Background) + 50, RedAudioManager::AudioType::Background);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Background)) / 1000.0f;
	mVolumeBar[1]->setFill(volumeRatio);
	mMainVolumeBar[1]->setFill(volumeRatio);
}

void GameUIManager::IncreaseSFXVolume()
{
	// Increase volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::SFX) + 50, RedAudioManager::AudioType::SFX);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::SFX)) / 1000.0f;
	mVolumeBar[2]->setFill(volumeRatio);
	mMainVolumeBar[2]->setFill(volumeRatio);
}

void GameUIManager::IncreaseDialogVolume()
{
	// Increase volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Dialog) + 50, RedAudioManager::AudioType::Dialog);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume(RedAudioManager::AudioType::Dialog)) / 1000.0f;
	mVolumeBar[3]->setFill(volumeRatio);
	mMainVolumeBar[3]->setFill(volumeRatio);
}

Odyssey::UICanvas* GameUIManager::CreatePopup(Odyssey::Entity* entity)
{
	UINT windowWidth = 1280;
	UINT windowHeight = 720;
	float x = 910;
	float y = 350;
	UINT width = 360;
	UINT height = 200;
	UINT pad = 10;

	Odyssey::TextProperties title;
	title.bold = true;
	title.italic = false;
	title.fontSize = 30.0f;
	title.textAlignment = Odyssey::TextAlignment::Center;
	title.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	title.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 16.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";

	Odyssey::UICanvas* canvas = entity->addComponent<Odyssey::UICanvas>();

	// Background and Separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 50), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 10);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 100), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 10);

	// Title Text and Icons
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 50, y), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), width - 100, 50, L"Basic Attack", title);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), L"assets/images/Paladin_Skill_1.png", 50, 50);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x + width - 50, y), DirectX::XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f), 50, 50);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + width - 50, y), DirectX::XMFLOAT4(0.0f, 122.5f, 122.5f, 1.0f), 50, 50, L"25", title);

	// Skill Info
	properties.italic = true;
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 60 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 25 + pad, y + 70), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 150, 50, L"Attack", properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 100 + pad, y + 60 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 125 + pad, y + 70), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 150, 50, L"Targets: 1", properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 225 + pad, y + 60 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 250 + pad, y + 70), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 150, 50, L"Damage: 15", properties);

	// Description
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 115), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), height - 110 - pad, L"Description: Strikes a single enemy with 100 % chance to apply provoke.", properties);
	canvas->setActive(false);

	return canvas;
}

void GameUIManager::UpdateCombatLogIcons(Character* caster, Character* target, Skills* skill)
{
	UINT spriteSize = 25;

	for (int i = (int)mCombatCasterIcons.size() - 1; i > 0; i--)
	{
		if (i - 1 >= 0)
		{
			mCombatCasterIcons[i]->setSprite(mCombatCasterIcons[i -1]->getSpriteFilename(), spriteSize, spriteSize);
			mCombatSkillIcons[i]->setSprite(mCombatSkillIcons[i - 1]->getSpriteFilename(), spriteSize, spriteSize);
			mCombatTargetIcons[i]->setSprite(mCombatTargetIcons[i - 1]->getSpriteFilename(), spriteSize, spriteSize);
		}
	}

	if (caster->GetPortraitPath().size() > 0)
		mCombatCasterIcons[0]->setSprite(caster->GetPortraitPath(), spriteSize, spriteSize);
	else
		mCombatCasterIcons[0]->setSprite(L"assets/images/Blank.png", spriteSize, spriteSize);
	
	if (skill->GetSkillIconPath().size() > 0)
		mCombatSkillIcons[0]->setSprite(skill->GetSkillIconPath(), spriteSize, spriteSize);
	else
		mCombatSkillIcons[0]->setSprite(L"assets/images/Blank.png", spriteSize, spriteSize);
	
	if (target->GetPortraitPath().size() > 0)
		mCombatTargetIcons[0]->setSprite(target->GetPortraitPath(), spriteSize, spriteSize);
	else
		mCombatTargetIcons[0]->setSprite(L"assets/images/Blank.png", spriteSize, spriteSize);

}

void GameUIManager::ClearCombatLog()
{
	UINT spriteSize = 20;

	for (int i = 0; i < mCombatCasterIcons.size(); i++)
		mCombatCasterIcons[i]->setSprite(L"assets/images/Blank.png", spriteSize, spriteSize);

	for (int i = 0; i < mCombatSkillIcons.size(); i++)
		mCombatSkillIcons[i]->setSprite(L"assets/images/Blank.png", spriteSize, spriteSize);

	for (int i = 0; i < mCombatTargetIcons.size(); i++)
		mCombatTargetIcons[i]->setSprite(L"assets/images/Blank.png", spriteSize, spriteSize);

	for (int i = (int)mBattleLogVec.size() - 1; i >= 0; i--)
	{
		mBattleLogVec[i]->setText(L"");
	}
}

void GameUIManager::UpdateCombatLogText(float damage)
{
	std::wstring newText = Converter::FormatToPercentageW(damage, 2);

	for (int i = (int)mBattleLogVec.size() - 1; i > 0; i--)
	{
		if (i - 1 >= 0)
		{
			mBattleLogVec[i]->setText(mBattleLogVec[i - 1]->getText());
			DirectX::XMFLOAT4 tempColor = mBattleLogVec[i - 1]->getColor();
			DirectX::XMFLOAT3 tempColor2 = { tempColor.x, tempColor.y, tempColor.z };
			mBattleLogVec[i]->setColor(tempColor2);
		}
	}

	mBattleLogVec[0]->setColor(newCombatLogColor);
	mBattleLogVec[0]->setText(newText);
	
}

///////////////////////////////
//                           //
// Prefab Creation Functions //
//                           //
///////////////////////////////
Odyssey::Entity* GameUIManager::CreatePauseMenuPrefab()
{
	// Create the pause menu prefab
	mPauseMenu = mApplication->createPrefab();
	// Add a canvas to the object
	Odyssey::UICanvas* pauseMenuCanvas = mPauseMenu->addComponent<Odyssey::UICanvas>();

	// Create the rectangle object
	UINT width = 1920;
	UINT height = 1080;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
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
	properties.fontName = L"Tw Cen MT Condensed";
	mPauseTitle = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, 640, 60, L"Paused", properties);

	// Resume Button
	width /= (UINT)2.5f;
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

	return mPauseMenu;
}

// Clickable functions
Odyssey::Entity* GameUIManager::CreateClickableUIPrefab(DirectX::XMFLOAT2 _clickableRectPos, bool _isHero)
{
	// Create the clickable object prefab
	Odyssey::Entity* mClickableUI = mApplication->createPrefab();
	// Add a canvas to the object
	Odyssey::UICanvas* canvas = mClickableUI->addComponent<Odyssey::UICanvas>();

	// Set screen size
	UINT width = 0;
	UINT height = 0;

	if (_isHero)
	{
		width = 100;
		height = 200;
	}
	else
	{
		width = 50;
		height = 100;
	}

	// Set position
	DirectX::XMFLOAT2 position = _clickableRectPos;
	// Set color
	DirectX::XMFLOAT4 color = { 0.0f, 255.0f, 0.0f, 1.0f };
	// Add the rectangle to the pause menu canvas
	Odyssey::Rectangle2D* rect = canvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Make the rectangle have 50% transparency
	rect->setOpacity(0.0f);

	return mClickableUI;
}

void GameUIManager::Character1ClickableCallback()
{
	Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Hero1));
}

void GameUIManager::Character2ClickableCallback()
{
	Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Hero2));
}

void GameUIManager::Character3ClickableCallback()
{
	Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Hero3));
}

void GameUIManager::Character4ClickableCallback()
{
	Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Enemy1));
}

void GameUIManager::Character5ClickableCallback()
{
	if (TeamManager::getInstance().GetEnemyTeam().size() == 1)
		Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Enemy1));
	else
		Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Enemy2));
}

void GameUIManager::Character6ClickableCallback()
{
	Odyssey::EventManager::getInstance().publish(new SetNewTargetEvent(SetNewTargetEvent::Player::Enemy3));
}

void GameUIManager::CharacterEnterHoverCallback()
{
	// Set the cursor to the sword
	Odyssey::EventManager::getInstance().publish(new Odyssey::ChangeMouseCursorEvent(L"assets/images/Cursor/Cursor_Attack.cur"));
}

void GameUIManager::CharacterExitHoverCallback()
{
	// Set the cursor to the basic pointer
	Odyssey::EventManager::getInstance().publish(new Odyssey::ChangeMouseCursorEvent(L"assets/images/Cursor/Cursor_Basic.cur"));
}

void GameUIManager::TutorialCallBack()
{
	UINT imageWidth = 1280;
	UINT imageHeight = 720;

	switch (tutorialCounter)
	{
		// Display Player UI
		case 0:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightPlayerUI.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainPlayerUI.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Player Abilities
		case 1:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightPlayerAbilities.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainPlayerAbilities.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Stats
		case 2:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightPlayerStats.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainPlayerStats.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Buffs
		case 3:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightPlayerBuffs.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainPlayerBuffs.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Turn 
		case 4:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightTurnNumber.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainTurnOrder.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Level
		case 5:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightLevel.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainLevel.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Enemy
		case 6:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightEnemy.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainEnemy.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Enemy UI
		case 7:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightEnemyUI.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainEnemyUI.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Display Enemy Buffs
		case 8:
		{
			mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightEnemyBuffs.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainEnemyBuffs.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Turn Off Canvas
		case 9:
		{
			mHighlightTutorial->setSprite(L"assets/images/blackSquare.png", imageWidth, imageHeight);
			mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainEnd.png", imageWidth, imageHeight);
			tutorialCounter += 1;
			break;
		}
		// Turn Off Canvas
		case 10:
		{
			mTutorialCanvas->setActive(false);
			break;
		}
		default:
		{
			break;
		}
	}
}

void GameUIManager::EndTutorial()
{
	UINT imageWidth = 1280;
	UINT imageHeight = 720;
	mHighlightTutorial->setSprite(L"assets/images/TutorialLevel/HighlightPlayer.png", imageWidth, imageHeight);
	mHighlightTutorialText->setSprite(L"assets/images/TutorialLevel/ExplainPlayer.png", imageWidth, imageHeight);

	tutorialCounter = 0;
	mTutorialCanvas->setActive(false);
}

