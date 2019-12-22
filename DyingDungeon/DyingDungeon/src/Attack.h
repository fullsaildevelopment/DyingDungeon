#pragma once
#include "Skills.h"

class Attack : public Skills
{
	// Variables
public:
private:
	float mDamage;
	StatusEffect* mDebuff;
	// Functions
public:
	void Use(Character& caster, Character& target);
	float GetDamage();
	StatusEffect GetStatusEffect();
private:
};

