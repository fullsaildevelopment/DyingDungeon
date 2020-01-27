#include "Attack.h"
#include "Character.h"

Attack::Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Attack::Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, float healing)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = healing;
	mStatusEffect = nullptr;
	mIsAOE = false;
}

Attack::Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffect = debuff;
	mIsAOE = false;
}

Attack::Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, bool AOE)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffect = nullptr;
	mIsAOE = AOE;
}

Attack::Attack(std::string skillName, std::string animationId, float animationTiming, float mpCost, float damage, std::shared_ptr<StatusEffect> debuff, bool AOE)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mAnimationTime = animationTiming;
	mMpCost = mpCost;
	mDamage = damage;
	mHealing = 0.0f;
	mStatusEffect = debuff;
	mIsAOE = AOE;
}

Attack::~Attack()
{
}

void Attack::Use(Character& caster, Character& target)
{
	float totalDps = 0.0f;
	totalDps = mDamage + (mDamage * caster.GetAtk());
	std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << " for ";
	//UI battle log
	std::string casterName = caster.GetName();
	std::string targetNameS = target.GetName();
	std::wstring battleText = std::wstring(casterName.begin(), casterName.end());
	std::wstring skillName = std::wstring(mName.begin(), mName.end());
	std::wstring targetName = std::wstring(targetNameS.begin(), targetNameS.end());
	battleText.append(L" used " + skillName + L" on " + targetName + L" for");
	GameUIManager::getInstance().SetBattleLogText(battleText, false);

	target.TakeDamage(totalDps);
	if (mStatusEffect != nullptr)
	{
		mStatusEffect->Apply(target);
		Odyssey::EventManager::getInstance().publish(new CharacterDealtDamageEvent(caster.GetName(), mName, mDamage, caster.GetAtk(), mStatusEffect->GetTypeId()));
		//Switch stament for reds evvents
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
		case EFFECTTYPE::Provoke:
		{
			Odyssey::EventManager::getInstance().publish(new CharacterDebuffsEvent(caster.GetName(), target.GetName(), mName, EFFECTTYPE::Provoke, mStatusEffect->GetAmountOfEffect()));
			break;
		}
		default:
			break;
		}
	}
	else
		Odyssey::EventManager::getInstance().publish(new CharacterDealtDamageEvent(caster.GetName(), mName, mDamage, caster.GetAtk(), EFFECTTYPE::None));
	Odyssey::EventManager::getInstance().publish(new CharacterTakeDamage(target.GetName(), mName, target.GetDef()));
	if (mHealing > 0.0f)
		caster.ReceiveHealing(mHealing);
}

float Attack::GetDamage()
{
	return mDamage;
}



