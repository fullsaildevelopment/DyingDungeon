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
	std::shared_ptr<StatusEffect> newStatusEffect;
	newStatusEffect = std::make_shared<Bleed>(mAmountOfEffect, mDuration, &target);
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
	float totalBleedButBetter = totalBleed -  totalBleed * mRecipient->GetDef();
	std::string tempName = mRecipient->GetName();
	mRecipient->TakeDamage(totalBleed);
	std::cout << tempName << " has bleed for " << totalBleedButBetter << "HP!" <<std::endl;
}
