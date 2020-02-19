#pragma once
#include <vector>
#include "Entity.h"
#include "Scene.h"

class TeamManager
{
public:
	enum class HeroType
	{
		Paladin, 
		Mage, 
		Bard, 
		Warrior, 
		Monk
	};

	enum class EnemyType
	{
		Skeleton,
		Summoner,
		MeleeDemon,
		CasterDemon,
		Ganfaul
	};

	struct EnemySetups
	{
		EnemyType pEnemyType;
		DirectX::XMVECTOR pPosition;
		DirectX::XMVECTOR pRotation;
		DirectX::XMFLOAT2 pHudPosition;
		DirectX::XMFLOAT2 pHpPopupPosition;
		bool pIsBoss;
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
	std::vector<Odyssey::Entity*> CreateEnemyTeam(int _index);
	// Add a list of enemies to index through
	void AddEnemiesListToCreate(std::vector<EnemySetups> _enemyEnums) { mEnemiesToCreate.push_back(_enemyEnums); }

	// Add player to the player team
	void AddCharacterEnumToPlayerTeam(HeroType _characterHeroType);
	// Add player to the enemy team
	void AddCharacterToEnemyTeam(Odyssey::Entity* _characterToAdd);

	// Clear the players from the player team
	void ClearPlayerTeamEnumList();

	// Getters
	// Get the players that were created
	std::vector<HeroType> GetPlayerTeamToCreate() { return mPlayerTeamToCreate; }
	// Get the enemies that were created
	std::vector<Odyssey::Entity*> GetEnemyTeam() { return mEnemyTeam; }

	// Setters
	// Set the current tower
	void SetTheCurrentTower(Odyssey::Entity* _tower) { mCurrentTower = _tower; }
	// Set the first scene
	void SetTheFirstScene(Odyssey::Scene* _scene) { mSceneOne = _scene; }

private: // Variables

	// Hold the multiple list of enums that will need to be created;
	std::vector<std::vector<TeamManager::EnemySetups>> mEnemiesToCreate;

	// Holds the player characters to add into the Tower Manager
	std::vector<HeroType> mPlayerTeamToCreate;
	// Holds the enemy characters to add into the Tower Manager
	std::vector<Odyssey::Entity*> mEnemyTeam;

	// Holds the tower manager entity
	Odyssey::Entity* mCurrentTower;

	// Hold the first scene
	Odyssey::Scene* mSceneOne;

private: // Functions
	
};

