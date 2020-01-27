#include "Heal.h"
#include "Character.h"
// Constructor
Heal::Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing)
{
	mTypeId = SKILLTYPE::HEAL;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Heal::Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing, bool isAoe)
{
	mTypeId = SKILLTYPE::HEAL;
	mName = skillName;
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
	std::cout << caster.GetName() << " has healed " << target.GetName() << " for " << mHealing << " HP." << std::endl;
	Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), mName, EFFECTTYPE::None, mHealing));
}

// Get the amount the heal is for
float Heal::GetHealing()
{
	return mHealing;
}
