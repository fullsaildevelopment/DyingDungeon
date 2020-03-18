#pragma once
#include "Entity.h"
#include "Text2D.h"
#include <vector>
#include <queue>
#include <time.h>

typedef std::vector<Odyssey::Entity*> EntityList; //This will contain a list off shared pointer game objects

typedef std::queue<Odyssey::Entity*> EntityQueue; //This will contain a queue off shared pointer game objects

class BattleInstance
{

public: // Constructors
	BattleInstance(EntityList _playerTeam, EntityList _enemyTeam, bool isTutorial);

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

	// Get the turn indicator entity
	Odyssey::Entity* GetTurnIndicator() { return mTurnIndicator; }

	// Get the current character
	Odyssey::Entity* GetCurrentCharacter() { return mCurrentCharacter; }

	//Setters
	void SetTutorialState(bool state) { mIsTutorial = state; };

private: // Varibales

	// Vectors
	EntityList mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	EntityList mPlayerTeam; // playerTeam will hold the Entitys associated with the Player's team
	EntityList mEnemyTeam; // enemyTeam will hold the Entitys associated with the Overlord's team

	// Queues
	EntityQueue mBattleQueue; // Battle Queue that will hodl the order in which players can attack

	// Entitys
	Odyssey::Entity* mCurrentCharacter; // This will hold the current player who's turn it is
	Odyssey::Entity* mTurnIndicator; // This will be the object underneath the character who's turn it is

	// Ints
	int mTurnCounter = -1;
	int mCurrentRound = 1;

	// Floats


	// Bools
	bool mIsTutorial = false;

private: // Functions
	void GenerateBattleQueue(); // This will generate the battle queue for the character turn orders
	bool IsTeamAlive(EntityList _teamToCheck); // This will check to see if at least one character from the passed in team is alive
	void UpdateCharacterTurnNumbers(); // This will update each character's turn order text
	void SetTurnIndicatorPosition(); // This will update the turn indicator model to be at the feet of the current character who's turn it is
};

class SetPlayerToDead : public Odyssey::Event
{
public:
	SetPlayerToDead()
	{

	}
};