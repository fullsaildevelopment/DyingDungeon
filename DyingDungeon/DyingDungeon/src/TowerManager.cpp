#include "TowerManager.h"
#include "Transform.h"
#include "Character.h"
#include "HeroComponent.h"
#include "InputManager.h"
#include "EventManager.h"
#include "StatusEvents.h"
#include "GameUIManager.h"
#include "StatusEvents.h"
#include "UICanvas.h"
#include "RedAudioManager.h"
#include "TeamManager.h"
#include "CharacterFactory.h"
#include "CharacterHUDElements.h"
#include "SkillHUDElements.h"
#include "SkillHoverComponent.h"

CLASS_DEFINITION(Component, TowerManager)

std::shared_ptr<Odyssey::Component> TowerManager::clone() const
{
	return std::make_shared<TowerManager>(*this);
}

TowerManager::~TowerManager()
{
	DestroyBattleInstance();
}

void TowerManager::initialize()
{
	// Reset cheat code bools
	mUsedBossCheatCode = false;

	// The tower will not be paused on start up
	mIsPaused = false;

	// Create the player team
	CreateThePlayerTeam();

	// Create a Battle when we set up the tower !!THIS WILL BE TEMPORARY!!
	CreateBattleInstance();

	// Set the pause menu button callbacks
	GameUIManager::getInstance().GetResumeButton()->registerCallback("onMouseClick", this, &TowerManager::TogglePauseMenu);
	GameUIManager::getInstance().GetOptionsButton()->registerCallback("onMouseClick", this, &TowerManager::ShowOptionsMenu);
	GameUIManager::getInstance().GetMainMenuButton()->registerCallback("onMouseClick", this, &TowerManager::GoToMainMenu);
}

