#include "Buffs.h"
#include "Character.h"

Buffs::Buffs(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff)
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
Buffs::Buffs(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff, bool isAOE)
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

// Applies buff to target
void Buffs::Use(Character& caster, Character& target)
{
	if (mStatusEffect != nullptr)
	{
		mStatusEffect->Apply(target);
		//Alert Reds stuff for stat tracking?
		switch (mStatusEffect->GetTypeId())
		{
		case EFFECTTYPE::None:
		{
			break;
		}
		case EFFECTTYPE::Bleed:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::Bleed, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Regen:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), mName, EFFECTTYPE::Regen, (mStatusEffect->GetAmountOfEffect() * mStatusEffect->GetDuration())));
			break;
		}
		case EFFECTTYPE::StatUp:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::StatUp, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::StatDown:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::StatDown, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Stun:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::Stun, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Shield:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::Shield, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Provoke:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::Provoke, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		default:
			break;
		}
	}
}

