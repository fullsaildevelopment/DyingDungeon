#pragma once
#include "StatusEffect.h"

class Bleed : public StatusEffect
{
	// Variables
public:
private:
	// Functions
public:
	// Constructors, Destructors
	Bleed() = default;
	Bleed(EFFECTTYPE id ,float ammountOfEffect, int duration, Character* target);
	~Bleed();
	// Application functions
	void Apply();
	void Remove();
	void Use();
private:

};