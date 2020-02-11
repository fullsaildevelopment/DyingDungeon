#include "TeamManager.h"
#include "TowerManager.h"
#include "CharacterFactory.h"

TeamManager& TeamManager::getInstance()
{
	static TeamManager instance;
	return instance;
}

std::vector<Odyssey::Entity*> TeamManager::CreateEnemyTeam(int _index)
{
	// Clear the enemy team
	mEnemyTeam.clear();
	
	// Get the list of enemies to create
	std::vector<EnemySetups> enemies = mEnemiesToCreate[_index];

	for (int i = 0; i < enemies.size(); i++)
	{
		EnemyType enemyType = enemies[i].pEnemyType;
		DirectX::XMVECTOR position = enemies[i].pPosition;
		DirectX::XMVECTOR rotation = enemies[i].pRotation;
		DirectX::XMFLOAT2 hudPosition = enemies[i].pHudPosition;
		DirectX::XMFLOAT2 hpPopupPosition = enemies[i].pHpPopupPosition;
		bool isBoss = enemies[i].pIsBoss;

		// Character we are about to create
		Odyssey::Entity* newCharacter;

		// Create the character based on the enum
		switch (enemyType)
		{
			case EnemyType::Skeleton:
				newCharacter = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Skeleton, L"Skeleton", position, rotation, hudPosition, true, hpPopupPosition, mSceneOne);
				break;
			case EnemyType::Ganfaul:
				newCharacter = CharacterFactory::getInstance().CreateCharacter(CharacterFactory::CharacterOptions::Ganfaul, L"Ganfaul", position, rotation, hudPosition, true, hpPopupPosition, mSceneOne);
				break;
			default:
				std::cout << "This enemy enum does not exist in the TeamManager.cpp CreateEnemyTeam function" << std::endl;
				break;
		}

		// Check if the enemy is a boss or not
		if (isBoss)
		{
			// Set the character as the boss
			//mCurrentTower->getComponent<TowerManager>()->SetBossCharacter(newCharacter);
		}

		// Add the character we created to the enemy team list
		mEnemyTeam.push_back(newCharacter);
	}

	// Set the enemy team to the createdEnemyTeam
	return mEnemyTeam;
}

void TeamManager::AddCharacterToPlayerTeam(Odyssey::Entity* _characterToAdd)
{
	mPlayerTeam.push_back(_characterToAdd);
}

void TeamManager::AddCharacterToEnemyTeam(Odyssey::Entity* _characterToAdd)
{
	mEnemyTeam.push_back(_characterToAdd);
}

void TeamManager::ClearPlayerTeam()
{
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		mPlayerTeam[i] = nullptr;
	}

	mPlayerTeam.clear();
	mPlayerTeam.resize(0);
}
