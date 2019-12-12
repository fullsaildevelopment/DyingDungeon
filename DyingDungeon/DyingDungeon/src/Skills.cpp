#include "Skills.h"
#include "Character.h"

Skills::Skills(float dps, float mana)
{
	mDamage = dps;
	mMpCost = mana;
}
Skills::~Skills()
{
}
float Skills::GetManaCost()
{
	return mMpCost;
}
void Skills::Use(Character& caster,Character& target)
{
	caster.SetMana(caster.GetMana() - mMpCost);
	target.SetHP(target.GetHP() - mDamage);

	std::cout << caster.GetName() << " attacked " << target.GetName() << " for " << mDamage << std::endl;
	std::cout << target.GetName() << " now has " << target.GetHP() << "HP\n" << std::endl;

	if (target.GetHP() <= 0);
		//target.die();
}
