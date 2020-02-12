#include "Attack.h"
#include "Character.h"

Attack::Attack(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float damage)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::ATTACK;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffectChance = 1.0f;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Attack::Attack(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float damage, float healing)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::ATTACK;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = healing;
	mStatusEffectChance = 1.0f;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Attack::Attack(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::ATTACK;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffectChance = 1.0f;
	mStatusEffect = debuff;
	mIsAOE = false;
}

Attack::Attack(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float damage, bool AOE)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::ATTACK;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffectChance = 1.0f;
	mStatusEffect = nullptr;
	mIsAOE = AOE;
}

Attack::Attack(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff, bool AOE)
{
	mSkillTypeId = GameplayTypes::SKILLTYPE::ATTACK;
	mSkillName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffectChance = 1.0f;
	mStatusEffect = debuff;
	mIsAOE = AOE;
}

Attack::~Attack()
{
}

void Attack::Use(Character& caster, Character& target)
{
	float totalDps = 0.0f;
	totalDps = mDamage + (mDamage * caster.GetAtkMod());
	//UI battle log
	std::wstring battleText = caster.GetName();
	std::wstring skillName = mSkillName;
	std::wstring targetName = target.GetName();
	battleText.append(L" used " + skillName + L" on " + targetName + L" for");
	GameUIManager::getInstance().UpdateCombatLogIcons(&caster, &target, this);
	target.TakeDamage(totalDps);
	if (mStatusEffect != nullptr && target.GetState() != STATE::DEAD && RandomChance() <= mStatusEffectChance)
	{
		mStatusEffect->Apply(caster, target);
		Odyssey::EventManager::getInstance().publish(new CharacterDealtDamageEvent(caster.GetName(), &caster, mSkillName, mDamage, caster.GetAtkMod(), mStatusEffect->GetTypeId()));
    
		//Switch stament for reds evvents
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
		case EFFECTTYPE::Provoke:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), &caster, target.GetName(), &target, mSkillName, EFFECTTYPE::Provoke, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		default:
			break;
		}
	}
	else
		Odyssey::EventManager::getInstance().publish(new CharacterDealtDamageEvent(caster.GetName(), &caster, mSkillName, mDamage, caster.GetAtkMod(), EFFECTTYPE::None));
	Odyssey::EventManager::getInstance().publish(new CharacterTakeDamage(target.GetName(), &target, mSkillName, target.GetDefMod()));
  
	if (mHealing > 0.0f)
		caster.ReceiveHealing(mHealing);
}

float Attack::GetDamage()
{
	return mDamage;
}



