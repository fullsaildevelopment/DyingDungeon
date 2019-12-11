#include "Character.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, Character)

void Character::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	SetHP(0);
	SetMana(0);
	SetDead(false);
}

void Character::update(double deltaTime)
{
	
}

bool Character::TakeTurn()
{
	return true;
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the HP of the character
 *
 * returns: A float representing current HP
 */
float Character::GetHP()
{
	return mHP;
}

/*
 * Function:  SetHP(float HP)
 * --------------------
 * Sets the HP of the character to the passed in float
 *
 * returns: nothing
 */
void Character::SetHP(float HP)
{
	mHP = HP;
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the mana of the character
 *
 * returns: A float representing mana
 */
float Character::GetMana()
{
	return mMana;
}

/*
 * Function:  SetMana(float Mana)
 * --------------------
 * Sets the Mana of the character to the passed in float
 *
 * returns: nothing
 */
void Character::SetMana(float Mana)
{
	mMana = Mana;
}

/*
 * Function:  IsDead()
 * --------------------
 * Check to see if the character is dead
 *
 * returns: nothing
 */
bool Character::IsDead()
{
	return mDead;
}

/*
 * Function:  SetMana(float Mana)
 * --------------------
 * Set the dead staus of the character
 *
 * returns: nothing
 */
void Character::SetDead(bool deadStatus)
{
	mDead = deadStatus;
}

/*
 * Function:  IsHero()
 * --------------------
 * Gets the Hero staus of the character
 *
 * returns: nothing
 */
bool Character::IsHero()
{
	return mHero;
}

/*
 * Function:  SetHero(bool heroStat)
 * --------------------
 * Set the Hero staus of the character
 *
 * returns: nothing
 */
void Character::SetHero(bool heroStat)
{
	mHero = heroStat;
}
