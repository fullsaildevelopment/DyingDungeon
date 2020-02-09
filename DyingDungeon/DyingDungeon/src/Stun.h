#pragma once
#include "StatusEffect.h"

class Stun : public StatusEffect
{
	// Variables
public:
private:

	// Functions
public:
	// Constructors, Destructors
	Stun() = default;
	Stun(int duration, Character * target);
	~Stun();
	// Application functions
	void Apply(Character& caster, Character & target);
	void Remove();
	void Use();
private:

};

