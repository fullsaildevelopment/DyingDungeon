#pragma once
#include "StatusEffect.h"

class StatUp : public StatusEffect
{
	// Variables
public:
private:
	STATS mStatId;
	// Functions
public:
	// Constructors, Destructors
	StatUp() = default;
	StatUp(float ammountOfEffect, int duration, STATS stat, Character* target);
	~StatUp();
	// Application functions
	void Apply(Character& caster, Character & target);
	void Remove();
	void Use();
	// Getter for Stat id
	STATS GetStatId();
private:

 };