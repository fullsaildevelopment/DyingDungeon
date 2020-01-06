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
	StatusEffect* newStatusEffect = nullptr;
	newStatusEffect = new Stun(mDuration, &target);
	target.AddStatusEffect(newStatusEffect);
	target.SetStun(true);
	std::cout << target.GetName() << " has been stunned!" << std::endl;
	return;
}
void Stun::Use()
{
	return;
}
void Stun::Remove()
{
	mRecipient->SetStun(false);
	std::cout << mRecipient->GetName() << " is no longer stunned." << std::endl;
	return;
}
