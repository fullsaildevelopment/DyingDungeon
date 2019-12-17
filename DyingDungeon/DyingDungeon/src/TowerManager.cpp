#include "TowerManager.h"
#include "Transform.h"
#include "Character.h"
#include "HeroComponent.h"

CLASS_DEFINITION(Component, TowerManager)

TowerManager::TowerManager(GameObjectList _playerTeam, GameObjectList _enemyTeam, int _numberOfBattles)
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

	SetCurrentLevel(1);

	// Set the number of levels for this tower
	mNumberOfLevels = _numberOfBattles;
}

TowerManager::~TowerManager()
{
	DestroyBattleInstance();
}

void TowerManager::initialize()
{
	onEnable();

	// Create the battle on init, but this is TEMPORARY
	CreateBattleInstance();

	std::cout << "The current level is " << GetCurrentLevel() << "\n" << std::endl;
}

void TowerManager::update(double deltaTime)
{
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

			//Check to see if the update returned PLAYER_TEAM_DIED
			if (result == mCurrentBattle->PLAYER_TEAM_DIED)
			{
				std::cout << "You FAILED to complete the tower, Get Better\n" << std::endl;
				SetTowerState(NOT_IN_BATTLE);
			}
			else
			{
				// Set all of the healths for each player on the enemy team back to 100 and their dead status to false
				// This will show a sim of entering a new battle
				for (int i = 0; i < mEnemyTeam.size(); i++)
				{
					mEnemyTeam[i]->getComponent<Character>()->SetHP(100);
					mEnemyTeam[i]->getComponent<Character>()->SetDead(false);
				}

				// Update to the next level
				SetCurrentLevel(GetCurrentLevel() + 1);

				// Check to see if that was our last level for completing the tower
				if (GetCurrentLevel() > mNumberOfLevels)
				{
					std::cout << "You have completed the tower, Congratulations\n" << std::endl;
				}
				else
				{
					std::cout << "The current level is " << GetCurrentLevel() << "\n" << std::endl;
					CreateBattleInstance();
				}
			}
		}
	}
}

void TowerManager::CreateBattleInstance()
{
	// Create the battle instance
	mCurrentBattle = new BattleInstance(mPlayerTeam, mEnemyTeam);

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
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		Character* myChar = mEnemyTeam[i]->getComponent<Character>();
		std::cout << "- - " << myChar->GetName() << " - HP: " << myChar->GetHP() << "\n" << std::endl;
	}

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