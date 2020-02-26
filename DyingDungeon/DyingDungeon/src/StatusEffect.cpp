#include "StatusEffect.h"

EFFECTTYPE StatusEffect::GetTypeId()
{
	return mTypeId;
}

std::wstring StatusEffect::GetIcon()
{
	return mEffectIconName;
}

void StatusEffect::SetIcon(std::wstring newIcon)
{
	mEffectIconName = newIcon;
}

STATS StatusEffect::GetAffectedStatId()
{
	return mAffectedStatId;
}

int StatusEffect::GetDuration()
{
	return mDuration;
}

void StatusEffect::SetDuration(int newDuration)
{
	mDuration = newDuration;
}

void StatusEffect::ReduceDuration(int deduction)
{
	mDuration -= deduction;
}

float StatusEffect::GetAmountOfEffect()
{
	return mAmountOfEffect;
}

void StatusEffect::SetAmountOfEffect(float newAmount)
{
	mAmountOfEffect = newAmount;
}

Character* StatusEffect::GetRecipient()
{
	return mRecipient;
}

void StatusEffect::SetRecipient(Character* recipient)
{
	mRecipient = recipient;
}

bool StatusEffect::RemoveMe()
{
	return mRemoveMe;
}


