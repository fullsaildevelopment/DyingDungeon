#pragma once
#include <vector>
#include "Entity.h"


class TeamManager
{
public:
	enum class HeroType
	{
		Paladin, Mage, Bard, Warrior
	};

	enum class EnemyType
	{
		Skeleton, Ganfaul
	};

	struct EnemySetups
	{
		EnemyType pEnemyType;
		DirectX::XMVECTOR pPosition;
		DirectX::XMVECTOR pRotation;
		DirectX::XMFLOAT2 pHudPosition;
		DirectX::XMFLOAT2 pHpPopupPosition;
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

	// Create Enemy Team based on the index passed in
	void CreateEnemyTeam(int _index);

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

	// Setters
	void SetTheListOfEnemiesToCreate(std::vector<TeamManager::EnemySetups> _enemyEnums) { mEnemiesToCreate.push_back(_enemyEnums); }

private: // Variables

	// Hold the multiple list of enums that will need to be created;
	std::vector<std::vector<TeamManager::EnemySetups>> mEnemiesToCreate;

	// Holds the player characters to add into the Tower Manager
	std::vector<std::shared_ptr<Odyssey::Entity>> mPlayerTeam;
	// Holds the enemy characters to add into the Tower Manager
	std::vector<std::shared_ptr<Odyssey::Entity>> mEnemyTeam;

private: // Functions
	
};

