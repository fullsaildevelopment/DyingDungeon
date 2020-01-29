#pragma once
#include <vector>


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

	// Getters
	// Get the enemy team enums to determine what charcters we need to create
	std::vector<TeamManager::EnemyType> GetEnemyTeamToCreate() { return mEnemyTeamToCreate; }

	// Setters

private: // Variables

	// Holds the enum types of the characters we need to create for the enemy team
	std::vector<TeamManager::EnemyType> mEnemyTeamToCreate;

private: // Functions

};

