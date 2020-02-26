#include "Stun.h"
#include "Character.h"
#include "CharacterHUDElements.h"

Stun::Stun(int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Stun;
	mAmountOfEffect = 0.0f;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
	mEffectIconName = L"assets/images/StatusEffects/Stun.png";
}
Stun::~Stun()
{
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
}
void Stun::Apply(Character& caster, Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Stun>(mDuration, &target);
	target.AddStatusEffect(newStatusEffect, &caster);
	return;
}
void Stun::Use()
{
	return;
}
void Stun::Remove()
{
	return;
}
