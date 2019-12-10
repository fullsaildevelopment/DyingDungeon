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

//Get HP (Returns a float)
float EnemyComponent::GetHP()
{
	return mHP;
}
//Set HP (Pass in float)
void EnemyComponent::SetHP(float HP)
{
	mHP = HP;
}

//Get Mana (Returns a float)
float EnemyComponent::GetMana()
{
	return mMana;
}
//Set Mana (Pass in float)
void EnemyComponent::SetMana(float Mana)
{
	mMana = Mana;
}