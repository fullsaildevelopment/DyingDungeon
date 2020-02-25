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
	if(mRecipient != nullptr)
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
	if (!target.AddStatusEffect(newStatusEffect))
		return;
	std::string temp;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetAttackUpBuff()->setVisible(true);
		target.IncreaseAtk(mAmountOfEffect);
		temp = " Attack";
		break;
	}
	case STATS::Def:
	{
		GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetDefenseUpBuff()->setVisible(true);
		target.IncreaseDef(mAmountOfEffect);
		temp = " Defense";
		break;
	}
	case STATS::Spd:
	{
		GameUIManager::getInstance().GetCharacterHuds()[target.GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSpeedUpBuff()->setVisible(true);
		target.IncreaseSpd(mAmountOfEffect);
		temp = " Speed";
		break;
	}
	default:
		break;
	}
	caster.AddCastedEffect(newStatusEffect.get());
	return;
}

void StatUp::Remove()
{
	std::string temp;
	switch (mStatId)
	{
	case STATS::Atk:
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetAttackUpBuff()->setVisible(false);
		mRecipient->DecreaseAtk(mAmountOfEffect);
		temp = " Attack";
		break;
	}
	case STATS::Def:
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetDefenseUpBuff()->setVisible(false);
		mRecipient->DecreaseDef(mAmountOfEffect);
		temp = " Defense";
		break;
	}
	case STATS::Spd:
	{
		GameUIManager::getInstance().GetCharacterHuds()[mRecipient->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSpeedUpBuff()->setVisible(false);
		mRecipient->DecreaseSpd(mAmountOfEffect);
		temp = " Speed";
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
