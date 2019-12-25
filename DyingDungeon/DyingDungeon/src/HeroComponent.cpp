#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "EnemyComponent.h"
/// Check if better way
#include "Attack.h"
#include "Bleed.h"

CLASS_DEFINITION(Character, HeroComponent)

HeroComponent::HeroComponent(HEROID id)
{
	SetHero(true);
	mEXP = 0;
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	mCurrentState = STATE::NONE;
	switch (id)
	{
	case HEROID::Paladin:
	{
		mName = "Paladin";
		mBaseMaxHP = mCurrentHP = 150.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.0f;
		mDefense = 0.30f;
		mSpeed = 35.0f;
		mShielding = 0.0f;
		mSkillList = new Skills*[TOTALSKILLS];
		for (int i = 0; i < TOTALSKILLS; ++i)
			mSkillList[i] = nullptr;
		// Basic Attack
		mSkillList[0] = new Attack("Basic Attack", "BasicAttack", -5, 15, nullptr);
		// Skill 1 (Big Attack)
		Bleed* tempB = new Bleed(0.50f, 4, nullptr);
		mSkillList[1] = new Attack("Bleed Attack", "BigAttack", 25, 25, tempB);
		tempB = nullptr;
		//// Basic Attack
		//mSkillList[0] = Skills(10, -25, true, Buffs(STATS::NONE, -5, 0, false, nullptr), "Basic Attack", "BasicAttack");
		//// Skill 1 (Raise Attack)
		//mSkillList[1] = Skills(0, 10, false, Buffs(STATS::Atk, 0.15f, 3, false, nullptr), "Attack Up", "AttackUp");
		//// Skill 2 (Raise Defense)
		//mSkillList[2] = Skills(0, 10, false, Buffs(STATS::Def, 0.15f, 3, false, nullptr), "Defense Up", "Defense");
		//// Skill 3 (Regen HP)
		//mSkillList[3] = Skills(0, 35, false, Buffs(STATS::HP, -0.50f, 3, true, nullptr), "Regen", "Heal");
		//// Skill 4 (Shield)
		//mSkillList[4] = Skills(0, 25, false, Buffs(STATS::Shd, 50.0f, 5, false, nullptr), "Shield", "Shield");
		//// Skill 5 (Big Attack)
		//mSkillList[5] = Skills(35, 45, true, Buffs(STATS::HP, 0.25f, 4, true, nullptr), "Ultimate Move", "BigAttack");
		//// Skill 6 (Stun Attack)
		//mSkillList[6] = Skills(25, 25, true, Buffs(STATS::Stn, 0.0f, 1, false, nullptr), "Stunning Strike", "Stun");
		break;
	}
	default:
		break;
	}
}

HeroComponent::~HeroComponent()
{
	for (int i = 0; i < TOTALSKILLS; ++i)
	{
		if (mSkillList[i] != nullptr)
		{
			delete mSkillList[i];
			mSkillList[i] = nullptr;
		}
	}
}

bool HeroComponent::TakeTurn(GameObjectList heros, GameObjectList enemies)
{
	//Make these if checks into a state machine
	
	switch (mCurrentState)
	{
	case STATE::NONE:
	{
		if (mStunned)
		{
			std::cout << mName << " is stunned!" << std::endl;
			return ManageAllEffects();
		}
		ManageStatusEffects(mBleeds);
		ManageStatusEffects(mRegens);
		if (mCurrentHP <= 0.0f)
			Die();
		else
			mCurrentState = STATE::SELECTMOVE;
		break;
	}
	case STATE::SELECTMOVE:
	{
		if (mCurrentState == STATE::DEAD)
			return false;
		if (Odyssey::InputManager::getInstance().getKeyPress(int('1')) && mSkillList[0]->GetManaCost() <= mCurrentMana)
		{
			mCurrentSkill = mSkillList[0];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mCurrentState = STATE::SELECTTARGET;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(int('2')) && mSkillList[1]->GetManaCost() <= mCurrentMana)
		{
			mCurrentSkill = mSkillList[1];
			std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
			mCurrentState = STATE::SELECTTARGET;
		}
		break;
	}
	case STATE::SELECTTARGET:
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(int('1')))
		{
			if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				mCurrentTarget = enemies[0]->getComponent<Character>();
			}
			else
			{
				mCurrentTarget = heros[0]->getComponent<Character>();
			}
			mAnimator->playClip(mCurrentSkill->GetAnimationId());
			mCurrentState = STATE::INPROGRESS;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(VK_BACK))
		{
			mCurrentSkill = nullptr;
			mCurrentState = STATE::SELECTMOVE;
			std::cout << "Reselect A Skill" << std::endl;
		}
		break;
	}
	case STATE::INPROGRESS:
	{
		static bool trigger = false;
		if (!trigger && mAnimator->getProgress() > 0.25f)
		{
			if (mCurrentTarget->IsHero() == false)
				mCurrentTarget->getGameObject()->getComponent<Odyssey::Animator>()->playClip("Hit");
			trigger = true;
		}
		
		if (mAnimator->getProgress() > 0.9f)
		{
			mCurrentSkill->Use(*mGameObject->getComponent<Character>(), *mCurrentTarget);
			mCurrentSkill = nullptr;
			mCurrentTarget = nullptr;
			mCurrentState = STATE::NONE;
			trigger = false;
			ManageStatusEffects(mBuffs);
			ManageStatusEffects(mDebuffs);
			return true;
		}
		break;
	}
	case STATE::DEAD:
	{
		if (mAnimator->getProgress() > 0.8f)
		{
			SetDead(true);
			return true;
		}
		break;
	}
	default:
		break;
	}
	return false;
}

void HeroComponent::Die()
{
	if (GetHP() <= 0)
	{
		mCurrentState = STATE::DEAD;
		//TODO Uncomment for death animation
		mAnimator->playClip("Dead");
	}
}
