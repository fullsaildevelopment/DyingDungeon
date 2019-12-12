#include "Buffs.h"
#include "Character.h"
#include "GameObject.h"

CLASS_DEFINITION(Component, Buffs)

Buffs::Buffs(int effectedStat, float effect, int duration)
{
	mStatToAffect = nullptr;
	mAmountOfEffect = effect;
	mDuration = duration;
	mEffectedStat = effectedStat;
}
Buffs::~Buffs()
{
	*mStatToAffect -= mAmountOfEffect;
}
void Buffs::initialize(Odyssey::GameObject* parent)
{
	switch (mEffectedStat)
	{
	case 0:
	{
		//mStatToAffect = parent->getComponent<Character>()->GetHP();
	}
	default:
		break;
	}
}