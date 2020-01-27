#pragma once
#include "OdysseyEngine.h"
#include "RenderDevice.h"
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
	Odyssey::RenderDevice* mRenderRefrence;
	enum CharacterOptions
	{
		Paladin,
		Mage,
		Bard,
		Skeleton,
		Ganfaul
	};

public: // Functions

	std::shared_ptr<Odyssey::Entity> CreateCharacter(CharacterOptions _characterToCreate, std::string _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, DirectX::XMFLOAT2 _hudPosition, bool _showHUD, std::shared_ptr<Odyssey::Scene> _gameScene);

	//Getters

	//Setters

private: // Varibales

	// Scene
	std::shared_ptr<Odyssey::Scene> mCurrentScene;

	// Vectors

	// Queues

	// Entitys

	// Ints
	int characterHudIndex = 0;

	// Floats

	// Bools


private: // Functions
	void CreateCharacterImpactIndicator(std::shared_ptr<Odyssey::Entity> _character);
	Odyssey::ParticleSystem* setUpFireButBetter();
	Odyssey::ParticleSystem* setUpFireStorm();
	Odyssey::ParticleSystem* setupBlood();
};

