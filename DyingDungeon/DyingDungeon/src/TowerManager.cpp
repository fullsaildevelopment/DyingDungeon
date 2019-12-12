#include "TowerManager.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, TowerManager)

TowerManager::TowerManager(GameObjectList _playerTeam, GameObjectList _enemyTeam, int _numberOfBattles)
{
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

	mCurrentBattle = std::make_shared<BattleInstance>(mPlayerTeam, mEnemyTeam);
}

void TowerManager::update(double deltaTime)
{
	mCurrentBattle->UpdateBattle();
}