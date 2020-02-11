#include "Clense.h"
#include "Character.h"

Clense::Clense(int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Clense;
	mAmountOfEffect = 0.0f;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
	mStatId = STATS::None;
}

Clense::~Clense()
{
	Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
	mStatId = STATS::None;
}

void Clense::Apply(Character& caster, Character& target)
{
	target.ClearBadStatusEffects();
}

void Clense::Remove()
{
}

void Clense::Use()
{
}

STATS Clense::GetStatId()
{
	return STATS();
}
