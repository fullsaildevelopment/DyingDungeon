#include "StatusEffect.h"

EFFECTTYPE StatusEffect::GetTypeId()
{
	return mTypeId;
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

Character* StatusEffect::GetRecipient()
{
	return mRecipient;
}

void StatusEffect::SetRecipient(Character* recipient)
{
	mRecipient = recipient;
}
