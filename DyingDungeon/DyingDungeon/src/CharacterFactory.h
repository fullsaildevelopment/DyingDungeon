#pragma once
#include "GameObject.h"

enum CharacterOptions
{
	Paladin,
	Skeleton
};

class CharacterFactory
{
public: // Constructors
	CharacterFactory();

public: // Functions

	std::shared_ptr<Odyssey::GameObject> CreateCharacter(CharacterOptions _characterToCreate, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation);

	//Getters

	//Setters

private: // Varibales

	// Vectors

	// Queues

	// GameObjects

	// Ints

	// Floats

	// Bools


private: // Functions
};

