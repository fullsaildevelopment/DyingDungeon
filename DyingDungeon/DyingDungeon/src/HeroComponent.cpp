#include "HeroComponent.h"
#include "Entity.h"
#include "Transform.h"
#include "EnemyComponent.h"
/// Check if better way
#include "Attack.h"
#include "Buffs.h"
#include "Heal.h"
//////////////////
#include "Bleed.h"
#include "StatUp.h"
#include "StatDown.h"
#include "Regens.h"
#include "Stun.h"

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
		mSkillList[0] = new Attack("Basic Attack", "BasicAttack", -5, 10, nullptr);
		// Skill 1 (Big Attack)
		Bleed* tempB = new Bleed(0.50f, 4, nullptr);
		mSkillList[1] = new Attack("Bleed Attack", "BigAttack", 25, 25, tempB);
		tempB = nullptr;
		// Skill 2 (Heal)
		mSkillList[2] = new Heal("Heal", "Heal", 10, 25);
		// Skill 3 (StatUP)
		StatUp* tempSU = new StatUp(0.25f,1,STATS::Atk,nullptr);
		mSkillList[3] = new Buffs("StatUp", "AttackUp", 10, tempSU,true);
		tempSU = nullptr;
		// Skill 4 (Regen)
		StatusEffect* temp = new Regens(0.25f,4,nullptr);
		mSkillList[4] = new Buffs("Regen", "AttackUp", 10, temp,true);
		temp = nullptr;
		// Skill 5 (StatDown)
		temp = new StatDown(0.50f, 2, STATS::Atk, nullptr);
		mSkillList[5] = new Buffs("StatDown", "AttackUp", 10, temp, false);
		temp = nullptr;
		// Skill 6 (Stun)
		temp = new Stun(1, nullptr);
		mSkillList[6] = new Attack("StunAttak", "BasicAttack", 10, 15, temp);
		temp = nullptr;
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

bool HeroComponent::TakeTurn(EntityList heros, EntityList enemies)
{
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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			if (mSkillList[0]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[0];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D2))
		{
			if (mSkillList[1]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[1];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D3))
		{
			if (mSkillList[2]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[2];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D4))
		{
			if (mSkillList[3]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[3];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D5))
		{
			if (mSkillList[4]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[4];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D6))
		{
			if (mSkillList[5]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[5];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D7))
		{
			if (mSkillList[6]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[6];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				mCurrentState = STATE::SELECTTARGET;
			}
			else
				std::cout << "You dont have enough mana for that move." << std::endl;
		}
		break;
	}
	case STATE::SELECTTARGET:
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Back))
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
				mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
			trigger = true;
		}
		
		if (mAnimator->getProgress() > 0.9f)
		{
			mCurrentSkill->Use(*mEntity->getComponent<Character>(), *mCurrentTarget);
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
