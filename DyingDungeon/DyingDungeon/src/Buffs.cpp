#include "Buffs.h"
#include "Character.h"
#include "GameObject.h"

CLASS_DEFINITION(Component, Buffs)

Buffs::Buffs(int effectedStat, float effect, int duration, bool isBleed)
{
	mAmountOfEffect = effect;
	mDuration = duration;
	mEffectedStat = effectedStat;
	mBleed = isBleed;
}
Buffs::~Buffs()
{
	Character* tempC = nullptr;
	tempC = this->getGameObject()->getComponent<Character>();
	switch (mEffectedStat)
	{
	case HP:
	{
		if (mBleed == false)
		{
			mAmountOfEffect *= tempC->GetHP();
			tempC->SetHP(tempC->GetHP() + mAmountOfEffect);
		}
		break;
	}
	default:
		break;
	}
}
void Buffs::initialize(Odyssey::GameObject* parent)
{
	Character* tempC = nullptr;
	tempC = parent->getComponent<Character>();
	switch (mEffectedStat)
	{
	case HP:
	{
		mAmountOfEffect *= tempC->GetHP();
		tempC->SetHP(tempC->GetHP() + mAmountOfEffect);
		break;
	}
	default:
		break;
	}
}
void Buffs::Bleed()
{
	Character* tempC = nullptr;
	tempC = this->getGameObject()->getComponent<Character>();
	switch (mEffectedStat)
	{
	case HP:
	{
		mAmountOfEffect *= tempC->GetHP();
		tempC->SetHP(tempC->GetHP() + mAmountOfEffect);
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