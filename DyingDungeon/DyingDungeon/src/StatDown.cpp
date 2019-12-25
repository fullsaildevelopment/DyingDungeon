#include "StatDown.h"
#include "Character.h"

StatDown::StatDown(float ammountOfEffect, int duration, STATS stat ,Character* target)
{
	mTypeId = EFFECTTYPE::StatDown;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mStatId = stat;
}

StatDown::~StatDown()
{
}

void StatDown::Apply(Character& target)
{
	StatusEffect* newStatusEffect = nullptr;
	newStatusEffect = new StatDown(mAmountOfEffect, mDuration, mStatId, &target);
	target.AddStatusEffect(newStatusEffect);
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
		target.DecreaseDef(mAmountOfEffect);
		temp = " Speed";
		break;
	}
	default:
		break;
	}
	std::cout << target.GetName() << " has received a " << mAmountOfEffect << "%" << temp << " down!" << std::endl;
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
	std::cout << mRecipient->GetName() << " has lost it's " << mAmountOfEffect << "%" << temp << " down!" << std::endl;
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
