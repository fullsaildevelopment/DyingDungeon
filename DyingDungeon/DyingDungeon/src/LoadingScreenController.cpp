#include "OdysseyEngine.h"
#include "LoadingScreenController.h"

CLASS_DEFINITION(Odyssey::Component, LoadingScreenController)
LoadingScreenController::LoadingScreenController()
{
	Odyssey::EventManager::getInstance().subscribe(this, &LoadingScreenController::onSpawnLoadingScreen);

	mLoadingScreenPrefab = nullptr;
	mCurrentTime = 0.0f;
	mWaitTime = 2.5f;

	// Set default values for the loading screen
	// Paladin is sub 0
	titleList[0] = L"Renaud - The Zealous Knight";
	backgroundList[0] = L"assets/images/FantasyBackground0.jpg";
	spriteList[0] = L"assets/images/Renaud.png";
	biographyList[0] = L"Renaud has always been a devout follower of the church. He grew up in the church, training his entire life to become a paladin. Renaud has found absolution within the church’s teachings and knows nothing beyond them. Ever since the day Ganfaul killed The Council and took control of Metis, Renaud has sought to overthrow him and return the church to its former glory.";
	skill1TextList[0] = L"Judgement";
	skill2TextList[0] = L"Smite";
	skill3TextList[0] = L"Shield of Light";
	skill4TextList[0] = L"Blessing of Light";
	skill1IconList[0] = L"assets/images/PaladinSkills/Paladin_Skill_1.png";
	skill2IconList[0] = L"assets/images/PaladinSkills/Paladin_Skill_2.png";
	skill3IconList[0] = L"assets/images/PaladinSkills/Paladin_Skill_3.png";
	skill4IconList[0] = L"assets/images/PaladinSkills/Paladin_Skill_4.png";

	// Mage is sub 1
	titleList[1] = L"Velia - The Vengeful Witch";
	backgroundList[1] = L"assets/images/FantasyBackground1.jpg";
	spriteList[1] = L"assets/images/Velia.png";
	biographyList[1] = L"Velia lived a peaceful life in the forests outside of Metis. It wasn’t until Ganfaul’s armies marched across the Forest of Gelmeur that her peace was forever shattered. In their conquest, thousands of innocent villagers were slaughtered. Among these were Velia’s own parents. Little does Ganfaul know that he made a fatal error and a terrible enemy on that day. Velia dedicated herself to her magical studies so she could one day exact revenge and kill Ganfaul.";
	skill1TextList[1] = L"Meteor Shard";
	skill2TextList[1] = L"Wind Slash";
	skill3TextList[1] = L"Fire Storm";
	skill4TextList[1] = L"Lightning Bolt";
	skill1IconList[1] = L"assets/images/MageSkills/Mage_Skill_1.png";
	skill2IconList[1] = L"assets/images/MageSkills/Mage_Skill_2.png";
	skill3IconList[1] = L"assets/images/MageSkills/Mage_Skill_3.png";
	skill4IconList[1] = L"assets/images/MageSkills/Mage_Skill_4.png";

	// Bard is sub 2
	titleList[2] = L"Gale - The Beautiful Rose";
	backgroundList[2] = L"assets/images/FantasyBackground2.jpg";
	spriteList[2] = L"assets/images/Gale.png";
	biographyList[2] = L"Gale was orphaned at a young age. She was adopted by a minstrel that worked in one of the taverns who taught Gale the art of song and dance. Her natural talent and beauty combined with her charisma lent her a profitable position at the tavern, servicing men and women alike. But she longed for more. She wanted songs recounting her amazing escapades, immortalizing her in the words of the stories she lived. She wanted to become a hero.";
	skill1TextList[2] = L"Starfire Arrow";
	skill2TextList[2] = L"Song of Misery";
	skill3TextList[2] = L"Song of Hope";
	skill4TextList[2] = L"Purify";
	skill1IconList[2] = L"assets/images/BardSkills/Bard_Skill_1.png";
	skill2IconList[2] = L"assets/images/BardSkills/Bard_Skill_2.png";
	skill3IconList[2] = L"assets/images/BardSkills/Bard_Skill_3.png";
	skill4IconList[2] = L"assets/images/BardSkills/Bard_Skill_4.png";

	// Warrior
	titleList[3] = L"Cedric - The Drunkard";
	backgroundList[3] = L"assets/images/FantasyBackground3.jpg";
	spriteList[3] = L"assets/images/Cedric.png";
	biographyList[3] = L"Cedric spent his entire life training to become guard captain for The Council. It was a joyous occasion when he finally received the title. When Ganfaul and his followers rose up against the church, Cedric and his allies were quickly overwhelmed by their powerful magic. Cedric was stomped into the dirt, left to die with his comrades. Somehow, he managed to crawl out of the fight and survive, but the scars of war never left him. He stands with the heroes now in a final grasp at redemption.";
	skill1TextList[3] = L"Splitting Strike";
	skill2TextList[3] = L"Armor Buster";
	skill3TextList[3] = L"Cleave";
	skill4TextList[3] = L"RAGE!";
	skill1IconList[3] = L"assets/images/WarriorSkills/Warrior_Skill_1.png";
	skill2IconList[3] = L"assets/images/WarriorSkills/Warrior_Skill_2.png";
	skill3IconList[3] = L"assets/images/WarriorSkills/Warrior_Skill_3.png";
	skill4IconList[3] = L"assets/images/WarriorSkills/Warrior_Skill_4.png";

	// Monk
	titleList[4] = L"Nina - Judgement's Fist";
	backgroundList[4] = L"assets/images/FantasyBackground4.jpg";
	spriteList[4] = L"assets/images/Renaud.png";
	biographyList[4] = L"Cedric spent his entire life training to become guard captain for The Council. It was a joyous occasion when he finally received the title. When Ganfaul and his followers rose up against the church, Cedric and his allies were quickly overwhelmed by their powerful magic. Cedric was stomped into the dirt, left to die with his comrades. Somehow, he managed to crawl out of the fight and survive, but the scars of war never left him. He stands with the heroes now in a final grasp at redemption.";
	skill1TextList[4] = L"Leg Sweep";
	skill2TextList[4] = L"Armor Break";
	skill3TextList[4] = L"Pressure Point";
	skill4TextList[4] = L"Break Ribs";
	skill1IconList[4] = L"assets/images/MonkSkills/Monk_Skill_1.png";
	skill2IconList[4] = L"assets/images/MonkSkills/Monk_Skill_2.png";
	skill3IconList[4] = L"assets/images/MonkSkills/Monk_Skill_3.png";
	skill4IconList[4] = L"assets/images/MonkSkills/Monk_Skill_4.png";

	titleList[5] = L"Ganfaul - The Demon Emperor";
	backgroundList[5] = L"assets/images/FantasyBackground5.jpg";
	spriteList[5] = L"assets/images/Renaud.png";
	biographyList[5] = L"Ganfaul was not always the Demon Emperor he is today, once considered a brave and revolutionary leader. He led the rebellion against the church years ago. Seeking to free the people of oppression, he managed to slay The Council and take their place as Emperor of Metis. He started obsessively studying dark magic to protect himself and became consumed by demonic power. Ganfaul’s armies marched across the realm and without action, he may soon become unstoppable...";
	skill1TextList[5] = L"Necrotic Infection";
	skill2TextList[5] = L"Soul Steal";
	skill3TextList[5] = L"Demonic Chant";
	skill4TextList[5] = L"Chaotic Strike";
	skill1IconList[5] = L"assets/images/GanfaulAbilities/Ganfaul_Skill_1.png";
	skill2IconList[5] = L"assets/images/GanfaulAbilities/Ganfaul_Skill_2.png";
	skill3IconList[5] = L"assets/images/GanfaulAbilities/Ganfaul_Skill_3.png";
	skill4IconList[5] = L"assets/images/GanfaulAbilities/Ganfaul_Skill_4.png";

	// Construct the loading screen prefab
	constructLoadingScreen();
}

