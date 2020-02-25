#include "StatDown.h"
#include "Character.h"

StatDown::StatDown(float ammountOfEffect, int duration, STATS stat ,Character* target)
{
	mTypeId = EFFECTTYPE::StatDown;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = stat;
	mStatId = stat;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		mEffectIconName = L"assets/images/StatusEffects/AttackDown.png";
		break;
	}
	case STATS::Def:
	{
		mEffectIconName = L"assets/images/StatusEffects/DefenseDown.png";
		break;
	}
	case STATS::Spd:
	{
		mEffectIconName = L"assets/images/StatusEffects/SpeedDown.png";
		break;
	}
	default:
		break;
	}
}

StatDown::~StatDown()
{
	if(mRecipient != nullptr)
		Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
	mStatId = STATS::None;
}

void StatDown::Apply(Character& caster, Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<StatDown>(mAmountOfEffect, mDuration, mStatId, &target);
	if (!target.AddStatusEffect(newStatusEffect))
		return;
	std::string temp;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		target.DecreaseAtk(mAmountOfEffect);
		temp = " Attack";
		break;
	}
	case STATS::Def:
	{
		target.DecreaseDef(mAmountOfEffect);
		temp = " Defense";
		break;
	}
	case STATS::Spd:
	{
		target.DecreaseSpd(mAmountOfEffect);
		temp = " Speed";
		break;
	}
	default:
		break;
	}
	caster.AddCastedEffect(newStatusEffect.get());
	return;
}

void StatDown::Remove()
{
	std::string temp;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		mRecipient->IncreaseAtk(mAmountOfEffect);
		temp = " Attack";
		break;
	}
	case STATS::Def:
	{
		mRecipient->IncreaseDef(mAmountOfEffect);
		temp = " Defense";
		break;
	}
	case STATS::Spd:
	{
		mRecipient->IncreaseSpd(mAmountOfEffect);
		temp = " Speed";
		break;
	}
	default:
		break;
	}
	return;
}

void StatDown::Use()
{
	return;
}

STATS StatDown::GetStatId()
{
	return mStatId;
}
