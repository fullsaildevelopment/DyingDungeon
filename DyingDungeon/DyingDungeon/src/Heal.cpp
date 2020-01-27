#include "Heal.h"
#include "Character.h"
// Constructor
Heal::Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing)
{
	mSkillTypeId = SKILLTYPE::HEAL;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Heal::Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing, bool isAoe)
{
	mSkillTypeId = SKILLTYPE::HEAL;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mStatusEffect = nullptr;
	mIsAOE = isAoe;
}

// Use the heal on the target
void Heal::Use(Character& caster, Character& target)
{
	target.ReceiveHealing(mHealing);
	Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), mSkillName, EFFECTTYPE::None, mHealing));
}

// Get the amount the heal is for
float Heal::GetHealing()
{
	return mHealing;
}
