#include "Stun.h"
#include "Character.h"

Stun::Stun(int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Stun;
	mAmountOfEffect = 0.0f;
	mDuration = duration;
	mRecipient = target;
}
Stun::~Stun()
{
	mRecipient = nullptr;
}
void Stun::Apply(Character& target)
{
	if (target.GetState() != STATE::STUNNED)
	{
		std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
		newStatusEffect = std::make_shared<Stun>(mDuration, &target);
		target.AddStatusEffect(newStatusEffect);
		target.SetState(STATE::STUNNED);
		std::cout << target.GetName() << " has been stunned!" << std::endl;
	}
	else
		std::cout << target.GetName() << " is already stunned." << std::endl;
	return;
}
void Stun::Use()
{
	mRecipient->SetState(STATE::STUNNED);
	return;
}
void Stun::Remove()
{
	mRecipient->SetState(STATE::NONE);
	std::cout << mRecipient->GetName() << " is no longer stunned." << std::endl;
	return;
}
