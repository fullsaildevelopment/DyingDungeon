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

/*
 * Function:  GetManaCost()
 * --------------------
 * Gets the mana cost of the skill/ability
 *
 * returns: void
 */
float Skills::GetManaCost()
{
	return mMpCost;
}

/*
 * Function:  Use()
 * --------------------
 * Caster will attack the target with dealing damage
 * and any status effects will be applied.
 *
 * returns: void
 */
void Skills::Use(Character& caster,Character& target)
{
    //Does the caster have enough mana
	if (caster.GetMana() < mMpCost)
		return;

    caster.DepleteMana(mMpCost);
	target.TakeDamage(mDamage);

	std::cout << caster.GetName() << " attacked " << target.GetName() << " for " << mDamage << std::endl;
	std::cout << target.GetName() << " now has " << target.GetHP() << "HP\n" << std::endl;

	if (target.GetHP() <= 0)
	{
		//target.die();
	}
}
