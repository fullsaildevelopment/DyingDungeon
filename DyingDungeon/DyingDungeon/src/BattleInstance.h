#pragma once
#include "GameObject.h"
#include "Text2D.h"
#include <vector>
#include <queue>
#include <time.h>

typedef std::vector<std::shared_ptr<Odyssey::GameObject>> GameObjectList; //This will contain a list off shared pointer game objects

typedef std::queue<std::shared_ptr<Odyssey::GameObject>> GameObjectQueue; //This will contain a queue off shared pointer game objects

class BattleInstance
{

public: // Constructors
	BattleInstance(GameObjectList _playerTeam, GameObjectList _enemyTeam, std::vector<Odyssey::Text2D*> _turnOrderNumbers);

	enum BattleInstanceCommands
	{
		DESTORY = -1,
		CONTINUE = 1,
		PLAYER_TEAM_DIED
	};

public: // Functions

	int UpdateBattle();

	//Getters
	int GetCurrentRound() { return mCurrentRound; }

	//Setters

private: // Varibales

	// Vectors
	GameObjectList mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	GameObjectList mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	GameObjectList mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team
	std::vector<Odyssey::Text2D*> mTurnOrderNumbers;

	// Queues
	GameObjectQueue mBattleQueue; // Battle Queue that will hodl the order in which players can attack

	// GameObjects
	std::shared_ptr<Odyssey::GameObject> mCurrentCharacter; // This will hold the current player who's turn it is

	// Ints
	int mTurnCounter;
	int mCurrentRound;

	// Floats


	// Bools


private: // Functions
	void GenerateBattleQueue(); // This will generate the battle queue for the character turn orders
	bool IsTeamAlive(GameObjectList _teamToCheck); // This will check to see if at least one character from the passed in team is alive
	void UpdateCharacterTurnNumbers(); // This will update each character's turn order text
};