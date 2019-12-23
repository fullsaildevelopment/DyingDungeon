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
	// Constructors, Destructors
	Attack() = default;
	~Attack() = default;
	Attack(SKILLTYPE id, std::string skillName, std::string animationId, float mpCost, float mDamage, StatusEffect* debuff);
	// Use the skill
	void Use(Character& caster, Character& target);
	// Get how much damage the attack does
	float GetDamage();
	// Get what debuff this applys
	StatusEffect* GetStatusEffect();
private:
};