void LoadingScreenController::initialize()
{
	// Show the screen
	randomizeLoadingScreen();
	Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mLoadingScreenPrefab, &mLoadingScreen, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }));
	Odyssey::TextProperties paragraphProperties;
	paragraphProperties.fontName = L"Tw Cen MT Condensed";
	paragraphProperties.fontSize = 18;
	paragraphProperties.textAlignment = Odyssey::TextAlignment::Left;
	paragraphProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	paragraphProperties.bold = false;
	mLogo = mLoadingScreen->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Sprite2D>()[6];
	mContinue = mLoadingScreen->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Text2D>()[6];
	mContinue->setOpacity(0.0f);
	mCurrentTime = 0.0f;
	mWaitTime = 2.5f;
}

std::shared_ptr<Odyssey::Component> LoadingScreenController::clone() const
{
	return std::make_shared<LoadingScreenController>(*this);
}

void LoadingScreenController::update(double deltaTime)
{
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
			opacity = 0.0f;
			direction = 1;
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

	// Set up the text properties for the title and paragraph
	Odyssey::TextProperties titleProperties;
	titleProperties.fontName = L"Tw Cen MT Condensed";
	titleProperties.fontSize = 32;
	titleProperties.textAlignment = Odyssey::TextAlignment::Left;
	titleProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	titleProperties.bold = true;

	Odyssey::TextProperties paragraphProperties;
	paragraphProperties.fontName = L"Tw Cen MT Condensed";
	paragraphProperties.fontSize = 20;
	paragraphProperties.textAlignment = Odyssey::TextAlignment::Left;
	paragraphProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	paragraphProperties.bold = false;

	// KEEP
	std::wstring odysseyWhite = L"assets/images/Odyssey_White_Mask.png";
	std::wstring odysseyBlack = L"assets/images/Odyssey_Black_Mask.png";
	std::wstring odysseyBronze = L"assets/images/Odyssey_Bronze_Logo.png";
	std::wstring healthIcon = L"assets/images/Meat.png";
	std::wstring manaIcon = L"assets/images/mp.png";
	std::wstring attackIcon = L"assets/images/Sword.png";
	std::wstring defenseIcon = L"assets/images/Shield.png";
	std::wstring speedIcon = L"assets/images/Speed.png";

	// Add the background image and the title/image
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), backgroundList[0], 1280, 720);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(90.0f, 70.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.6f), 650, 400);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 80.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 500, 100, titleList[0], titleProperties);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(850.0f, 105.0f), spriteList[0], 350, 500);

	// Add the summary and background
	paragraphProperties.italic = true;
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(115.0f, 130.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 610, 500, biographyList[0], paragraphProperties);

	// Skill 1
	paragraphProperties.fontSize = 24;
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(110.0f, 360.0f), skill1IconList[0], 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(170.0f, 362.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill1TextList[0], paragraphProperties);

	// Skill 2
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(410.0f, 360.0f), skill2IconList[0], 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(470.0f, 362.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill2TextList[0], paragraphProperties);

	// Skill 3
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(110.0f, 410.0f), skill3IconList[0], 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(170.0f, 412.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill3TextList[0], paragraphProperties);

	// Skill 4
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(410.0f, 410.0f), skill4IconList[0], 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(470.0f, 412.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill4TextList[0], paragraphProperties);

	// Odyssey logo
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 470.0f), odysseyBronze, 250, 250);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(500.0f, 600.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 610, 500, L"Press Enter to Continue...", titleProperties);
}

void LoadingScreenController::randomizeLoadingScreen()
{
	int index = std::rand() % 4;
	Odyssey::UICanvas* canvas = mLoadingScreenPrefab->getComponent<Odyssey::UICanvas>();
	std::vector<Odyssey::Sprite2D*> spriteElements = canvas->getElements<Odyssey::Sprite2D>();
	spriteElements[0]->setSprite(backgroundList[index]);
	spriteElements[1]->setSprite(spriteList[index]);
	spriteElements[2]->setSprite(skill1IconList[index]);
	spriteElements[3]->setSprite(skill2IconList[index]);
	spriteElements[4]->setSprite(skill3IconList[index]);
	spriteElements[5]->setSprite(skill4IconList[index]);

	std::vector<Odyssey::Text2D*> textElements = canvas->getElements<Odyssey::Text2D>();
	textElements[0]->setText(titleList[index]);
	textElements[1]->setText(biographyList[index]);
	textElements[2]->setText(skill1TextList[index]);
	textElements[3]->setText(skill2TextList[index]);
	textElements[4]->setText(skill3TextList[index]);
	textElements[5]->setText(skill4TextList[index]);
}