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
}

void Character::update(double deltaTime)
{

}

void Character::TakeTurn()
{

}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the HP of the enemy
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
 * Sets the HP of the enemy to the passed in float
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
 * Gets the mana of the enemy
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
 * Sets the Mana of the enemy to the passed in float
 *
 * returns: nothing
 */
void Character::SetMana(float Mana)
{
	mMana = Mana;
}

