#pragma once
#include "OdysseyEngine.h"
#include "RenderManager.h"
#include "Scene.h"
#include "Application.h"
#include <map>


class CharacterFactory
{
public:
	// Singleton pattern
		/**
		 *	Get the singleton instance of the game ui manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the game ui manager.
		 */
	static CharacterFactory& getInstance();
	~CharacterFactory() { }
private: // Singleton pattern
	CharacterFactory() {}

public:
	enum CharacterOptions
	{
		Paladin,
		Mage,
		Bard,
		Warrior,
		Monk,
		Skeleton,
		Summoner,
		MeleeDemon,
		CasterDemon,
		Ganfaul
	};

	enum CharacterType
	{
		Hero,
		Enemy
	};

public: // Functions

	void initialize(Odyssey::Application* _application);

	Odyssey::Entity* CreateCharacter(CharacterOptions _characterToCreate, std::wstring _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, DirectX::XMFLOAT2 _hudPosition, bool _showHUD, DirectX::XMFLOAT2 _hpPopupPosition, Odyssey::Scene* _gameScene = nullptr);

	//Getters
	Odyssey::Entity* GetCharacterPrefab(CharacterOptions _characterType);
	Odyssey::Entity* GetHUDPrefab(bool isHero);
	Odyssey::Entity* GetImpactIndicatorPrefab();
	Odyssey::Entity* GetTurnIndicatorPrefab();

	//Setters

private: // Varibales

	// Scene

	// Vectors

	// Queues

	// Entitys

	// Ints
	unsigned int characterHudIndex = 0;

	// Floats

	// Bools


private: // Functions
	// Create character prefabs
	Odyssey::Entity* CreateCharacterPrefab(CharacterOptions _characterToCreate);

	// Create hero hud
	Odyssey::Entity* CreateHeroHudPrefab();

	// Create enemy hud
	Odyssey::Entity* CreateEnemyHudPrefab();

	// Create impact indicator
	Odyssey::Entity* CreateImpactIndicatorPrefab();

	// Create turn indicator
	Odyssey::Entity* CreateTurnIndicatorPrefab();

	// Particle Creation Functions
	Odyssey::ParticleSystem* setUpFireButBetter(Odyssey::Scene* _sceneToAddTo);
	Odyssey::ParticleSystem* setUpFireStorm(Odyssey::Scene* _sceneToAddTo);
	Odyssey::ParticleSystem* setupBlood(Odyssey::Scene* _sceneToAddTo);

	// Character Map
	std::map<CharacterOptions, Odyssey::Entity*> mCharacterPrefabMap;

	// HUD Map
	std::map<CharacterType, Odyssey::Entity*> mHudPrefabMap;

	// Impact Indicator
	Odyssey::Entity* mImpactIndicatorPrefab;

	// Turn Indicator
	Odyssey::Entity* mTurnIndicatorPrefab;

	// Application
	Odyssey::Application* mApplication;
};

