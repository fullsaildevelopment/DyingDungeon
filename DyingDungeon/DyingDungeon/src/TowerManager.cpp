#include "TowerManager.h"
#include "Transform.h"
#include "Character.h"
#include "HeroComponent.h"
#include "InputManager.h"
#include "EventManager.h"

CLASS_DEFINITION(Component, TowerManager)

TowerManager::~TowerManager()
{
	DestroyBattleInstance();
}

void TowerManager::initialize()
{
	// Create a Battle when we set up the tower !!THIS WILL BE TEMPORARY!!
	CreateBattleInstance();
}

void TowerManager::update(double deltaTime)
{
	// TODO: REMOVE POST BUILD 02
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::F3))
	{
		Rewards->setActive(!Rewards->isActive());
	}

	// If we are in battle, Update the battle
	if (GetTowerState() == IN_BATTLE)
	{
		// Update the current battle
		int result = mCurrentBattle->UpdateBattle();

		// If the result of the updated battle was DESTROY, destory the current battle instance
		if (result == mCurrentBattle->PLAYER_TEAM_DIED || result == mCurrentBattle->DESTORY)
		{
			// Destroy the battle instance
			DestroyBattleInstance();
			SetTowerState(IN_REWARDS);
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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
		{
			float tempXP = 0.0f;

			// Check to see if that was our last level for completing the tower
			if (GetCurrentLevel() > mNumberOfLevels)
			{
				std::cout << "You have completed the tower, Congratulations\n" << std::endl;
				SetTowerState(NOT_IN_BATTLE);

				// Give player some XP
				tempXP = 500.0f;
				// Print how much XP was given to the player
				mPlayerTeam[0]->getComponent<Character>()->AddExp(tempXP);
				std::cout << "Paladin gained " << tempXP << "XP for completing the level.\n" << std::endl;
				// Go to main menu screen
				GoToMainMenu();
			}
			else
			{
				std::cout << "The current level is " << GetCurrentLevel() << "\n" << std::endl;

				// Give player some XP
				tempXP = 100.0f;
				// Print how much XP was given to the player
				mPlayerTeam[0]->getComponent<Character>()->AddExp(tempXP);
				std::cout << "Paladin gained " << tempXP << "XP for completing the level.\n" << std::endl;

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

void TowerManager::SetUpTowerManager(EntityList _playerTeam, EntityList _enemyTeam, int _numberOfBattles)
{
	// Assign the player team and the enemy team
	mPlayerTeam = _playerTeam;
	mEnemyTeam = _enemyTeam;

	// Add all of the characters from the player's team to the allCharacters vector
	for (int i = 0; i < mPlayerTeam.size(); i++)
		mAllCharacters.push_back(mPlayerTeam[i]);

	// Add all of the characters from the enemy's team to the allCharacters vector
	for (int i = 0; i < mEnemyTeam.size(); i++)
		mAllCharacters.push_back(mEnemyTeam[i]);

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

	// Create the battle instance
	mCurrentBattle = new BattleInstance(mPlayerTeam, mEnemyTeam, TurnOrderNumbers);

	// Since we created a BattleInstance we will be in combat
	SetTowerState(IN_BATTLE);

	std::cout << "Created a battle instance\n" << std::endl;
	std::cout << "- Player Team\n" << std::endl;
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		Character* myChar = mPlayerTeam[i]->getComponent<Character>();
		std::cout << "- - " << myChar->GetName() <<  " - HP: " << myChar->GetHP() << "\n" << std::endl;
	}

	std::cout << "- Enemy Team\n" << std::endl;
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		Character* myChar = mEnemyTeam[i]->getComponent<Character>();
		std::cout << "- - " << myChar->GetName() << " - HP: " << myChar->GetHP() << "\n" << std::endl;
	}

	std::cout << "The current level is " << GetCurrentLevel() << "\n" << std::endl;
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

void TowerManager::GoToMainMenu()
{
	SetTowerState(NOT_IN_BATTLE);

	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		if (mPlayerTeam[i] != nullptr)
		{
			// Set all of the healths for each player on the enemy team back to 100 and their dead status to false
			// This will show a sim of entering a new battle
			mPlayerTeam[i]->getComponent<Character>()->SetHP(1000);
			mPlayerTeam[i]->getComponent<Character>()->SetMana(1000);
			mPlayerTeam[i]->getComponent<Character>()->SetDead(false);
			mPlayerTeam[i]->getComponent<Character>()->ClearStatusEffects();
			mPlayerTeam[i]->getComponent<Odyssey::Animator>()->playClip("Idle");
		}
	}

	Rewards->setActive(false);

	mCurrentLevel = 1;
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("MainMenu"));
}
