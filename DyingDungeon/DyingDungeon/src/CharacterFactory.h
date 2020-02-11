#pragma once
#include "OdysseyEngine.h"
#include "RenderManager.h"
#include "Scene.h"


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
	CharacterFactory() { }

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

public: // Functions

	Odyssey::Entity* CreateCharacter(CharacterOptions _characterToCreate, std::wstring _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, DirectX::XMFLOAT2 _hudPosition, bool _showHUD, DirectX::XMFLOAT2 _hpPopupPosition, Odyssey::Scene* _gameScene);

	//Getters

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
	void CreateCharacterImpactIndicator(Odyssey::Entity* _character, Odyssey::Scene* _sceneToAddTo);

	// Particle Creation Functions
	Odyssey::ParticleSystem* setUpFireButBetter(Odyssey::Scene* _sceneToAddTo);
	Odyssey::ParticleSystem* setUpFireStorm(Odyssey::Scene* _sceneToAddTo);
	Odyssey::ParticleSystem* setupBlood(Odyssey::Scene* _sceneToAddTo);
};

