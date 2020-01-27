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
	Heal(std::wstring skillName, std::string animationId,float animationTiming, float mpCost, float healing);
	Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing, bool isAoe);
	// Applys healing to target
	void Use(Character& caster, Character& target);
	// Get how much the skill heals for
	float GetHealing();
private:
};

