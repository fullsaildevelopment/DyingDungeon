#include "BattleInstance.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, BattleInstance)

BattleInstance::BattleInstance(std::vector<std::shared_ptr<Odyssey::GameObject>> _playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> _enemyTeam)
{
	playerTeam = _playerTeam;
	enemyTeam = _enemyTeam;
}

void BattleInstance::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	srand(time(NULL));
	GenerateBattleQueue();
}

void BattleInstance::update(double deltaTime)
{
	//Get the next character in line so the character can do it's attack
	std::shared_ptr<Odyssey::GameObject> pCurrentCharacter = battleQueue.front();

	//Take the current character out of the front of the line
	battleQueue.pop();
	//Put the current character to back into the queue, he will go to the back of the line
	battleQueue.emplace(pCurrentCharacter);
}

void BattleInstance::GenerateBattleQueue()
{
	// This will hold all of the characters that will be in the match
	std::vector<std::shared_ptr<Odyssey::GameObject>> characterPool;

	// Add each character from the player's team to the characetr pool 
	for (int i = 0; i < playerTeam.size(); i++)
		characterPool.push_back(playerTeam[i]);

	// Add each character from the enemy's team to the characetr pool 
	for (int i = 0; i < enemyTeam.size(); i++)
		characterPool.push_back(enemyTeam[i]);

	// Get the beginning count of the character pool
	int numOfCharacters = characterPool.size();
	// What? - Loop for the number of beginning amount of characters
	// Why? - Because we will be removing characters from the pool when we add the character to the battle queue 
	for (int i = 0; i < numOfCharacters; i++)
	{
		// Get random index from the character pool
		int rndIndex = rand() % characterPool.size();

		// Add the character into the battle queue
		battleQueue.push(characterPool[rndIndex]);

		// Remove the character from the character pool so he won't be added to the battle queue multiple times
		characterPool.erase(characterPool.begin() + rndIndex);
	}
}