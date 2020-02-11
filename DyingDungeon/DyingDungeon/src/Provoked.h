#pragma once
#include "StatusEffect.h"

class Provoked : public StatusEffect
{
	// Variables
public:
private:
	Character* mProvoker;
	// Functions
public:
	// Constructors, Destructors
	Provoked() = default;
	Provoked(int duration, Character* provoker, Character* target);
	~Provoked();
	// Application functions
	void Apply(Character& caster, Character& target);
	void Remove();
	void Use();
	// mProvoker functions
private:
	Character* GetProvoker();
	void SetProvoker(Character* provoker);

};

