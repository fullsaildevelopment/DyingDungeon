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
	// Get the player team enums to determine what charcters we need to create
	std::vector<HeroType> GetPlayerTeamToCreate() { return mPlayerTeamToCreate; }
	// Get the enemy team enums to determine what charcters we need to create
	std::vector<EnemyType> GetEnemyTeamToCreate() { return mEnemyTeamToCreate; }

	// Setters

private: // Variables

	// Holds the enum types of the characters we need to create for the player team
	std::vector<HeroType> mPlayerTeamToCreate;
	// Holds the enum types of the characters we need to create for the enemy team
	std::vector<EnemyType> mEnemyTeamToCreate;

private: // Functions

};

