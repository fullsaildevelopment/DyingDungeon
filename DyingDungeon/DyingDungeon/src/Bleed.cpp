#include "Bleed.h"
#include "Character.h"

Bleed::Bleed(EFFECTTYPE id ,float ammountOfEffect, int duration, Character* target)
{
	mTypeId = id;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
}

Bleed::~Bleed()
{
	mRecipient = nullptr;
}
void Bleed::Apply()
{
	return;
}

void Bleed::Remove()
{
	return;
}

void Bleed::Use()
{
	float totalBleed = 0;
	totalBleed = mAmountOfEffect * mRecipient->GetMaxHP();
	mRecipient->TakeDamage(totalBleed);
}
