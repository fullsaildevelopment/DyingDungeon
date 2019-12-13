#include "TowerManager.h"
#include "GameObject.h"
#include "Transform.h"

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
	mCurrentLevel = 1;

	// Set the number of levels for this tower
	mNumberOfLevels = _numberOfBattles;
}

void TowerManager::initialize(Odyssey::GameObject* _parent)
{
	onEnable();
	mGameObject = _parent;
	mGameObject->addComponent<Odyssey::Transform>();

	// Create the battle on init, but this is TEMPORARY
	CreateBattleInstance();
}

void TowerManager::update(double deltaTime)
{
	// If we are in battle, Update the battle
	if (GetTowerState() == IN_BATTLE)
	{
		// Update the current battle
		int result = mCurrentBattle->UpdateBattle();

		// If the result of the updated battle was DESTROY, destory the current battle instance
		if (result == mCurrentBattle->DESTORY)
		{
			// Destory the battle instance
			DestroyBattleInstance();
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
}

void TowerManager::DestroyBattleInstance()
{
	// Destory pointer and set it to a nullptr
	delete mCurrentBattle;

	// Since we destoryed the BattleInstance we will NOT be in combat
	SetTowerState(NOT_IN_BATTLE);

	std::cout << "Destroyed a battle instance\n" << std::endl;
}