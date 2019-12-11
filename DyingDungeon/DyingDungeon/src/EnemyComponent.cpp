#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

void EnemyComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	SetHP(100);
	SetMana(100);
}

void EnemyComponent::update(double deltaTime)
{

}

void EnemyComponent::TakeTurn()
{
	
}