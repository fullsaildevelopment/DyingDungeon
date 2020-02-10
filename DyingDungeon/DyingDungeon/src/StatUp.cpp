#include "StatUp.h"
#include "Character.h"

StatUp::StatUp(float ammountOfEffect, int duration, STATS stat, Character* target)
{
	mTypeId = EFFECTTYPE::StatUp;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = stat;
	mStatId = stat;
}

StatUp::~StatUp()
{
}

void StatUp::Apply(Character& caster,Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<StatUp>(mAmountOfEffect, mDuration,mStatId, &target);
	if (!target.AddStatusEffect(newStatusEffect))
		return;
	std::string temp;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		target.IncreaseAtk(mAmountOfEffect);
		temp = " Attack";
		break;
	}
	case STATS::Def:
	{
		target.IncreaseDef(mAmountOfEffect);
		temp = " Defense";
		break;
	}
	case STATS::Spd:
	{
		target.IncreaseSpd(mAmountOfEffect);
		temp = " Speed";
		break;
	}
	default:
		break;
	}
	caster.AddCastedEffect(newStatusEffect.get());
	return;
}

void StatUp::Remove()
{
	std::string temp;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		mRecipient->DecreaseAtk(mAmountOfEffect);
		temp = " Attack";
		break;
	}
	case STATS::Def:
	{
		mRecipient->DecreaseDef(mAmountOfEffect);
		temp = " Defense";
		break;
	}
	case STATS::Spd:
	{
		mRecipient->DecreaseSpd(mAmountOfEffect);
		temp = " Speed";
		break;
	}
	default:
		break;
	}
	return;
}

void StatUp::Use()
{
	return;
}

STATS StatUp::GetStatId()
{
	return mStatId;
}