void TowerManager::update(double deltaTime)
{
	// Always look for the pause input button
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::P))
	{
		// Turn the pause menu either on or off
		TogglePauseMenu();
	}

	// Toggles the combat canvas
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Tab))
	{
		Odyssey::UICanvas* combatCanvas = GameUIManager::getInstance().GetCombatLogCanvas();

		GameUIManager::getInstance().ToggleCanvas(combatCanvas, !combatCanvas->isActive());
	}

	// Go straight to the BOSS when F3 is hit
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::F3))
	{
		// Destroy the current battle instance
		DestroyBattleInstance();
		// Set the game to in rewards 
		mTowerState = IN_REWARDS;
		// Set the tower level to the last level which is the boss
		mCurrentLevel = mNumberOfLevels;
		// Set the cheat code bool
		mUsedBossCheatCode = true;
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::M))
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

	// Don't update unless the game is not paused
	if (!mIsPaused)
	{
		// Update the health popups
		//GameUIManager::getInstance().UpdateCharacterHealthPopups(deltaTime);
		// Update the UI bars
		GameUIManager::getInstance().UpdateCharacterBars(deltaTime);

		// If we are in battle, Update the battle
		if (GetTowerState() == IN_BATTLE)
		{
			int result = mCurrentBattle->UpdateBattle();

			// If the result of the updated battle was DESTROY, destory the current battle instance
			if (result == mCurrentBattle->PLAYER_TEAM_DIED || result == mCurrentBattle->DESTORY)
			{
				// Destroy the battle instance
				DestroyBattleInstance();
				SetTowerState(IN_REWARDS);
			    Odyssey::EventManager::getInstance().publish(new RewardsActiveEvent(mCurrentLevel));
				Rewards->setActive(true);

				//Check to see if the update returned PLAYER_TEAM_DIED
				if (result == mCurrentBattle->PLAYER_TEAM_DIED)
				{
					std::cout << "You FAILED to complete the tower, Get Better\n" << std::endl;
					std::cout << "Your team DIED!!!!!!!\n" << std::endl;

					SetTowerState(NOT_IN_BATTLE);
					// Go to main menu screen
					GoToMainMenu();
				}
				else
				{
					// Update to the next level
					mCurrentLevel = GetCurrentLevel() + 1;
				}
			}
		}
		else if (GetTowerState() == IN_REWARDS)
		{
			if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter) || mUsedBossCheatCode)
			{
				// Reset cheat code bool
				mUsedBossCheatCode = false;
				// Create temp xp variable
				float tempXP = 0.0f;

				// Check to see if that was our last level for completing the tower
				if (GetCurrentLevel() > mNumberOfLevels)
				{
					std::cout << "You have completed the tower, Congratulations\n" << std::endl;
					SetTowerState(NOT_IN_BATTLE);

					// Give player some XP
					tempXP = 500.0f;
					// Print how much XP was given to the players
					for (int i = 0; i < mPlayerTeam.size(); i++)
					{
						mPlayerTeam[i]->getComponent<Character>()->AddExp(tempXP);
					}
					std::cout << "Player team characters gained " << tempXP << "XP for completing the tower.\n" << std::endl;
					// Go to main menu screen
					GoToMainMenu();
				}
				else
				{
					//// If this is the last level of the tower, spawn the boss
					//if (mCurrentLevel == mNumberOfLevels)
					//{
					//	// Turn off the other enemies
					//	for (int i = 0; i < mEnemyTeam.size(); i++)
					//	{
					//		mEnemyTeam[i]->setActive(false);
					//		GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(false);
					//		mSkeletonTeam.push_back(mEnemyTeam[i]);
					//	}
					//
					//	// Now active the boos and only add the boss to the enemy list
					//	mBossCharacter->setActive(true);
					//	// Turn on Ganny's UI
					//	GameUIManager::getInstance().GetCharacterHuds()[mBossCharacter->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(true);
					//	mEnemyTeam.push_back(mBossCharacter);
					//}

					std::cout << "The current level is " << mCurrentLevel << "\n" << std::endl;

					// Give player some XP
					tempXP = 100.0f;
					// Print how much XP was given to the player
					for (int i = 0; i < mPlayerTeam.size(); i++)
					{
						Character* currCharacter = mPlayerTeam[i]->getComponent<Character>();
						currCharacter->AddExp(tempXP);

						// If the player made the finishing kill set him back to NONE state
						if (currCharacter->GetState() == STATE::FINISHED)
						{
							currCharacter->SetState(STATE::NONE);
						}
					}
					std::cout << "Player team characters gained " << tempXP << "XP for completing the level.\n" << std::endl;

					// Make a new battle to continue the tower
					CreateBattleInstance();
				}

				// Turn off the rewads screen
				Rewards->setActive(false);
			}
		}
		else if (GetTowerState() == NOT_IN_BATTLE)
		{

		}
	}
}

void TowerManager::SetUpTowerManager(int _numberOfBattles)
{
	// Set the current level to 1
	mCurrentLevel = 1;

	// Set the number of levels for this tower
	mNumberOfLevels = _numberOfBattles;
	mCurrentBattle = nullptr;
}

