#include "Attack.h"
#include "Character.h"

Attack::Attack(SKILLTYPE id, std::string skillName, std::string animationId, float mpCost, float damage, StatusEffect* debuff)
{
	mTypeId = id;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mDamage = damage;
	mDebuff = debuff;
}

void Attack::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	target.TakeDamage(mDamage);
	if (mDebuff != nullptr)
	{
		mDebuff->Apply();
	}
}
float Attack::GetDamage()
{
	return mDamage;
}
StatusEffect* Attack::GetStatusEffect()
{
	return mDebuff;
}


