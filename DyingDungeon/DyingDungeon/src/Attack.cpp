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
	//TODO Fix aoe mana issue move into the controlers 
	caster.DepleteMana(mMpCost);
	float totalDps = 0.0f;
	totalDps = mDamage + (mDamage * caster.GetAtk());
	std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << " for ";
	target.TakeDamage(totalDps);
	if (mStatusEffect != nullptr)
	{
		mStatusEffect->Apply(target);
		Odyssey::EventManager::getInstance().publish(new CharacterDealtDamageEvent(caster.GetName(), mName, mDamage, caster.GetAtk(), mStatusEffect->GetTypeId()));
	}
	else
		Odyssey::EventManager::getInstance().publish(new CharacterDealtDamageEvent(caster.GetName(), mName, mDamage, caster.GetAtk(), EFFECTTYPE::None));
	if (mHealing > 0.0f)
		caster.ReceiveHealing(mHealing);
	Odyssey::EventManager::getInstance().publish(new CharacterTakeDamage(target.GetName(), mName, target.GetDef()));
}

float Attack::GetDamage()
{
	return mDamage;
}



