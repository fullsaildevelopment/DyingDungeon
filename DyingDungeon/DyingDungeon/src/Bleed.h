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
	Bleed(float ammountOfEffect, int duration, Character* target);
	~Bleed();
	// Application functions
	void Apply(Character& caster, Character& target);
	void Remove();
	void Use();
private:

};