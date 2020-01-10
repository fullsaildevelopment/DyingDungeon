#pragma once
#include "Skills.h"
#include <memory>

class Attack : public Skills
{
	// Variables
private:
	float mDamage;
	float mHealing;
	std::shared_ptr<StatusEffect> mDebuff;
	// Functions
public:
	// Constructors, Destructors
	Attack() = default;
	~Attack();
	Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage);
	Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, float healing);
	Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff);
	Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, bool AOE);
	Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff, bool AOE);
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

