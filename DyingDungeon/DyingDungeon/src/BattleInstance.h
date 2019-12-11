#pragma once
#include "Component.h"
#include <queue>
#include <vector>
#include <time.h>

class BattleInstance : public Odyssey::Component
{
	CLASS_DECLARATION(BattleInstance);

public: // Constructors
	BattleInstance(std::vector<std::shared_ptr<Odyssey::GameObject>> _playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> _enemyTeam);

public: // Functions
	virtual void initialize(Odyssey::GameObject* Parent);
	virtual void update(double deltaTime);

private: // Varibales
	std::vector<std::shared_ptr<Odyssey::GameObject>> playerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	std::vector<std::shared_ptr<Odyssey::GameObject>> enemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team

	std::queue<std::shared_ptr<Odyssey::GameObject>> battleQueue; // Battle Queue that will hodl the order in which players can attack

private: // Functions
	void GenerateBattleQueue(); // This will generate the battle queue for the character turn orders
	bool IsCharacterOnPlayerTeam(std::shared_ptr<Odyssey::GameObject> _characterToCheck); // This will check to see if a character is on the player's team
};

