#include "Attack.h"
#include "Character.h"
// See if theres a better way than this 
#include "Bleed.h"

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
	if (mDebuff != nullptr)
	{
		StatusEffect* newStatusEffect = nullptr;
		switch (mDebuff->GetTypeId())
		{
		case EFFECTTYPE::Bleed:
		{
			newStatusEffect = new Bleed(mDebuff->GetAmountOfEffect(), mDebuff->GetDuration(), &target);
			break;
		}
		default:
			break;
		}
		target.AddStatusEffect(newStatusEffect);
		newStatusEffect->Apply();
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

void Attack::SetStatusEffect(StatusEffect* se)
{
	mDebuff = se;
}


