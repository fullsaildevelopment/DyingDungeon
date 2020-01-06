#pragma once
#include "Entity.h"


class CharacterFactory
{
public: // Constructors
	CharacterFactory() = default;

	enum CharacterOptions
	{
		Paladin,
		Skeleton
	};

public: // Functions

	std::shared_ptr<Odyssey::Entity> CreateCharacter(CharacterOptions _characterToCreate, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation);

	//Getters

	//Setters

private: // Varibales

	// Vectors

	// Queues

	// Entitys

	// Ints

	// Floats

	// Bools


private: // Functions
};

