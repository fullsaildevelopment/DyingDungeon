#include "BattleInstance.h"
#include "RedAudioManager.h"
#include "Transform.h"
#include "Character.h"
#include "StatusEvents.h"
#include <string>

BattleInstance::BattleInstance(EntityList _playerTeam, EntityList _enemyTeam, std::vector<Odyssey::Text2D*> _turnOrderNumbers, std::shared_ptr<Odyssey::Entity> _turnIndicatorModel)
{
	mPlayerTeam = _playerTeam;
	mEnemyTeam = _enemyTeam;
	mTurnOrderNumbers = _turnOrderNumbers;
	mTurnIndicator = _turnIndicatorModel;

	// Resize the vectors to be 4 so we can check for nullptr in our TakeTurn functions
	// This will help for determining if a slot is even available to attack
	mPlayerTeam.resize(4);
	mEnemyTeam.resize(4);

	// Make a turn order index to keep track of the index for both of the player and the enemy for loop.
	int turnOrderIndex = 0;

	// Add all of the characters from the player's team to the allCharacters vector
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		if (mPlayerTeam[i] != nullptr)
		{
			// Set the player's turn order number
			mPlayerTeam[i]->getComponent<Character>()->pTurnNumber = mTurnOrderNumbers[turnOrderIndex];
			turnOrderIndex++;

			if (mPlayerTeam[i]->getComponent<Character>()->GetState() != STATE::DEAD)
			{
				// Play an attack animation at the beginning of each battle
				mPlayerTeam[i]->getComponent<Odyssey::Animator>()->playClip("AttackUp");
				// Put him into the mAllCharacters list
				mAllCharacters.push_back(mPlayerTeam[i]);
			}
		}
	}

	// Add all of the characters from the enemy's team to the allCharacters vector
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		if (mEnemyTeam[i] != nullptr)
		{
			// Set the player's turn order number
			mEnemyTeam[i]->getComponent<Character>()->pTurnNumber = mTurnOrderNumbers[turnOrderIndex];
			turnOrderIndex++;

			// Set all of the healths for each player on the enemy team back to 100 and their dead status to false
			// This will show a sim of entering a new battle
			mEnemyTeam[i]->getComponent<Character>()->SetHP(1000);
			mEnemyTeam[i]->getComponent<Character>()->SetMana(1000);
			mEnemyTeam[i]->getComponent<Character>()->SetState(STATE::NONE);
			mEnemyTeam[i]->getComponent<Character>()->ClearStatusEffects();
			mEnemyTeam[i]->getComponent<Odyssey::Animator>()->playClip("Idle");

			mAllCharacters.push_back(mEnemyTeam[i]);
		}
	}

	// Set time to be random
	srand(static_cast<int>(time(NULL)));

	// Create the battle queue before going to battle
	GenerateBattleQueue();
	// Update the turn numbers for each character
	UpdateCharacterTurnNumbers();
	// Set the circle to the current player's location
	SetTurnIndicatorPosition();

	// Set the current round to round 1 at the start
	mCurrentRound = 1;
	std::cout << "--------\nRound - " << GetCurrentRound() << "\n--------\n" << std::endl;
	// No turns have been completed yet
	mTurnCounter = 0;
}

int BattleInstance::UpdateBattle()
{
	// Check to see if the current charcter is even alive before the character takes its turn
	if (mCurrentCharacter->getComponent<Character>()->GetState() == STATE::DEAD)
	{
		std::cout << mCurrentCharacter->getComponent<Character>()->GetName() << " Died, R.I.P.\n" << std::endl;
		//Update the character's turn number to an X - this will represent that he is dead
		mCurrentCharacter->getComponent<Character>()->pTurnNumber->setText(L"X");
		// Take the current character out of the battle queue
		mBattleQueue.pop();
		// Update turn numbers
		UpdateCharacterTurnNumbers();
		// Update Turn Indicator	
		SetTurnIndicatorPosition();
	}
	// Check to see if both teams have at least one character still alive
	else if (IsTeamAlive(mPlayerTeam) && IsTeamAlive(mEnemyTeam))
	{
		// Has the current player taken it's turn yet
		if (mCurrentCharacter->getComponent<Character>()->TakeTurn(mPlayerTeam, mEnemyTeam))
		{
			// One turn has been taken
			mTurnCounter++;
			// Take the current character out of the front of the line
			mBattleQueue.pop();
			// Put the current character to back into the queue, he will go to the back of the line
			mBattleQueue.emplace(mCurrentCharacter);
			//Update the turn numbers
			UpdateCharacterTurnNumbers();
			// Reset the current turn indicator
			SetTurnIndicatorPosition();

			// Has everyone taken their turn in the round?
			if (mTurnCounter == mBattleQueue.size())
			{
				// Increase the round
				mCurrentRound++;
				std::cout << "--------\nRound - " << GetCurrentRound() << "\n--------\n" << std::endl;
				// Set the turn counter for this new round back to zero
				mTurnCounter = 0;
			}
		}
	}
	else
	{
		// The current battle has ended, at least one team is completely dead
		std::cout << "The battle has ended!\n" << std::endl;

		// Check again to see if it was the player's team that died
		if (!IsTeamAlive(mPlayerTeam))
		{
			return PLAYER_TEAM_DIED;
		}

		//Return -1 to Destory the BattleInstance object when the battle is over
		return DESTORY;
	}

	// Return 1 if there is no change in state and the battle needs to continue
	return CONTINUE;
}

