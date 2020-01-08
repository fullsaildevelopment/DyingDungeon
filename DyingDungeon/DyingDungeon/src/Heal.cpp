#include "Heal.h"
#include "Character.h"
// Constructor
Heal::Heal(std::string skillName, std::string animationId, float animationTiming, float mpCost, float healing)
{
	mTypeId = SKILLTYPE::HEAL;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mIsAOE = false;
}

Heal::Heal(std::string skillName, std::string animationId, float animationTiming, float mpCost, float healing, bool isAoe)
{
	mTypeId = SKILLTYPE::HEAL;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mIsAOE = isAoe;
}

// Use the heal on the target
void Heal::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	target.ReceiveHealing(mHealing);
	std::cout << caster.GetName() << " has healed " << target.GetName() << " for " << mHealing << " HP." << std::endl;
}
// Get the amount the heal is for
float Heal::GetHealing()
{
	return mHealing;
}
