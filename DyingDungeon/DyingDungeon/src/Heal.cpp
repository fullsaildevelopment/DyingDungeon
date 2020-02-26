#include "Heal.h"
#include "Character.h"


// Constructors for the diffrent cases of attacks //
//////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////

// Function that applies the heal to the passed in target
void Heal::Use(Character& caster, Character& target)
{
	// Apply the heal to the target
	target.ReceiveHealing(mHealing);

	// Update the combat log
	GameUIManager::getInstance().UpdateCombatLogIcons(&caster, &target, this);
	GameUIManager::getInstance().SetCombatLogTextColor({ 0.0f, 255.0f, 0.0f });
	GameUIManager::getInstance().UpdateCombatLogText(mStatusEffect->GetAmountOfEffect());

	// Fire off reds event
	Odyssey::EventManager::getInstance().publish(new CharacterHealsEvent(caster.GetName(), &caster, mSkillName, EFFECTTYPE::None, mHealing));

	// Play audio "heal" sound effect
	RedAudioManager::Instance().PlaySFX("Healing"); 

	// If i have a status effect to apply, then apply it
	if (mStatusEffect != nullptr && target.GetState() != STATE::DEAD && RandomChance() <= mStatusEffectChance)
	{
		mStatusEffect->Apply(caster, target);

		//Alert Reds stuff for stat tracking
		switch (mStatusEffect->GetTypeId())
		{
		case EFFECTTYPE::None:
		{
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
		case EFFECTTYPE::Shield:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterBuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::Shield, mStatusEffect->GetAmountOfEffect()));
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
