#include "Buffs.h"
#include "Character.h"

// Constructors for the diffrent cases of attacks //
//////////////////////////////////////////////////////////////
Buffs::Buffs(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff)
{
	if (isBuff)
		mSkillTypeId = GameplayTypes::SKILLTYPE::BUFF;
	else
		mSkillTypeId = GameplayTypes::SKILLTYPE::DEBUFF;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mStatusEffect = buff;
	mStatusEffectChance = 1.0f;
	mIsAOE = false;
}
Buffs::Buffs(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff, bool isAOE)
{
	if (isBuff)
		mSkillTypeId = GameplayTypes::SKILLTYPE::BUFF;
	else
		mSkillTypeId = GameplayTypes::SKILLTYPE::DEBUFF;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mStatusEffect = buff;
	mStatusEffectChance = 1.0f;
	mIsAOE = isAOE;
}
//////////////////////////////////////////////////////////////


// Function that applies the attack to the passed in target
void Buffs::Use(Character& caster, Character& target)
{
	// Update combat log
	GameUIManager::getInstance().UpdateCombatLogIcons(&caster, &target, this);
	GameUIManager::getInstance().UpdateCombatLogText(mStatusEffect->GetAmountOfEffect());

	// If i have a status effect to apply, apply it
	if (mStatusEffect != nullptr)
	{
		mStatusEffect->Apply(caster, target);

		// Play audio "heal" sound effect
		RedAudioManager::Instance().PlaySFX("GokuSJJ3");

		//Alert Reds stuff for stat tracking
		switch (mStatusEffect->GetTypeId())
		{
		case EFFECTTYPE::None:
		{
			break;
		}
		case EFFECTTYPE::Bleed:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::Bleed, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Regen:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), &caster, mSkillName, EFFECTTYPE::Regen, (mStatusEffect->GetAmountOfEffect() * mStatusEffect->GetDuration())));
			break;
		}
		case EFFECTTYPE::StatUp:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::StatUp, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::StatDown:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::StatDown, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Stun:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::Stun, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Shield:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::Shield, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Provoke:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::Provoke, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		default:
			break;
		}
	}
}

