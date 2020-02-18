#include "TeamManager.h"
#include "TowerManager.h"
#include "CharacterFactory.h"
#include "Character.h"

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
		//DirectX::XMFLOAT2 hudPosition = enemies[i].pHudPosition;
		//DirectX::XMFLOAT2 hpPopupPosition = enemies[i].pHpPopupPosition;
		//bool isBoss = enemies[i].pIsBoss;

		// Character we are about to create
		Odyssey::Entity* newCharacter = nullptr;
		// Character's HUD
		Odyssey::Entity* newHUD = nullptr;
		// Hud Id type
		CharacterFactory::HudID hudID;
		// Prefab we will need
		Odyssey::Entity* prefab;

		// Determine the enemy hud we need based on the enemy list
		// If there is only 1 player on the enemy team to start out with, make sur ehe is placed in the middle
		if (i == 0 && enemies.size() != 1)
			hudID = CharacterFactory::HudID::EnemyLeft;
		else if (i == 1 || enemies.size() == 1)
			hudID = CharacterFactory::HudID::EnemyMiddle;
		else
			hudID = CharacterFactory::HudID::EnemyRight;

		// Create the character based on the enum
		switch (enemyType)
		{
			case EnemyType::Skeleton:
				// Spawn Enemy
				prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Skeleton);
				Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
				// Spawn Enemy HUD
				prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
				Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
				break;
			case EnemyType::Ganfaul:
				prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Ganfaul);
				Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
				// Spawn Enemy HUD
				prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
				Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
				break;
			default:
				std::cout << "This enemy enum does not exist in the TeamManager.cpp CreateEnemyTeam function" << std::endl;
				break;
		}

		// Set the character's hud index number
		newCharacter->getComponent<Character>()->SetHudIndex(CharacterFactory::getInstance().GetCharacterHudIndex());
		// Increase the character index
		CharacterFactory::getInstance().IncreaseCharacterHUDIndex();
		// Add the new HUD to the list of HUDs
		GameUIManager::getInstance().AddHudToList(newHUD);

		// Set the elements of the character's HUD
		GameUIManager::getInstance().AssignCharacterHudElements(newCharacter->getComponent<Character>(), newHUD);

		// Add the character we created to the enemy team list
		mEnemyTeam.push_back(newCharacter);
	}

	// Set the enemy team to the createdEnemyTeam
	return mEnemyTeam;
}

void TeamManager::AddCharacterEnumToPlayerTeam(HeroType _characterHeroType)
{
	mPlayerTeamToCreate.push_back(_characterHeroType);
}

void TeamManager::AddCharacterToEnemyTeam(Odyssey::Entity* _characterToAdd)
{
	mEnemyTeam.push_back(_characterToAdd);
}

void TeamManager::ClearPlayerTeamEnumList()
{
	mPlayerTeamToCreate.clear();
	mPlayerTeamToCreate.resize(0);
}
