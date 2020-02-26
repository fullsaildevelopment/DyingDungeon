#include "Regens.h"
#include "Character.h"
#include "CharacterHUDElements.h"

Regens::Regens(float ammountOfEffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Regen;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
	mEffectIconName = L"assets/images/StatusEffects/Regen.png";
}
Regens::~Regens()
{
	mTypeId = EFFECTTYPE::Regen;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
}
void Regens::Apply(Character& caster, Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Regens>(mAmountOfEffect, mDuration, &target);
	target.AddStatusEffect(newStatusEffect, &caster);
	return;
}
void Regens::Remove()
{
	return;
}
void Regens::Use()
{
	float totalRegen = 0.0f;
	totalRegen = mAmountOfEffect * mRecipient->GetMaxHP();
	mRecipient->ReceiveHealing(mAmountOfEffect);
}