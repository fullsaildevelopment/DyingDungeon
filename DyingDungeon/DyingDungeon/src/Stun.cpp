#include "Stun.h"
#include "Character.h"

Stun::Stun(int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Stun;
	mAmountOfEffect = 0.0f;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
}
Stun::~Stun()
{
	Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
}
void Stun::Apply(Character& caster, Character& target)
{
	if (target.GetState() != STATE::STUNNED)
	{
		std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
		newStatusEffect = std::make_shared<Stun>(mDuration, &target);
		target.AddStatusEffect(newStatusEffect);
		caster.AddCastedEffect(newStatusEffect.get());
		target.SetState(STATE::STUNNED);
	}
	return;
}
void Stun::Use()
{
	mRecipient->SetState(STATE::STUNNED);
	return;
}
void Stun::Remove()
{
	if (mRecipient->GetState() != STATE::DEAD)
	{
		mRecipient->SetState(STATE::NONE);
	}
	return;
}
