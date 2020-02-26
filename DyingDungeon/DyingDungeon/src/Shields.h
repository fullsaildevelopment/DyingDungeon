#pragma once
#include "StatusEffect.h"

class Shields : public StatusEffect
{
	// Variables
public:
private:

	// Functions
public:
	// Constructors, Destructors
	Shields() = default;
	Shields(float amountOfAffect, int duration, Character * target);
	~Shields();
	// Application functions
	void Apply(Character& caster, Character & target);
	void Remove();
	void Use();
private:

};

