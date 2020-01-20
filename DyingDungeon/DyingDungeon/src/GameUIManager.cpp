#include "GameUIManager.h"
#include "UICanvas.h"

GameUIManager& GameUIManager::getInstance()
{
	static GameUIManager instance;
	return instance;
}

void GameUIManager::ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive)
{
	// Set the passed in entity's canvas to active or deactived based on the bool _isActive.
	_canvas->setActive(_isActive);
}

// This is where I will design and add all the lements into the tower select screen
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
	width = 782.0f / 3.0f;
	height = 927.0f / 3.0f;
	position = { 100.0f, screenHeight - (height - 50.0f) };
	mDoorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/MedievalDoor.png", width, height);

	// Add the pause menu to the game scene most likely
	_sceneToAddTo->addEntity(mTowerSelectMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mTowerSelectMenu->getComponent<Odyssey::UICanvas>(), false);
}

// This is where I will design and add all elemnts into the pause menu canvas
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
	mBlackBackground->setOpacity(0.5f);

	// Create the pause menu smaller black rectangle
	width = 640;
	height = 360;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	// Add the rectangle to the pause menu canvas
	mSmallerBlackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mSmallerBlackBackground->setOpacity(0.8f);

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
}

// Create the character's UI Portrait
void GameUIManager::CreateCharacterPortrait(float anchorX, float anchorY, LPCWSTR _imageName, Odyssey::UICanvas* canvas, Character* owner)
{
	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";

	// Get the position of the anchor points
	DirectX::XMFLOAT2 position = { anchorX, anchorY };
	// Save the original position for the future when creating new items
	DirectX::XMFLOAT2 originalPosition = position;
	// Set the image width and height
	// Set the bar width and height for the Rectangle2Ds
	UINT imageWidth = 397;
	UINT imageHeight = 150;
	UINT barWidth = 233;
	UINT barHeight = 23;
	DirectX::XMFLOAT4 color = { 116.0f, 71.0f, 201.0f, 1.0f };

	// Create the base ui template
	canvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/HeroUILayout.png", imageWidth, imageHeight);

	// Create the character's image
	imageWidth = 133;
	imageHeight = 122;
	canvas->addElement<Odyssey::Sprite2D>(position, _imageName, imageWidth, imageHeight);

	// Create the xp/name bar
	//TODO::Create XP Bars In The Character Class To Assign
	position.x += imageWidth;
	if (owner)
		canvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);
	else										 
		canvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);
	
	// Create the text element of the character's name
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	position.x += 7.5f;
	// Convert name to wstring
	size_t cSize = strlen(owner->GetName().c_str()) + 1;
	size_t convertedChars = 0;
	wchar_t* characterName = new wchar_t[cSize];
	mbstowcs_s(&convertedChars, characterName, cSize, owner->GetName().c_str(), _TRUNCATE);
	// Add the name to the canvas
	canvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, characterName, properties);
	// Delete the newed pointer
	delete characterName;
	characterName = nullptr;

	// Create the character's level number text next to the XP bar
	position.x += barWidth;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	canvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"99", properties);

	// TODO: Add the text elements for Attack, Defense, and Speed;
	position = originalPosition;
	position.x += imageWidth + 30.0f;
	position.y += 23.5f;
	// Attack number
	canvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring((int)owner->GetAtk()), properties);
	// Defense number
	position.y += 35.0f;
	canvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring((int)owner->GetDef()), properties);
	// Speed number
	position.y += 35.0f;
	canvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring((int)owner->GetSpeed()), properties);

	// Health and Mana bars
	// Set the position to the origanal top left position
	position = originalPosition;
	barHeight = 14;
	barWidth = 397;
	position.y += imageHeight;
	if (owner)
	{
		// Create and assign the health bar
		color = { 0.0f, 255.0f, 0.0f, 1.0f };
		owner->pHealthBar = canvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);
		owner->pHealthBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		// Create the text for the health numbers of the character
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		properties.fontSize = 10.5f;
		position.x += 5.0f;
		canvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring((int)owner->GetHP()) + L"/" + std::to_wstring((int)owner->GetMaxHP()), properties);
		// Create and assign the mana bar
		position.x -= 5.0f;
		position.y += barHeight;
		color = { 0.0f, 255.0f, 255.0f, 1.0f };
		owner->pManaBar = canvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);
		owner->pManaBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		// Create the text for the mana numbers of the character
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		position.x += 5.0f;
		canvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring((int)owner->GetMana()) + L"/" + std::to_wstring((int)owner->GetMaxMana()), properties);
	}
	else
	{
		Odyssey::Rectangle2D* rect = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + 50), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 50, 10);
		rect->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		rect = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(anchorX, anchorY + 62), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 50, 10);
		rect->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	}

	// Position where the turn number will be located
	position = originalPosition;
	position.x += 5.0f;
	properties.fontSize = 14.0f;
	// Assign the character's turn order text
	if (owner)
		owner->pTurnNumber = canvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 210.0f, 0.0f, 1.0f), 32, 32, L"1", properties);
	else
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(anchorX - static_cast<int>(properties.fontSize), anchorY), DirectX::XMFLOAT4(255.0f, 210.0f, 0.0f, 1.0f), 32, 32, L"1", properties);
}