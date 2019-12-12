#include "BattleInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "Character.h"

BattleInstance::BattleInstance(GameObjectList _playerTeam, GameObjectList _enemyTeam)
{
	mPlayerTeam = _playerTeam;
	mEnemyTeam = _enemyTeam;

	// Add all of the characters from the player's team to the allCharacters vector
	for (int i = 0; i < mPlayerTeam.size(); i++)
		mAllCharacters.push_back(mPlayerTeam[i]);

	// Add all of the characters from the enemy's team to the allCharacters vector
	for (int i = 0; i < mEnemyTeam.size(); i++)
		mAllCharacters.push_back(mEnemyTeam[i]);

	// Set time to be random
	srand(static_cast<int>(time(NULL)));

	//Set the state of the battle
	mIsBattleOver = false;

	// Create the battle queue before going to battle
	GenerateBattleQueue();

	// Set the pCurrentCharacter to the front of the battle queue
	mCurrentCharacter = mBattleQueue.front();

	// Update Battle State
	mBattleState = IN_BATTLE;
}

void BattleInstance::UpdateBattle()
{
	// Check to see if both teams have at least one character still alive
	if (IsTeamAlive(mPlayerTeam) && IsTeamAlive(mEnemyTeam))
	{
		// Check to see if the current charcter is even alive before the character takes its turn
		if (mCurrentCharacter->getComponent<Character>()->IsDead())
		{
			// Take the current character out of the battle queue
			mBattleQueue.pop();
			// Reassign the next character to the 
			mCurrentCharacter = mBattleQueue.front();
		}
		// Has the current player taken it's turn yet
		else if (mCurrentCharacter->getComponent<Character>()->TakeTurn(mAllCharacters))
		{
			// Take the current character out of the front of the line
			mBattleQueue.pop();
			// Put the current character to back into the queue, he will go to the back of the line
			mBattleQueue.emplace(mCurrentCharacter);
			// Reassign the next character to the 
			mCurrentCharacter = mBattleQueue.front();
		}
	}
	else
	{
		// The current battle has ended, at least one team is completely dead
		std::cout << "The battle has ended!\n" << std::endl;

		// Update Battle State
		mBattleState = BATTLE_OVER;
	}
}

void BattleInstance::GenerateBattleQueue()
{
	// This will hold all of the characters that will be in the match
	GameObjectList characterPool;

	// Add each character from the player's team to the characetr pool 
	for (int i = 0; i < mPlayerTeam.size(); i++)
		characterPool.push_back(mPlayerTeam[i]);

	// Add each character from the enemy's team to the characetr pool 
	for (int i = 0; i < mEnemyTeam.size(); i++)
		characterPool.push_back(mEnemyTeam[i]);

	// Get the beginning count of the character pool
	int numOfCharacters = static_cast<int>(characterPool.size());
	// What? - Loop for the number of beginning amount of characters
	// Why? - Because we will be removing characters from the pool when we add the character to the battle queue 
	for (int i = 0; i < numOfCharacters; i++)
	{
		// Get random index from the character pool
		int rndIndex = rand() % characterPool.size();

		// Add the character into the battle queue
		mBattleQueue.push(characterPool[rndIndex]);

		// Remove the character from the character pool so he won't be added to the battle queue multiple times
		characterPool.erase(characterPool.begin() + rndIndex);
	}
}

bool BattleInstance::IsTeamAlive(GameObjectList _teamToCheck)
{
	for (int i = 0; i < _teamToCheck.size(); i++)
	{
		// Check to see if current character on the team is alive
		if (!_teamToCheck[i]->getComponent<Character>()->IsDead())
		{
			// That person was alive so return true, we just need to make sure that at least one player is still alive
			return true;
		}
	}

	// Everyone on the team is dead
	return false;
}
