#pragma once
#include "Skills.h"

class Heal : public Skills
{
	// Variables
public:
private:
	//How much healing the skill does
	float mHealing;
	// Functions
public:
	// Constructors
	Heal() = default;
	Heal(SKILLTYPE id,std::string skillName, std::string animationId, float mpCost, float healing);
	// Applys healing to target
	void Use(Character& caster, Character& target);
	// Get how much the skill heals for
	float GetHealing();
private:
};

