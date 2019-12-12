#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "EnemyComponent.h"

CLASS_DEFINITION(Character, HeroComponent)

void HeroComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	SetHP(100);
	SetMana(100);
	SetHero(true);

	skillList[0] = Skills(10, 5);
}

bool HeroComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> characters)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(VK_RETURN))
	{
		for (std::shared_ptr<Odyssey::GameObject> temp : characters)
		{
			Character* target = temp->getComponent<Character>();
			if (target->IsHero() == false && target->IsDead() == false)
			{
				BasicAttack(target);
				return true;
			}
		}
	}

	return false;
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
void HeroComponent::BasicAttack(Character* target)
{
	skillList[0].Use(*mGameObject->getComponent<Character>(), *target);
}