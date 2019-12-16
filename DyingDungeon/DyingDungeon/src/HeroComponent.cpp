#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "EnemyComponent.h"

CLASS_DEFINITION(Character, HeroComponent)

HeroComponent::HeroComponent(HEROID id)
{
	SetHero(true);
	mEXP = 0;
	switch (id)
	{
	case HEROID::Paladin:
	{
		mBaseMaxHP = mCurrentHP = 150.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.0f;
		mDefense = 0.30f;
		// Basic Attack
		mSkillList[0] = Skills(10, 0, Buffs(STATS::NONE, -5, 0, false, nullptr), "Basic Attack");
		// Skill 1 (Raise Attack)
		mSkillList[1] = Skills(0, 10, Buffs(STATS::Atk, 0.15f, 3, false, nullptr), "Attack Up");
		// Skill 2 (Raise Defense)
		mSkillList[2] = Skills(0, 10, Buffs(STATS::Def, 0.15f, 3, false, nullptr), "Defense Up");
		// Skill 3 (Bleed Target HP)
		mSkillList[3] = Skills(5, 20, Buffs(STATS::HP, 0.05f, 3, true, nullptr), "Bleed Target");
		break;
	}
	default:
		break;
	}
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