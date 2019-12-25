#include "Bleed.h"
#include "Character.h"

Bleed::Bleed(float ammountOfEffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Bleed;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
}

Bleed::~Bleed()
{
	mRecipient = nullptr;
}
void Bleed::Apply(Character& target)
{
	StatusEffect* newStatusEffect = nullptr;
	newStatusEffect = new Bleed(mAmountOfEffect, mDuration, &target);
	target.AddStatusEffect(newStatusEffect);
	std::cout << target.GetName() << " has been inflicted with bleed!" << std::endl;
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
	totalBleed -= totalBleed * mRecipient->GetDef();
	std::cout << mRecipient->GetName() << " has bleed for " << totalBleed << "HP!" <<std::endl;
}
