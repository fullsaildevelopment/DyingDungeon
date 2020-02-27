#include "OdysseyEngine.h"
#include "LoadingScreenController.h"

CLASS_DEFINITION(Odyssey::Component, LoadingScreenController)
LoadingScreenController::LoadingScreenController()
{
	Odyssey::EventManager::getInstance().subscribe(this, &LoadingScreenController::onSpawnLoadingScreen);
	constructLoadingScreen();
	mCurrentTime = 0.0f;
	mWaitTime = 2.5f;
}

void LoadingScreenController::initialize()
{
	// Show the screen
	Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mLoadingScreenPrefab, &mLoadingScreen, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }));
	Odyssey::TextProperties paragraphProperties;
	paragraphProperties.fontName = L"Tw Cen MT Condensed";
	paragraphProperties.fontSize = 18;
	paragraphProperties.textAlignment = Odyssey::TextAlignment::Left;
	paragraphProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	paragraphProperties.bold = false;
	mLogo = mLoadingScreen->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Sprite2D>()[11];
	mContinue = mLoadingScreen->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Text2D>()[11];
	mContinue->setOpacity(0.0f);
	mCurrentTime = 0.0f;
}

std::shared_ptr<Odyssey::Component> LoadingScreenController::clone() const
{
	return std::make_shared<LoadingScreenController>(*this);
}

void LoadingScreenController::update(double deltaTime)
{
	static float opacity = 1.0f;
	static int direction = -1;

	// Update the current time
	mCurrentTime += deltaTime;
	mCurrentTime = min(mWaitTime, mCurrentTime);

	// Set the mask's opacity
	mLogo->setFill(mCurrentTime / mWaitTime);

	// Check if the user is able to move to the next scene
	if (mCurrentTime == mWaitTime)
	{
		static float opacity = 0.0f;
		static int direction = 1;
		opacity += deltaTime * direction;
		opacity = max(0.25f, min(opacity, 1.0f));
		if (opacity == 0.25f && direction == -1)
		{
			direction = 1;
		}
		else if (opacity == 1.0f && direction == 1)
		{
			direction = -1;
		}
		mContinue->setOpacity(opacity);
		// Allow interaction
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
		{
			Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent(mNextScene));
		}
	}
}

void LoadingScreenController::onDestroy()
{
	Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mLoadingScreen));
}

void LoadingScreenController::onSpawnLoadingScreen(SpawnLoadingScreenEvent* evnt)
{
	mNextScene = evnt->nextScene;
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Loading Screen"));
}

void LoadingScreenController::constructLoadingScreen()
{
	Odyssey::EventManager::getInstance().publish(new Odyssey::CreatePrefabEvent(&mLoadingScreenPrefab));
	Odyssey::UICanvas* canvas = mLoadingScreenPrefab->addComponent<Odyssey::UICanvas>();

	// Loading Screen Code
	Odyssey::TextProperties titleProperties;
	titleProperties.fontName = L"Tw Cen MT Condensed";
	titleProperties.fontSize = 32;
	titleProperties.textAlignment = Odyssey::TextAlignment::Left;
	titleProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	titleProperties.bold = true;

	Odyssey::TextProperties paragraphProperties;
	paragraphProperties.fontName = L"Tw Cen MT Condensed";
	paragraphProperties.fontSize = 18;
	paragraphProperties.textAlignment = Odyssey::TextAlignment::Left;
	paragraphProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	paragraphProperties.bold = false;

	// Set the text to use
	std::wstring titleText = L"Renaud - The Zealous Knight";
	std::wstring backgroundFile = L"assets/images/FantasyBackground1.jpg";
	std::wstring spriteFile = L"assets/images/Renaud.png";
	std::wstring paragraphText = L"Renaud has always been a devout follower of the church. He grew up in the church, training his entire life to become a paladin. Renaud has found absolution within the church’s teachings and knows nothing beyond them. Ever since the day Ganfaul killed The Council and took control of Metis, Renaud has sought to overthrow him and return the church to its former glory.";
	std::wstring odysseyWhite = L"assets/images/Odyssey_White_Mask.png";
	std::wstring odysseyBlack = L"assets/images/Odyssey_Black_Mask.png";
	std::wstring odysseyBronze = L"assets/images/Odyssey_Bronze_Logo.png";
	// Stat Icons
	std::wstring healthIcon = L"assets/images/Meat.png";
	std::wstring healthText = L"Health: 100";
	std::wstring manaIcon = L"assets/images/mp.png";
	std::wstring manaText = L"Mana: 100";
	std::wstring attackIcon = L"assets/images/Sword.png";
	std::wstring attackText = L"Attack: 65";
	std::wstring defenseIcon = L"assets/images/Shield.png";
	std::wstring defenseText = L"Defense: 80";
	std::wstring speedIcon = L"assets/images/Speed.png";
	std::wstring speedText = L"Speed: 20";

	std::wstring skill1Text = L"Judgement";
	std::wstring skill1Icon = L"assets/images/Paladin_Skill_1.png";
	std::wstring skill2Text = L"Smite";
	std::wstring skill2Icon = L"assets/images/Paladin_Skill_2.png";
	std::wstring skill3Text = L"Shield of Light";
	std::wstring skill3Icon = L"assets/images/Paladin_Skill_3.png";
	std::wstring skill4Text = L"Blessing of Light";
	std::wstring skill4Icon = L"assets/images/Paladin_Skill_4.png";

	// Add the background image and the title/image
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), backgroundFile, 1280, 720);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(90.0f, 70.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.6f), 650, 400);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 80.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 500, 100, titleText, titleProperties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(850.0f, 105.0f), spriteFile, 350, 500);

	// Add the summary and background
	paragraphProperties.italic = true;
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(115.0f, 130.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 610, 500, paragraphText, paragraphProperties);

	// Stats line - Health
	paragraphProperties.italic = false;
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(220.0f, 270.0f), healthIcon, 30, 30);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(265.0f, 275.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), 100, 100, healthText, paragraphProperties);

	// Stats line - Mana
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(380.0f, 270.0f), manaIcon, 30, 30);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(420.0f, 275.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 255.0f, 1.0f), 100, 100, manaText, paragraphProperties);

	// Stats line - Attack
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(170.0f, 310.0f), attackIcon, 30, 30);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(205.0f, 310.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), 100, 100, attackText, paragraphProperties);

	// Stats line - Defense
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(300.0f, 310.0f), defenseIcon, 25, 30);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(335.0f, 310.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, 100, defenseText, paragraphProperties);

	// Stats line - Speed
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(440.0f, 310.0f), speedIcon, 30, 30);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(475.0f, 310.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 0.0f, 1.0f), 100, 100, speedText, paragraphProperties);

	// Skill 1
	paragraphProperties.fontSize = 24;
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(110.0f, 360.0f), skill1Icon, 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(170.0f, 362.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill1Text, paragraphProperties);

	// Skill 2
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(410.0f, 360.0f), skill2Icon, 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(470.0f, 362.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill2Text, paragraphProperties);

	// Skill 3
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(110.0f, 410.0f), skill3Icon, 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(170.0f, 412.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill3Text, paragraphProperties);

	// Skill 4
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(410.0f, 410.0f), skill4Icon, 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(470.0f, 412.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill4Text, paragraphProperties);

	// Odyssey logo
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 470.0f), odysseyBronze, 250, 250);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(500.0f, 600.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 610, 500, L"Press Enter to Continue...", titleProperties);
}