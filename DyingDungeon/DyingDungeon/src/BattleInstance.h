#pragma once
#include "Component.h"
#include <queue>
#include <vector>
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
	BattleInstance(std::vector<std::shared_ptr<Odyssey::GameObject>> _playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> _enemyTeam);

public: // Functions

	// Necessary
	virtual void initialize(Odyssey::GameObject* _parent);
	virtual void update(double deltaTime);

	//Getters
	int GetBattleState(){ return mBattleState; }

	//Setters


private: // Varibales

	// Vectors
	std::vector<std::shared_ptr<Odyssey::GameObject>> mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	std::vector<std::shared_ptr<Odyssey::GameObject>> mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	std::vector<std::shared_ptr<Odyssey::GameObject>> mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team

	// Queues
	std::queue<std::shared_ptr<Odyssey::GameObject>> mBattleQueue; // Battle Queue that will hodl the order in which players can attack

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
	bool IsTeamAlive(std::vector<std::shared_ptr<Odyssey::GameObject>> _teamToCheck); // This will check to see if at least one character from the passed in team is alive
};

