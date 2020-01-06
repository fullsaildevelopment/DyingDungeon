#pragma once
#include "StatusEffect.h"

class Regens : public StatusEffect
{
	// Variables
public:
private:

	// Functions
public:
	// Constructors, Destructors
	Regens() = default;
	Regens(float ammountOfEffect, int duration, Character* target);
	~Regens();
	// Application functions
	void Apply(Character& target);
	void Remove();
	void Use();
private:

};