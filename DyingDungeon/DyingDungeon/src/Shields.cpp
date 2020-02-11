#include "Shields.h"
#include "Character.h"

Shields::Shields(float amountOfAffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Shield;
	mAmountOfEffect = amountOfAffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
}

Shields::~Shields()
{
	Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
}

void Shields::Apply(Character& caster, Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Shields>(mAmountOfEffect, mDuration, &target);
	target.AddStatusEffect(newStatusEffect);
	caster.AddCastedEffect(newStatusEffect.get());
	return;
}

void Shields::Remove()
{
	return;
}

void Shields::Use()
{
	return;
}
