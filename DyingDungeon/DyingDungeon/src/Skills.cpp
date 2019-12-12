#include "Skills.h"
#include "Character.h"

Skills::Skills(float dps, float mana)
{
	mDamage = dps;
	mMpCost = mana;
	mBuff = nullptr;
}
Skills::Skills(float dps, float mana,Buffs* buff)
{
	mDamage = dps;
	mMpCost = mana;
	mBuff = buff;
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
    // Does the caster have enough mana
	if (caster.GetMana() < mMpCost)
		return;
	// Deal damage and deduct MP
    caster.DepleteMana(mMpCost);
	target.TakeDamage(mDamage);
	// Attach buff/debuff using the skills stored buff/debuff
	if (mBuff != nullptr)
		target.getGameObject()->addComponent<Buffs>(mBuff->GetEffectedStat(),mBuff->GetAmountOfEffect(),mBuff->GetDuration(),mBuff->IsBleed());
	// Put info to console
	std::cout << caster.GetName() << " attacked " << target.GetName() << " for " << mDamage << std::endl;
	std::cout << target.GetName() << " now has " << target.GetHP() << "HP\n" << std::endl;
	// If target is dead kill him
	if (target.GetHP() <= 0)
	{
		target.Die();
	}
}

Buffs* Skills::GetBuff()
{
	return mBuff;
}
