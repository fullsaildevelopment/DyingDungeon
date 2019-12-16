#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "EnemyComponent.h"

CLASS_DEFINITION(Character, HeroComponent)

void HeroComponent::initialize()
{
	onEnable();
	mGameObject->addComponent<Odyssey::Transform>();

	mBaseMaxHP = 100.0f;
	mBaseMaxMana = 100.0f;

	SetHP(100);
	SetMana(100);
	SetHero(true);

	mSkillList[0] = Skills(10, 5, Buffs(0,0.05f,2,true,nullptr));
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

void HeroComponent::Die()
{
	if (GetHP() <= 0)
	{
		SetDead(true);
		//TODO Uncomment for death animation
		//mGameObject->getComponent<Odyssey::Animator>()->setAnimationClip("Death");
	}
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
	mSkillList[0].Use(*mGameObject->getComponent<Character>(), *target);
}