void TowerManager::CreateBattleInstance()
{
	// TODO: REMOVE POST BUILD 02
	if (mCurrentLevel == 1)
		system("CLS");

	// Clear the combat at the start of each battle log
	GameUIManager::getInstance().ClearCombatLog();

	// Send off the current level number
	Odyssey::EventManager::getInstance().publish(new LevelStartEvent(mCurrentLevel, mPlayerTeam[0]->getComponent<Character>()->GetName(), mPlayerTeam[1]->getComponent<Character>()->GetName(), mPlayerTeam[2]->getComponent<Character>()->GetName(),
																					mPlayerTeam[0]->getComponent<Character>()->GetPortraitPath(), mPlayerTeam[1]->getComponent<Character>()->GetPortraitPath(), mPlayerTeam[2]->getComponent<Character>()->GetPortraitPath(),
																					mPlayerTeam[0]->getComponent<Character>(), mPlayerTeam[1]->getComponent<Character>(), mPlayerTeam[2]->getComponent<Character>()));

	// Remove the current enemy team from the scene
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		// Destory the previous enemy's UI Elements
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the previous enemy's impact indicator
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetInpactIndicator()));
		// Destroy the previous enemy's blood particle effect
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetPSBlood()->getEntity()));
		// Destroy the previous enemies
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]));
	}
	
	// Create the new enemy team before creating the battle
	mEnemyTeam = TeamManager::getInstance().CreateEnemyTeam(mCurrentLevel - 1);

	// Add all of the new characters from the enemy team to the allCharacters vector
	for (int i = 0; i < mEnemyTeam.size(); i++)
		mAllCharacters.push_back(mEnemyTeam[i]);

	// Set up clickable character UI
	GameUIManager::getInstance().SetupClickableCharacterUI();

	// Create the battle instance
	mCurrentBattle = new BattleInstance(mPlayerTeam, mEnemyTeam);

	// Since we created a BattleInstance we will be in combat
	SetTowerState(IN_BATTLE);

	std::cout << "Created a battle instance\n" << std::endl;
	std::cout << "- Player Team\n" << std::endl;
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		Character* myChar = mPlayerTeam[i]->getComponent<Character>();
	}

	std::cout << "- Enemy Team\n" << std::endl;
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		Character* myChar = mEnemyTeam[i]->getComponent<Character>();
	}

	std::cout << "The current level is " << mCurrentLevel << "\n" << std::endl;

}

void TowerManager::DestroyBattleInstance()
{
	// Destory pointer and set it to a nullptr
	if (mCurrentBattle)
	{
		// Destroy the current mTurnIndicator in the current battle
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mCurrentBattle->GetTurnIndicator()));

		// Delete the current battle
		delete mCurrentBattle;
		mCurrentBattle = nullptr;
	}
	
	// Since we destoryed the BattleInstance we will NOT be in combat
	SetTowerState(NOT_IN_BATTLE);

	std::cout << "Destroyed a battle instance\n" << std::endl;
}

void TowerManager::TogglePauseMenu()
{
	//Toggle mIsPaused bool
	mIsPaused = !mIsPaused;

	// Toggle pause menu canvas
	Odyssey::UICanvas* pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu()->getComponent<Odyssey::UICanvas>();
	GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, !pauseMenuCanvas->isActive());

	// Loop through all of the characters and toggle their animator
	for (int i = 0; i < mAllCharacters.size(); i++)
		mAllCharacters[i]->getComponent<Odyssey::Animator>()->setActive(!mAllCharacters[i]->getComponent<Odyssey::Animator>()->isActive());
}

void TowerManager::ShowOptionsMenu()
{
	// Turn off the pause menu
	Odyssey::UICanvas* pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu()->getComponent<Odyssey::UICanvas>();
	GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, false);

	// Turn on the options menu
	Odyssey::UICanvas* optionsMenuCanvas = GameUIManager::getInstance().GetOptionsMenu()->getComponent<Odyssey::UICanvas>();
	GameUIManager::getInstance().ToggleCanvas(optionsMenuCanvas, true);
}

