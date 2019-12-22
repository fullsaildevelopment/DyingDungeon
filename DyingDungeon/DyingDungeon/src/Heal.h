#pragma once
#include "Skills.h"

class Heal : public Skills
{
	// Variables
public:
private:
	float mHealing;
	// Functions
public:
	void Use(Character& caster, Character& target);
	float GetHealing();
private:
};

