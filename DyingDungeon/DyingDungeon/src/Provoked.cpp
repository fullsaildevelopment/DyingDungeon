#include "Provoked.h"
#include "Character.h"

Provoked::Provoked(int duration, Character* provoker, Character* target)
{
	mTypeId = EFFECTTYPE::Provoke;
	mAmountOfEffect = 0.0f;
	mDuration = duration;
	mRecipient = target;
	mProvoker = provoker;
	mAffectedStatId = STATS::None;
}

Provoked::~Provoked()
{
	mRecipient = nullptr;
	mProvoker = nullptr;
}

void Provoked::Apply(Character& caster, Character& target)
{
	target.SetProvoked(mProvoker);
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Provoked>(mDuration, mProvoker, &target);
	target.AddStatusEffect(newStatusEffect);
	caster.AddCastedEffect(newStatusEffect.get());
}

void Provoked::Remove()
{
	if (mRecipient->GetProvoked() == mProvoker)
	{
		mRecipient->SetProvoked(nullptr);
	}
}

void Provoked::Use()
{
}

Character* Provoked::GetProvoker()
{
	return mProvoker;
}

void Provoked::SetProvoker(Character* provoker)
{
	mProvoker = provoker;
}