void TowerManager::GoToMainMenu()
{
	SetTowerState(NOT_IN_BATTLE);

	// Remove the current player team from the scene
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		// Destory the previous player's UI Elements
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the previous player's clickable box
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetClickableUIElements()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the previous player's impact indicator
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mPlayerTeam[i]->getComponent<Character>()->GetInpactIndicator()));
		// Destroy the previous player's blood particle effect
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mPlayerTeam[i]->getComponent<Character>()->GetPSBlood()->getEntity()));
		// Destroy the previous player
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mPlayerTeam[i]));
	}

	// Remove the current enemy team from the scene
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		// Destory the previous enemy's UI Elements
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the enemy's clickable box
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetClickableUIElements()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the previous enemy's impact indicator
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetInpactIndicator()));
		// Destroy the previous enemy's blood particle effect
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetPSBlood()->getEntity()));
		// Destroy the previous enemies
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]));
	}

	// Deactivate the rewards screen
	Rewards->setActive(false);
	// Deactivate the pause menu
	Odyssey::Entity* pauseMenu = GameUIManager::getInstance().GetPauseMenu();
	GameUIManager::getInstance().ToggleCanvas(pauseMenu->getComponent<Odyssey::UICanvas>(), false);

	// Set the current level back to 1
	mCurrentLevel = 1;
	
	// Turn off battle music
	RedAudioManager::Instance().Stop("BackgroundBattle");

	// Destory the battle instance
	DestroyBattleInstance();

	mIsPaused = true;
	// Switch to main menu scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("MainMenu"));
}

