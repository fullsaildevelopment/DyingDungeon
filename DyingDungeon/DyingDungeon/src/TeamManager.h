#pragma once
#include <vector>
#include "Entity.h"


class TeamManager
{
public:
	enum class HeroType
	{
		Paladin, Mage
	};

	enum class EnemyType
	{
		Skeleton, Ganfaul
	};

public: // Singleton pattern
	/**
	 *	Get the singleton instance of the game ui manager.
	 *	@param[in] void
	 *	@return InputManager& The singleton instance of the game ui manager.
	 */
	static TeamManager& getInstance();
	~TeamManager() { }
private: // Singleton pattern
	TeamManager() { }

public: // Functions

	// Add player to the player team
	void AddCharacterToPlayerTeam(std::shared_ptr<Odyssey::Entity> _characterToAdd);
	// Add player to the enemy team
	void AddCharacterToEnemyTeam(std::shared_ptr<Odyssey::Entity> _characterToAdd);

	// Clear the players from the player team
	void ClearPlayerTeam();

	// Getters
	// Get the players that were created
	std::vector<std::shared_ptr<Odyssey::Entity>> GetPlayerTeam() { return mPlayerTeam; }
	// Get the enemies that were created
	std::vector<std::shared_ptr<Odyssey::Entity>> GetEnemyTeam() { return mEnemyTeam; }

	// Get the enemy team enums to determine what charcters we need to create
	std::vector<TeamManager::EnemyType> GetEnemyTeamToCreate() { return mEnemyTeamToCreate; }

	// Setters

private: // Variables

	// Holds the enum types of the characters we need to create for the enemy team
	std::vector<TeamManager::EnemyType> mEnemyTeamToCreate;

	// Holds the player characters to add into the Tower Manager
	std::vector<std::shared_ptr<Odyssey::Entity>> mPlayerTeam;
	// Holds the enemy characters to add into the Tower Manager
	std::vector<std::shared_ptr<Odyssey::Entity>> mEnemyTeam;

private: // Functions

};

