#pragma once
#include "OdysseyEngine.h"
#include "RenderDevice.h"
#include "Scene.h"


class CharacterFactory
{
public: // Constructors
	CharacterFactory() = default;
	Odyssey::RenderDevice* mRenderRefrence;
	enum CharacterOptions
	{
		Paladin,
		Mage,
		Skeleton,
		Ganfaul
	};

public: // Functions

	std::shared_ptr<Odyssey::Entity> CreateCharacter(CharacterOptions _characterToCreate, std::string _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, std::shared_ptr<Odyssey::Scene> _gameScene);

	//Getters

	//Setters

private: // Varibales

	// Scene
	std::shared_ptr<Odyssey::Scene> mGameScene;

	// Vectors

	// Queues

	// Entitys

	// Ints

	// Floats

	// Bools


private: // Functions
	void CreateCharacterImpactIndicator(std::shared_ptr<Odyssey::Entity> _character);
	Odyssey::ParticleSystem* setUpFireButBetter();
	Odyssey::ParticleSystem* setUpFireStorm();
	Odyssey::ParticleSystem* setupBlood();
};