void TowerManager::CreateThePlayerTeam()
{
	// Create the player characters
	std::vector<DirectX::XMVECTOR> mPlayerPositions;
	mPlayerPositions.resize(3);
	mPlayerPositions[0] = DirectX::XMVectorSet(-5.0f, 0.0f, 10.0f, 1.0f); // First Character Selected
	mPlayerPositions[1] = DirectX::XMVectorSet(0.0f, 0.0f, 10.0f, 1.0f); // Second Character Selected
	mPlayerPositions[2] = DirectX::XMVectorSet(5.0f, 0.0f, 10.0f, 1.0f); // Third Character Selected

	// Create each player
	for (int i = 0; i < TeamManager::getInstance().GetPlayerTeamToCreate().size(); i++)
	{
		// Character we are about to create
		Odyssey::Entity* newCharacter = nullptr;
		// Character's HUD
		Odyssey::Entity* newHUD = nullptr;
		// Prefab
		Odyssey::Entity* prefab;
		// Hud Id type
		CharacterFactory::HudID hudID;
		// Clickable Character position
		GameUIManager::ClickableCharacterUI clickablePos;
		// Position taht the skill popup will needed to be spawned at
		SkillHoverComponent::HudPosition skillPopupPos;
		// Define the character type we need to create
		CharacterFactory::CharacterOptions characterToCreate = CharacterFactory::CharacterOptions::Paladin;
		// Get the hero type
		TeamManager::HeroType newHeroType = TeamManager::getInstance().GetPlayerTeamToCreate()[i];
		// Set the player rotations
		DirectX::XMVECTOR mPlayerRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		if (i == 0)
		{
			hudID = CharacterFactory::HudID::HeroLeft;
			skillPopupPos = SkillHoverComponent::HudPosition::Left;
			clickablePos = GameUIManager::ClickableCharacterUI::HeroLeft;
		}
		else if (i == 1)
		{
			hudID = CharacterFactory::HudID::HeroMiddle;
			skillPopupPos = SkillHoverComponent::HudPosition::Middle;
			clickablePos = GameUIManager::ClickableCharacterUI::HeroMiddle;
		}
		else
		{
			hudID = CharacterFactory::HudID::HeroRight;
			skillPopupPos = SkillHoverComponent::HudPosition::Right;
			clickablePos = GameUIManager::ClickableCharacterUI::HeroRight;
		}

		// Set the enum based on the name of the character
		if (newHeroType == TeamManager::HeroType::Paladin)
			characterToCreate = CharacterFactory::CharacterOptions::Paladin;
		else if (newHeroType == TeamManager::HeroType::Mage)
			characterToCreate = CharacterFactory::CharacterOptions::Mage;
		else if (newHeroType == TeamManager::HeroType::Bard)
			characterToCreate = CharacterFactory::CharacterOptions::Bard;
		else if (newHeroType == TeamManager::HeroType::Warrior)
			characterToCreate = CharacterFactory::CharacterOptions::Warrior;
		else if (newHeroType == TeamManager::HeroType::Monk)
		{
			characterToCreate = CharacterFactory::CharacterOptions::Monk;
			mPlayerRotation = DirectX::XMVectorSet(DirectX::XMVectorGetX(mPlayerRotation), 180.0f, DirectX::XMVectorGetZ(mPlayerRotation), 1.0f);
		}
		else
			std::cout << "Not the correct hero type so we defaulted to Paladin in TowerManager.cpp Init()";


		// Create the character prefab
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(characterToCreate);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, mPlayerPositions[i], mPlayerRotation));

		// Create the hud prefab
		prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, mPlayerPositions[i], mPlayerRotation));

		// Set up the clickable UI and skill hover huds
		CharacterHUDElements* hudElements = newHUD->getComponent<CharacterHUDElements>();
		SkillHoverComponent* hover = newHUD->addComponent<SkillHoverComponent>();

		// Clickable UI
		HeroComponent* heroComp = newCharacter->getComponent<HeroComponent>();
		heroComp->SetupClickableUI(hudElements->GetSkill1(), hudElements->GetSkill2(), hudElements->GetSkill3(), hudElements->GetSkill4());

		// Assign the character component
		hover->characterComponent = newCharacter->getComponent<Character>();
		// Register the skill sprites for hovering over them
		hover->registerSprite(hudElements->GetSkill1());
		hover->registerSprite(hudElements->GetSkill2());
		hover->registerSprite(hudElements->GetSkill3());
		hover->registerSprite(hudElements->GetSkill4());
		// Assign the characters skills to the hover list
		hover->mCharacterSkills = newCharacter->getComponent<Character>()->GetSkills();
		// Assign the position that the prefab will needed to be spawned at
		hover->mHudPositionEnum = skillPopupPos;

		// Create the hero clickable UI box
		Odyssey::Entity* clickableHeroUI = nullptr;
		prefab = GameUIManager::getInstance().GetClickableUIPrefab(clickablePos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &clickableHeroUI, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }));
		GameUIManager::getInstance().AddClickableElementToList(clickableHeroUI);
		
		// Create the impact indicator for the heroes
		Odyssey::Entity* impactIndicator = nullptr;
		DirectX::XMVECTOR impactIndicatorPosition = mPlayerPositions[i];
		prefab = CharacterFactory::getInstance().GetImpactIndicatorPrefab();
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &impactIndicator, impactIndicatorPosition, mPlayerRotation));
		// Assign the impact indicator for the heroes
		impactIndicator->setActive(false);
		newCharacter->getComponent<Character>()->SetImpactIndicator(impactIndicator);

		// Create the blood effect for the heroes
		Odyssey::Entity* bloodEffect = nullptr;
		DirectX::XMVECTOR bloodEffectPosition = { DirectX::XMVectorGetX(mPlayerPositions[i]), DirectX::XMVectorGetY(mPlayerPositions[i]) + 5.0f, DirectX::XMVectorGetZ(mPlayerPositions[i]) };
		prefab = CharacterFactory::getInstance().GetBloodEffectPrefab();
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &bloodEffect, bloodEffectPosition, mPlayerRotation));
		// Assign the blood effect for the heroes
		newCharacter->getComponent<Character>()->SetPSBlood(bloodEffect->getComponent<Odyssey::ParticleSystem>());

		// Set the character's hud index number
		newCharacter->getComponent<Character>()->SetHudIndex(CharacterFactory::getInstance().GetCharacterHudIndex());
		// Increase the character index
		CharacterFactory::getInstance().IncreaseCharacterHUDIndex();
		// Add the new HUD to the list of HUDs
		GameUIManager::getInstance().AddHudToList(newHUD);

		// Set the elements of the character's HUD
		GameUIManager::getInstance().AssignCharacterHudElements(newCharacter->getComponent<Character>(), newHUD);

		// Add the mudda fricken character in the player list
		mPlayerTeam.push_back(newCharacter);
	}
}
