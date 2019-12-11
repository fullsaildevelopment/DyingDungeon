#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Character, HeroComponent)

void HeroComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	SetHP(100);
	SetMana(100);
}

void HeroComponent::update(double deltaTime)
{

}

void HeroComponent::TakeTurn()
{

}

/*
 * Function:  BasicAttack()
 * --------------------
 * Uses basic attack (skillList[0])
 *
 * Currently a public function but will be moved
 * to a private one
 *
 * returns: void
 */
void HeroComponent::BasicAttack(EnemyComponent& target)
{
	skillList[0].Use(target);
}