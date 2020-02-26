#include "Provoked.h"
#include "Character.h"
#include "CharacterHUDElements.h"

Provoked::Provoked(int duration, Character* provoker, Character* target)
{
	mTypeId = EFFECTTYPE::Provoke;
	mAmountOfEffect = 0.0f;
	mDuration = duration;
	mRecipient = target;
	mProvoker = provoker;
	mAffectedStatId = STATS::None;
	mEffectIconName = L"assets/images/StatusEffects/Provoke.png";
}

Provoked::~Provoked()
{
	if(mRecipient != nullptr)
		Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mProvoker = nullptr;
	mAffectedStatId = STATS::None;
}

void Provoked::Apply(Character& caster, Character& target)
{
	target.SetProvoked(mProvoker);
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Provoked>(mDuration, mProvoker, &target);
	target.AddStatusEffect(newStatusEffect);
	caster.AddCastedEffect(newStatusEffect.get());		
	GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetProvokeBuff()->setVisible(true);
}

void Provoked::Remove()
{
	if (mRecipient->GetProvoked() == mProvoker)
	{
		mRecipient->SetProvoked(nullptr);
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetProvokeBuff()->setVisible(false);
	}
}

void Provoked::Use()
{
}

Character* Provoked::GetProvoker()
{
	return mProvoker;
}

void Provoked::SetProvoker(Character* provoker)
{
	mProvoker = provoker;
}
