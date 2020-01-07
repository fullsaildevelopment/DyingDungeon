#pragma once
#include "Skills.h"

class Attack : public Skills
{
	// Variables
public:
private:
	float mDamage;
	StatusEffect* mDebuff;
	Skills* mAdditionalEffect;
	// Functions
public:
	// Constructors, Destructors
	Attack() = default;
	~Attack();
	Attack(std::string skillName, std::string animationId, float mpCost, float damage, StatusEffect* debuff, bool AOE);
	Attack(std::string skillName, std::string animationId, float mpCost, float mDamage, StatusEffect* debuff, bool AOE, Skills* AdditionalEffect);
	// Use the skill
	void Use(Character& caster, Character& target);
	// Get how much damage the attack does
	float GetDamage();
	// Get what debuff this applys
	StatusEffect* GetStatusEffect();
	// Set the status Effect this applies
	void SetStatusEffect(StatusEffect* se);
private:
};

