#include "StatUp.h"
#include "Character.h"

StatUp::StatUp(float ammountOfEffect, int duration, STATS stat, Character* target)
{
	mTypeId = EFFECTTYPE::StatUp;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mStatId = stat;
}

StatUp::~StatUp()
{
}

void StatUp::Apply(Character& target)
{
	StatusEffect* newStatusEffect = nullptr;
	newStatusEffect = new StatUp(mAmountOfEffect, mDuration,mStatId, &target);
	target.AddStatusEffect(newStatusEffect);
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
	std::cout << target.GetName() << " has received a " << mAmountOfEffect << "%" << temp << " up!"<< std::endl;
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
	std::cout << mRecipient->GetName() << " has lost it's " << mAmountOfEffect << "%" << temp << " up!" << std::endl;
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