void BattleInstance::GenerateBattleQueue()
{
	// This will hold all of the characters that will be in the match
	EntityList characterPool = mAllCharacters;

	// Get the beginning count of the character pool
	int numOfCharacters = static_cast<int>(characterPool.size());

	// Setting Battle Order from highest speed to lowest speed
	for (int i = 0; i < numOfCharacters; i++)
	{
		std::shared_ptr<Odyssey::Entity> highestSpeedCharacter;
		int indexOfCharacter = -1;
		float highestSpeed = -1.0f;
	
		for (int j = 0; j < characterPool.size(); j++)
		{
			if (characterPool[j]->getComponent<Character>()->GetSpeed() > highestSpeed)
			{
				// Set the new highest speed
				highestSpeed = characterPool[j]->getComponent<Character>()->GetSpeed();
				// Set the Character with the current highest speed
				highestSpeedCharacter = characterPool[j];
				// Set the index of the character with the highest speed
				indexOfCharacter = j;
			}
		}
	
		// Add the character into the battle queue
		mBattleQueue.push(highestSpeedCharacter);
	
		// Remove the character from the character pool so he won't be added to the battle queue multiple times
		characterPool.erase(characterPool.begin() + indexOfCharacter);
	}

	// Set the current character after the queue has been created
	mCurrentCharacter = mBattleQueue.front();
}

bool BattleInstance::IsTeamAlive(EntityList _teamToCheck)
{
	for (int i = 0; i < _teamToCheck.size(); i++)
	{
		if (_teamToCheck[i] != nullptr)
		{
			// Check to see if current character on the team is alive
			if (_teamToCheck[i]->getComponent<Character>()->GetState() != STATE::DEAD)
			{
				// That person was alive so return true, we just need to make sure that at least one player is still alive
				return true;
			}
		}
	}

	// Everyone on the team is dead
	return false;
}

void BattleInstance::UpdateCharacterTurnNumbers()
{
	EntityQueue tempBattleQueue = mBattleQueue;
	int startingNum = tempBattleQueue.size();
	int counter = 1;

	//Loop through each character and update it's
	for (int i = 0; i < startingNum; i++)
	{
		Character* currChar = tempBattleQueue.front()->getComponent<Character>();

		if (currChar->GetState() == STATE::DEAD)
		{
			currChar->pTurnNumber->setText(L"X");
			counter--;
		}
		else
		{
			currChar->pTurnNumber->setText(std::to_wstring(counter));
		}
		tempBattleQueue.pop();
		counter++;
	}
}

void BattleInstance::SetTurnIndicatorPosition()
{
	// The placement of the turn indicator should always be underneath the player who is in the front of the queue
	mCurrentCharacter = mBattleQueue.front();

	// Get the character's position
	DirectX::XMFLOAT3 characterPosition = mCurrentCharacter->getComponent<Odyssey::Transform>()->getPosition();
	
	// Set the turn indicator's position based on the character's position
	mTurnIndicator->getComponent<Odyssey::Transform>()->setPosition(characterPosition.x, characterPosition.y + 0.05f, characterPosition.z);
	mTurnIndicator->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);

	// Send out event letting the stat tracker know a new player is taking a turn
	std::string characterName = mCurrentCharacter->getComponent<Character>()->GetName();
	bool isHero = mCurrentCharacter->getComponent<Character>()->IsHero();
	Odyssey::EventManager::getInstance().publish(new TurnStartEvent(characterName, mTurnCounter, mCurrentRound, isHero));
}
