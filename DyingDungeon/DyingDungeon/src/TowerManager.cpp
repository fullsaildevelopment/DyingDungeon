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

	// Don't show the boos character
	//mBossCharacter->setActive(false);
	//GameUIManager::getInstance().GetCharacterHuds()[mBossCharacter->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(false);

	//// Reset the enemy team to the skely bois
	//if (mSkeletonTeam.size() != 0)
	//{
	//	// Clear enemy team
	//	mEnemyTeam.clear();
	//
	//	// Set the enemy team to be the skeleton team on start
	//	for (int i = 0; i < mSkeletonTeam.size(); i++)
	//	{
	//		mSkeletonTeam[i]->setActive(true);
	//		GameUIManager::getInstance().GetCharacterHuds()[mSkeletonTeam[i]->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(true);
	//		mEnemyTeam.push_back(mSkeletonTeam[i]);
	//	}
	//}

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

		// Remove the current enemy team from the scene
		for (int i = 0; i < mEnemyTeam.size(); i++)
		{
			// Turn off model
			mEnemyTeam[i]->setActive(false);
			// Turn off the HUD UI
			GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(false);
		}

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
		GameUIManager::getInstance().UpdateCharacterHealthPopups(deltaTime);
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
	// Add Boss to the mAllCharacters
	//mAllCharacters.push_back(mBossCharacter);

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
		// Turn off model
		mEnemyTeam[i]->setActive(false);
		// Turn off the HUD UI
		GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(false);
	}
	
	// Create the new enemy team before creating the battle
	mEnemyTeam = TeamManager::getInstance().CreateEnemyTeam(mCurrentLevel - 1);

	// Add all of the new characters from the enemy team to the allCharacters vector
	for (int i = 0; i < mEnemyTeam.size(); i++)
		mAllCharacters.push_back(mEnemyTeam[i]);

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

	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		if (mPlayerTeam[i] != nullptr)
		{
			// Set all of the healths for each player on the enemy team back to 100 and their dead status to false
			// This will show a sim of entering a new battle
			mPlayerTeam[i]->getComponent<Character>()->ResetMe();
			mPlayerTeam[i]->getComponent<Odyssey::Animator>()->playClip("Idle");

			// Turn off the previous canvases
			GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->pCanvas->setActive(false);
			// Turn off the skill canvases
			GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->pSkill1Canvas->setActive(false);
			GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->pSkill2Canvas->setActive(false);
			GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->pSkill3Canvas->setActive(false);
			GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->pSkill4Canvas->setActive(false);
			Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mPlayerTeam[i]));
		}
	}

	//// TODO: REFACTOR LATER
	//for (int i = 0; i < mEnemyTeam.size(); i++)
	//{
	//	if (mEnemyTeam[i])
	//	{
	//		scene->removeEntity(mEnemyTeam[i]);
	//	}
	//}

	// Deactivate the rewards screen
	Rewards->setActive(false);
	// Deactivate the pause menu
	Odyssey::Entity* pauseMenu = GameUIManager::getInstance().GetPauseMenu();
	GameUIManager::getInstance().ToggleCanvas(pauseMenu->getComponent<Odyssey::UICanvas>(), false);

	// Set the current level back to 1
	mCurrentLevel = 1;
	
	// Turn off battle music
	RedAudioManager::Instance().Stop("BackgroundBattle");

	delete mCurrentBattle;
	mCurrentBattle = nullptr;
	mIsPaused = true;
	// Switch to main menu scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("MainMenu"));
}
