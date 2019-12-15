#include "Buffs.h"
#include "Character.h"

CLASS_DEFINITION(Component, Buffs)

Buffs::Buffs(int effectedStat, float effect, int duration, bool isBleed, bool alive)
{
	mAmountOfEffect = effect;
	mDuration = duration;
	mEffectedStat = effectedStat;
	mBleed = isBleed;
	setEnabled(alive);
}
Buffs::~Buffs()
{
}
void Buffs::initialize()
{
	InitalEffect();
}
void Buffs::onEnable()
{
	InitalEffect();
}
void Buffs::InitalEffect()
{
	switch (mEffectedStat)
	{
	case Buffs::HP:
	{
		std::cout << "Bleed on" << std::endl;
		break;
	}
	case Buffs::MP:
	{
		break;
	}
	default:
		break;
	}
}
void Buffs::RevertEffect()
{
	switch (mEffectedStat)
	{
	case Buffs::HP:
	{
		break;
	}
	case Buffs::MP:
	{
		break;
	}
	default:
		break;
	}
}
void Buffs::Bleed()
{
	Character* tempC = mGameObject->getComponent<Character>();
	switch (mEffectedStat)
	{
	case Buffs::HP:
	{
		tempC->TakeDamage(mAmountOfEffect * tempC->GetMaxHP());
		std::cout << tempC->GetName() << " Took " << mAmountOfEffect * tempC->GetMaxHP() << "Damage in bleeding"  << tempC->GetName() << " now has " << tempC->GetHP() << "HP\n" << std::endl;
		break;
	}
	case Buffs::MP:
	{
		break;
	}
	default:
		break;
	}
}
int Buffs::GetDuration()
{
	return mDuration;
}
void Buffs::SetDuration(int newDuration)
{
	mDuration = newDuration;
}
void Buffs::ReduceDuration(int deduction)
{
	mDuration -= deduction;
}
bool Buffs::IsBleed()
{
	return mBleed;
}
int Buffs::GetEffectedStat()
{
	return mEffectedStat;
}
void Buffs::SetEffectedStat(int statEffected)
{
	mEffectedStat = statEffected;
}
float Buffs::GetAmountOfEffect()
{
	return mAmountOfEffect;
}