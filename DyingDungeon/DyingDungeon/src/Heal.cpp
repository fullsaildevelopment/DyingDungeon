#include "Heal.h"
#include "Character.h"
// Constructor
Heal::Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::HEAL;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mStatusEffectChance = 1.0f;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Heal::Heal(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float healing, bool isAoe)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::HEAL;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mHealing = healing;
	mStatusEffectChance = 1.0f;
	mStatusEffect = nullptr;
	mIsAOE = isAoe;
}

// Use the heal on the target
void Heal::Use(Character& caster, Character& target)
{
	GameUIManager::getInstance().UpdateCombatLogIcons(&caster, &target, this);
	target.ReceiveHealing(mHealing);
	Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), mSkillName, EFFECTTYPE::None, mHealing));
	if (mStatusEffect != nullptr && target.GetState() != STATE::DEAD && RandomChance() <= mStatusEffectChance)
	{
		mStatusEffect->Apply(caster, target);
		//Alert Reds stuff for stat tracking?
		switch (mStatusEffect->GetTypeId())
		{
		case EFFECTTYPE::None:
		{
			break;
		}
		case EFFECTTYPE::Regen:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), mSkillName, EFFECTTYPE::Regen, (mStatusEffect->GetAmountOfEffect() * mStatusEffect->GetDuration())));
			break;
		}
		case EFFECTTYPE::StatUp:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), target.GetName(), mSkillName, EFFECTTYPE::StatUp, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		case EFFECTTYPE::Shield:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), target.GetName(), mSkillName, EFFECTTYPE::Shield, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		default:
			break;
		}
	}
}

// Get the amount the heal is for
float Heal::GetHealing()
{
	return mHealing;
}
