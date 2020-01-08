#pragma once
#include "Skills.h"
#include <memory>

class Buffs : public Skills
{
public:
private:
	std::shared_ptr<StatusEffect> mBuff;
public:
	// Constructors
	Buffs() = default;
	Buffs(std::string skillName, std::string animationId, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff, bool isAOE);
	// Use the skill 
	void Use(Character& caster, Character& target);
	// Get what kind of buff this applys
	StatusEffect* GetStatusEffect();
private:
};

