#pragma once
#include "Skills.h"


class Buffs : public Skills
{
public:
private:
	StatusEffect* mBuff;
public:
	// Constructors
	Buffs() = default;
	Buffs(std::string skillName, std::string animationId, float mpCost, StatusEffect* buff);
	// Use the skill 
	void Use(Character& caster, Character& target);
	// Get what kind of buff this applys
	StatusEffect* GetStatusEffect();
private:
};

