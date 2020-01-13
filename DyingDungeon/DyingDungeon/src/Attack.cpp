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
	mDebuff = nullptr;
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
	mDebuff = nullptr;
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
	mDebuff = debuff;
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
	mDebuff = nullptr;
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
	mDebuff = debuff;
	mIsAOE = AOE;
}

Attack::~Attack()
{
}


void Attack::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	float totalDps = 0.0f;
	totalDps = mDamage + (mDamage * caster.GetAtk());
	std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << " for ";
	target.TakeDamage(totalDps);
	if (mDebuff != nullptr)
		mDebuff->Apply(target);
	if (mHealing > 0.0f)
		caster.ReceiveHealing(mHealing);
}
float Attack::GetDamage()
{
	return mDamage;
}
//StatusEffect* Attack::GetStatusEffect()
//{
//	return mDebuff.get();
//}
//
//void Attack::SetStatusEffect(std::shared_ptr<StatusEffect> se)
//{
//	mDebuff = se;
//}


