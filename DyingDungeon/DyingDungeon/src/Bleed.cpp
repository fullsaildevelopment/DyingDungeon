#include "Bleed.h"
#include "Character.h"

Bleed::Bleed(float ammountOfEffect, int duration, Character* target)
{
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
}

Bleed::~Bleed()
{
	mRecipient = nullptr;
}
void Bleed::Apply(Character* target)
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
