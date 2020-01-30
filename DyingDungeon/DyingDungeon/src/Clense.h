#pragma once
#include "StatusEffect.h"

class Clense : public StatusEffect
{	
	// Variables
public:
private:
	STATS mStatId;
	// Functions
public:
	// Constructors, Destructors
	Clense() = default;
	Clense(int duration, Character* target);
	~Clense();
	// Application functions
	void Apply(Character& target);
	void Remove();
	void Use();
	// Getter for Stat id
	STATS GetStatId();
private:

};

