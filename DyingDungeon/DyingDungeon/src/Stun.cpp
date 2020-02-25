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
	if(mRecipient != nullptr)
		Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
}
void Stun::Apply(Character& caster, Character& target)
{
	if (target.GetState() != STATE::STUNNED)
	{
		GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetStunBuff()->setVisible(true);
		std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
		newStatusEffect = std::make_shared<Stun>(mDuration, &target);
		target.AddStatusEffect(newStatusEffect);
		caster.AddCastedEffect(newStatusEffect.get());
		target.SetState(STATE::STUNNED);
	}
	return;
}
void Stun::Use()
{
	mRecipient->SetState(STATE::STUNNED);
	return;
}
void Stun::Remove()
{
	if (mRecipient->GetState() != STATE::DEAD)
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetStunBuff()->setVisible(false);
		mRecipient->SetState(STATE::NONE);
	}
	return;
}
