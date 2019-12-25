#pragma once
#include "StatusEffect.h"

class StatDown : public StatusEffect
{
	// Variables
public:
private:
	STATS mStatId;
	// Functions
public:
	// Constructors, Destructors
	StatDown() = default;
	StatDown(float ammountOfEffect, int duration, STATS stat, Character * target);
	~StatDown();
	// Application functions
	void Apply(Character & target);
	void Remove();
	void Use();
	// Getter for Stat id
	STATS GetStatId();
private:

};