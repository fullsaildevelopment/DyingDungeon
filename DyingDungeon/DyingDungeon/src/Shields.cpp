#include "Shields.h"
#include "Character.h"

Shields::Shields(float amountOfAffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Shield;
	mAmountOfEffect = amountOfAffect;
	mDuration = duration;
	mRecipient = target;
}

Shields::~Shields()
{
	mRecipient = nullptr;
}

void Shields::Apply(Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Shields>(mAmountOfEffect, mDuration, &target);
	target.AddStatusEffect(newStatusEffect);
	std::cout << target.GetName() << " has been shielded for " << mAmountOfEffect << " temp HP." << std::endl;
	return;
}

void Shields::Remove()
{
	std::cout << mRecipient->GetName() << " has lost a shield!" << std::endl;
	return;
}

void Shields::Use()
{
	return;
}
