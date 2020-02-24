#include "Shields.h"
#include "Character.h"
#include "CharacterHUDElements.h"

Shields::Shields(float amountOfAffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Shield;
	mAmountOfEffect = amountOfAffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
	mEffectIconName = L"assets/images/StatusEffects/Shield.png";
}

Shields::~Shields()
{
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = 0.0f;
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
	GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetShieldBuff()->setVisible(true);
	return;
}

void Shields::Remove()
{
	if(mRecipient->IsShielded())
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetShieldBuff()->setVisible(false);
	return;
}

void Shields::Use()
{
	return;
}
