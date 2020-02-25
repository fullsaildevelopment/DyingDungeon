#include "Bleed.h"
#include "Character.h"
#include "CharacterHUDElements.h"

Bleed::Bleed(float ammountOfEffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Bleed;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
	mEffectIconName = L"assets/images/StatusEffects/Bleed.png";
}

Bleed::~Bleed()
{
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
}
void Bleed::Apply(Character& caster, Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect;
	newStatusEffect = std::make_shared<Bleed>(mAmountOfEffect, mDuration, &target);
	target.AddStatusEffect(newStatusEffect);
	GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetBleedBuff()->setVisible(true);
	caster.AddCastedEffect(newStatusEffect.get());
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
	mRecipient->TakeDamage(totalBleed);
}
