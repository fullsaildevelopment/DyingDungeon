#pragma once
#include "Skills.h"
#include <memory>

class Attack : public Skills
{
	// Variables
private:
	float mDamage;
	std::shared_ptr<StatusEffect> mDebuff;
	std::shared_ptr<Skills> mAdditionalEffect;
	// Functions
public:
	// Constructors, Destructors
	Attack() = default;
	~Attack();
	Attack(std::string skillName, std::string animationId, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff, bool AOE);
	Attack(std::string skillName, std::string animationId, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff, bool AOE, std::shared_ptr<Skills> AdditionalEffect);
	// Use the skill
	void Use(Character& caster, Character& target);
	// Get how much damage the attack does
	float GetDamage();
	// Get what debuff this applys
	StatusEffect* GetStatusEffect();
	// Set the status Effect this applies
	void SetStatusEffect(std::shared_ptr<StatusEffect> se);
private:
};

