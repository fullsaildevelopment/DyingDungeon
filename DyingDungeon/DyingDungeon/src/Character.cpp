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

bool Character::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> _allCharacters)
{
	return false;
}

/*
 * Function:  Die()
 * --------------------
 * Sets dead boolean to true
 * Sets animation to death animation
 *
 * returns: void
 */
void Character::Die()
{
}

/*
 * Function:  TakeDamage(float dmg)
 * --------------------
 * Pass in a float to deal damage to the character
 * Calculates any damage reduction (TODO)
 *
 * returns: void
 */
void Character::TakeDamage(float dmg)
{
    //TODO calculate damage reduction here

    //Take Damage
    mHP - dmg;
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
    if (mHP < 0)
        mHP = 0;
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
 * returns: void
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
 * returns: bool
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
 * returns: void
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
 * returns: bool
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
 * returns: void
 */
void Character::SetHero(bool heroStat)
{
	mHero = heroStat;
}

/*
 * Function:  GetName()
 * --------------------
 * Gets the characters name
 *
 * returns: string
 */
std::string Character::GetName()
{
	return mName;
}

/*
 * Function:  SetName(string newName)
 * --------------------
 * Set the character's name
 *
 * returns: void
 */
void Character::SetName(std::string newName)
{
	mName = newName;
}
