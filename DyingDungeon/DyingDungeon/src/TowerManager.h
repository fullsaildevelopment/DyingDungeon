#pragma once
#include "Component.h"
#include "BattleInstance.h"
#include "Buffs.h"
#include "UICanvas.h"
#include "Text2D.h"

enum TOWER_STATE
{
	NOT_IN_BATTLE,
	IN_BATTLE,
	IN_REWARDS
};

class TowerManager : public Odyssey::Component
{
	CLASS_DECLARATION(TowerManager);
	
public: // Constructors
	TowerManager() = default;
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual ~TowerManager();

	Odyssey::UICanvas* UI;
	Odyssey::UICanvas* Rewards;

	// TODO: REFACTOR LATER
	Odyssey::Scene* scene;

public: // Functions

	// Necessary
	virtual void initialize();
	virtual void update(double deltaTime);
	
	void SetUpTowerManager(EntityList _playerTeam, int _numberOfBattles);
	void CreateBattleInstance();
	
	//Getters
	int GetTowerState() { return mTowerState; }

	EntityList GetEnemyTeam() { return mEnemyTeam; } // This will allow the battle instance to get the enemy team genreated by the Overlord

	Odyssey::Entity* GetBossCharacter() { return mBossCharacter; } // Get the boss character

	std::shared_ptr<float> GetStatitics() { return mStatitics; } // This will return the statics that the Overlord will be using to construct a new enemy team

	int GetCurrentLevel() { return mCurrentLevel; }

	BattleInstance* GetBattleInstance() { return mCurrentBattle; }

	//Setters
	void SetTowerState(TOWER_STATE _newTowerState) { mTowerState = _newTowerState; }

	void SetEnemyTeam(EntityList _newEnemyTeam) { mEnemyTeam = _newEnemyTeam; } // This will set the enemy team to the new enemy team that is generated by the Overlord

	void SetBossCharacter(Odyssey::Entity* _bossCharacter) { mBossCharacter = _bossCharacter; } // This will assign the boss character

	void SetStatitics(std::shared_ptr<float> _newStatitics) { mStatitics = _newStatitics; } // This will set a list of new statitics to the mStatitics varibale that will be used in the Overlord

private: // Varibales

	// Cheat Code Variables
	bool mUsedBossCheatCode = false;

	//Battle Instance
	BattleInstance* mCurrentBattle;

	// Vectors
	EntityList mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	EntityList mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	EntityList mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team
	EntityList mSkeletonTeam; // skeletonTeam will hold the gameObjects of skeltons // THIS IS TEMPORARY

	// Queues

	// Entities
	Odyssey::Entity* mBossCharacter; // This will be the object that is the boss character
	Odyssey::Entity* tmTurnIndicator; // This will be the object underneath the character who's turn it is

	// Ints
	int mCurrentLevel;
	int mNumberOfLevels;

	// Floats
	std::shared_ptr<float> mStatitics;

	// Bools
	bool mIsPaused = false;

	// Tower State
	TOWER_STATE mTowerState;

private: // Functions
	void DestroyBattleInstance();
	void TogglePauseMenu();
	void ShowOptionsMenu();
	void GoToMainMenu();
};

