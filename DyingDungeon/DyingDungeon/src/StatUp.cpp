#include "StatUp.h"
#include "Character.h"
#include "CharacterHUDElements.h"

StatUp::StatUp(float ammountOfEffect, int duration, STATS stat, Character* target)
{
	mTypeId = EFFECTTYPE::StatUp;
	mAmountOfEffect = ammountOfEffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = stat;
	mStatId = stat;
	mRemoveMe = false;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		mEffectIconName = L"assets/images/StatusEffects/AttackUp.png";
		break;
	}
	case STATS::Def:
	{
		mEffectIconName = L"assets/images/StatusEffects/DefenseUp.png";
		break;
	}
	case STATS::Spd:
	{
		mEffectIconName = L"assets/images/StatusEffects/SpeedUp.png";
		break;
	}
	default:
		break;
	}
}

StatUp::~StatUp()
{
	if (mRecipient != nullptr && mRemoveMe == false)
		Remove();
	mTypeId = EFFECTTYPE::None;
	mAmountOfEffect = -1.0f;
	mDuration = -1;
	mRecipient = nullptr;
	mAffectedStatId = STATS::None;
	mStatId = STATS::None;
}

void StatUp::Apply(Character& caster,Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<StatUp>(mAmountOfEffect, mDuration,mStatId, &target);
	target.AddStatusEffect(newStatusEffect, &caster);
	caster.AddCastedEffect(newStatusEffect);
	return;
}

void StatUp::Remove()
{
	mRemoveMe = true;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetAttackUpBuff()->setVisible(false);
		mRecipient->DecreaseAtk(mAmountOfEffect);
		break;
	}
	case STATS::Def:
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetDefenseUpBuff()->setVisible(false);
		mRecipient->DecreaseDef(mAmountOfEffect);
		break;
	}
	case STATS::Spd:
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSpeedUpBuff()->setVisible(false);
		mRecipient->DecreaseSpd(mAmountOfEffect);
		break;
	}
	default:
		break;
	}
	return;
}

void StatUp::Use()
{
	return;
}

STATS StatUp::GetStatId()
{
	return mStatId;
}
