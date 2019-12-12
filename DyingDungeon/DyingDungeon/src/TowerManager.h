#pragma once
#include "Component.h"
#include <vector>
#include <time.h>

class TowerManager : public Odyssey::Component
{
	CLASS_DECLARATION(TowerManager);

public: // Constructors
	TowerManager() = default;

public: // Functions

	// Necessary
	virtual void initialize(Odyssey::GameObject* _parent);
	virtual void update(double deltaTime);

	//Getters

	//Setters


private: // Varibales

	// Vectors
	std::vector<std::shared_ptr<Odyssey::GameObject>> mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	std::vector<std::shared_ptr<Odyssey::GameObject>> mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	std::vector<std::shared_ptr<Odyssey::GameObject>> mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team

	// Queues

	// GameObjects

	// Ints


	// Floats


	// Bools

	// Battle State

private: // Functions
};

