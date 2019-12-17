#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "EnemyComponent.h"

CLASS_DEFINITION(Character, HeroComponent)

HeroComponent::HeroComponent(HEROID id)
{
	SetHero(true);
	mEXP = 0;
	mSkillSelected = false;
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	switch (id)
	{
	case HEROID::Paladin:
	{
		mName = "Paladin";
		mBaseMaxHP = mCurrentHP = 150.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.0f;
		mDefense = 0.30f;
		// Basic Attack
		mSkillList[0] = Skills(10, 0, true, Buffs(STATS::NONE, -5, 0, false, nullptr), "Basic Attack", "");
		// Skill 1 (Raise Attack)
		mSkillList[1] = Skills(0, 10, false, Buffs(STATS::Atk, 0.15f, 3, false, nullptr), "Attack Up", "");
		// Skill 2 (Raise Defense)
		mSkillList[2] = Skills(0, 10, false, Buffs(STATS::Def, 0.15f, 3, false, nullptr), "Defense Up", "");
		// Skill 3 (Regen HP)
		mSkillList[3] = Skills(0, 0, false, Buffs(STATS::HP, -0.50f, 3, true, nullptr), "Regen", "");
		// Skill 4 (Shield)
		mSkillList[4] = Skills(0, 0, false, Buffs(STATS::Shd, 50.0f, 1, true, nullptr), "Shield", "");
		// Skill 5 (Big Attack)
		mSkillList[5] = Skills(35, 20, true, Buffs(STATS::HP, 0.25f, 4, true, nullptr), "Ultimate Move", "");
		break;
	}
	default:
		break;
	}
}

bool HeroComponent::TakeTurn(GameObjectList heros, GameObjectList enemies)
{
	if (mSkillSelected == false)
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(int('1')))          
		{
			mCurrentSkill = &mSkillList[0];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mSkillSelected = true;
		}
		 else if (Odyssey::InputManager::getInstance().getKeyPress(int('2')))
		{
			mCurrentSkill = &mSkillList[1];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mSkillSelected = true;
		}
		else if (Odyssey::InputManager::getInstance().getKeyPress(int('3')))
		{
			mCurrentSkill = &mSkillList[2];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mSkillSelected = true;
		}
		else if (Odyssey::InputManager::getInstance().getKeyPress(int('4')))
		{
			mCurrentSkill = &mSkillList[3];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mSkillSelected = true;
		}
		else if (Odyssey::InputManager::getInstance().getKeyPress(int('5')))
		{
			mCurrentSkill = &mSkillList[4];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mSkillSelected = true;
		}
	}
	else
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(int('1')))
		{
			if (mCurrentSkill->IsAttack())
			{
				mCurrentTarget = enemies[0]->getComponent<Character>();
			}
			else
			{
				mCurrentTarget = heros[0]->getComponent<Character>();
			}
			mCurrentSkill->Use(*mGameObject->getComponent<Character>(), *mCurrentTarget);
			mCurrentSkill = nullptr;
			mCurrentTarget = nullptr;
			mSkillSelected = false;
			ManageStatusEffects();
			return true;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(VK_BACK))
		{
			mCurrentSkill = nullptr;
			mSkillSelected = false;
			std::cout << "Reselect A Skill" << std::endl;
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
