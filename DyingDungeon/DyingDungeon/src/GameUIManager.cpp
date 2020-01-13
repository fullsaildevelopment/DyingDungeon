#include "GameUIManager.h"
#include "UICanvas.h"

GameUIManager& GameUIManager::getInstance()
{
	static GameUIManager instance;
	return instance;
}

void GameUIManager::ToggleCanvas(std::shared_ptr<Odyssey::Entity> _object, bool _isActive)
{
	// Set the passed in entity's canvas to active or deactived based on the bool _isActive.
	_object->getComponent<Odyssey::UICanvas>()->setActive(_isActive);
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
	blackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Make the rectangle have 50% transparency
	blackBackground->setOpacity(0.5f);

	// Create the pause menu smaller black rectangle
	width = 640;
	height = 360;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	// Add the rectangle to the pause menu canvas
	smallerBlackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	smallerBlackBackground->setOpacity(0.8f);

	// Create Pause Title
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 50.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";
	pauseTitle = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, 640, 50, L"Paused", properties);
	pauseTitle->registerCallback("onMouseClick", this, &GameUIManager::ToggleTitle);


	// Add the pause menu to the game scene most likely
	_sceneToAddTo->addEntity(mPauseMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mPauseMenu, false);
}

void GameUIManager::ToggleTitle()
{
	static bool yeehaw = true;

	if (yeehaw)
	{
		pauseTitle->setColor(0.0f, 255.0f, 0.0f);
	}
	else
		pauseTitle->setColor(255.0f, 255.0f, 255.0f);

		yeehaw = !yeehaw;
}
