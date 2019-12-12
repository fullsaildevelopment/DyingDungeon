#pragma once
#include "MyTypeDefs.h"
#include <queue>
#include <time.h>

enum BATTLE_STATE
{
	IN_BATTLE,
	BATTLE_OVER
};

class BattleInstance : public Odyssey::Component
{
	CLASS_DECLARATION(BattleInstance);

public: // Constructors
	BattleInstance(GameObjectList _playerTeam, GameObjectList _enemyTeam);

public: // Functions

	// Necessary
	virtual void initialize(Odyssey::GameObject* _parent);
	virtual void update(double deltaTime);

	//Getters
	int GetBattleState(){ return mBattleState; }

	//Setters


private: // Varibales

	// Vectors
	GameObjectList mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	GameObjectList mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	GameObjectList mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team

	// Queues
	GameObjectQueue mBattleQueue; // Battle Queue that will hodl the order in which players can attack

	// GameObjects
	std::shared_ptr<Odyssey::GameObject> mCurrentCharacter; // This will hold the current player who's turn it is

	// Ints


	// Floats


	// Bools
	bool mIsBattleOver;

	// Battle State
	BATTLE_STATE mBattleState;

private: // Functions
	void GenerateBattleQueue(); // This will generate the battle queue for the character turn orders
	bool IsTeamAlive(GameObjectList _teamToCheck); // This will check to see if at least one character from the passed in team is alive
};

