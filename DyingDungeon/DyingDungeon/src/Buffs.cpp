#include "Buffs.h"
#include "Character.h"

Buffs::Buffs(std::string skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff)
{
	if (isBuff)
		mTypeId = SKILLTYPE::BUFF;
	else
		mTypeId = SKILLTYPE::DEBUFF;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mStatusEffect = buff;
	mIsAOE = false;
}
Buffs::Buffs(std::string skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff, bool isAOE)
{
	if (isBuff)
		mTypeId = SKILLTYPE::BUFF;
	else
		mTypeId = SKILLTYPE::DEBUFF;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mStatusEffect = buff;
	mIsAOE = isAOE;
}
void Buffs::Use(Character& caster, Character& target)
{
	if (mStatusEffect != nullptr)
	{
		mStatusEffect->Apply(target);
		std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << "." << std::endl;
		if (mStatusEffect->GetTypeId() == EFFECTTYPE::Regen)
			Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), mName, EFFECTTYPE::Regen, (mStatusEffect->GetAmountOfEffect() * mStatusEffect->GetDuration())));
		else if (mStatusEffect->GetTypeId() == EFFECTTYPE::Shield)
			Odyssey::EventManager::getInstance().publish(new CharacterShieldsEvent(caster.GetName(), target.GetName(), mName, mStatusEffect->GetAmountOfEffect()));

	}
}

