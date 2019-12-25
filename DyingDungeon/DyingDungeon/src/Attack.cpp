#include "Attack.h"
#include "Character.h"

Attack::Attack(std::string skillName, std::string animationId, float mpCost, float damage, StatusEffect* debuff)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mDamage = damage;
	mDebuff = debuff;
}

Attack::~Attack()
{
	if (mDebuff != nullptr)
	{
		delete mDebuff;
		mDebuff = nullptr;
	}
}

void Attack::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	target.TakeDamage(mDamage);
	std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << " for " << mDamage << " damage!" << std::endl;
	if (mDebuff != nullptr)
		mDebuff->Apply(target);
}
float Attack::GetDamage()
{
	return mDamage;
}
StatusEffect* Attack::GetStatusEffect()
{
	return mDebuff;
}

void Attack::SetStatusEffect(StatusEffect* se)
{
	mDebuff = se;
}


