#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, EnemyComponent)

void EnemyComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();
}

void EnemyComponent::update(double deltaTime)
{
	
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the HP of the enemy
 *
 * returns: A float representing current HP
 */
float EnemyComponent::GetHP()
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
void EnemyComponent::SetHP(float HP)
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
float EnemyComponent::GetMana()
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
void EnemyComponent::SetMana(float Mana)
{
	mMana = Mana;
}