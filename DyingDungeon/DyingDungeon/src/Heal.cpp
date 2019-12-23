#include "Heal.h"
#include "Character.h"
// Constructor
Heal::Heal(SKILLTYPE id, std::string skillName, std::string animationId, float mpCost, float healing)
{
	mTypeId = id;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mHealing = healing;
}

// Use the heal on the target
void Heal::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	target.ReceiveHealing(mHealing);
}
// Get the amount the heal is for
float Heal::GetHealing()
{
	return mHealing;
}
