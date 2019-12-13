#include "Buffs.h"
#include "Character.h"


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
	if (mGameObject != nullptr)
	{
		Character* tempC = nullptr;
		tempC = mGameObject->getComponent<Character>();
		switch (mEffectedStat)
		{
		case HP:
		{
			if (mBleed == false)
			{
				mAmountOfEffect *= tempC->GetHP();
				tempC->SetHP(tempC->GetHP() + mAmountOfEffect);
				std::cout << "Dab the buff is gone" << std::endl;
			}
			break;
		}
		default:
			break;
		}
	}
}
void Buffs::initialize()
{
	Character* tempC = nullptr;
	tempC = mGameObject->getComponent<Character>();
	switch (mEffectedStat)
	{
	case HP:
	{
		mAmountOfEffect *= tempC->GetHP();
		tempC->TakeDamage(mAmountOfEffect);
		std::cout << "Fuck you tristan" << std::endl;
		break;
	}
	default:
		break;
	}
}
void Buffs::Bleed()
{
	Character* tempC = nullptr;
	tempC = mGameObject->getComponent<Character>();
	switch (mEffectedStat)
	{
	case HP:
	{
		mAmountOfEffect *= tempC->GetHP();
		tempC->TakeDamage(mAmountOfEffect);
		std::cout << "Bleed" << std::endl;
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