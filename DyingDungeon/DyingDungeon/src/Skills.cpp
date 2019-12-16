#include "Skills.h"
#include "Character.h"

Skills::Skills(float dps, float mana,std::string skillName)
{
	mDamage = dps;
	mMpCost = mana;
	mBuff = Buffs(STATS::NONE, -1.0f, 0, false, false);
	mName = skillName;
}
Skills::Skills(float dps, float mana,Buffs buff, std::string skillName)
{
	mDamage = dps;
	mMpCost = mana;
	mBuff = buff;
	mName = skillName;
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
	float totalDps = mDamage + (mDamage * caster.GetAtk());
	target.TakeDamage(totalDps);
	// Attach buff/debuff using the skills stored buff/debuff
	if (mBuff.GetEffectedStat() != STATS::NONE)
	{
		Buffs* newBuff = new Buffs(mBuff.GetEffectedStat(), mBuff.GetAmountOfEffect(), mBuff.GetDuration(), mBuff.IsBleed(), &target);
		newBuff->InitalEffect();
		target.AddStatusEffect(newBuff);
	}
	// Put info to console
	std::cout << caster.GetName() << " attacked " << target.GetName() << " for " << mDamage << std::endl;
	std::cout << target.GetName() << " now has " << target.GetHP() << "HP\n" << std::endl;
	// If target is dead kill him
	if (target.GetHP() <= 0)
	{
		target.Die();
	}
}
Buffs Skills::GetBuff()
{
	return mBuff;
}

// Get skill name 
std::string Skills::GetName()
{
	return mName;
}
