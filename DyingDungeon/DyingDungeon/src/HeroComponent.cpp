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
#include "Shields.h"
#include "Provoked.h"

CLASS_DEFINITION(Character, HeroComponent)

HeroComponent::HeroComponent(HEROID id)
{
	mDead = false;
	SetHero(true);
	mEXP = 0;
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	mCurrentState = STATE::NONE;
	StatusEffect* temp = nullptr;
	Skills* tempSkill = nullptr;
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
		// Basic Attack (Add Provoke 30% chance)
		temp = new Provoked(1, this, nullptr);
		mSkillList[0] = new Attack("Basic Attack", "BasicAttack", -5, 10, temp, false);
		// Skill 1 Judgement (deal damage and heal self)
		tempSkill = new Heal("SelfHeal", "Idle", 0.0f, 20.0f);
		mSkillList[1] = new Attack("Judgement", "BigAttack", 15.0f, 20.0f, nullptr, false, tempSkill);
		// Skill 2 Shield of Light (Gives the team 25 temp hp with a shield)
		temp = new Shields(25.0f, 3, nullptr);
		mSkillList[2] = new Buffs("Shield of Light", "Heal", 20.0f, temp, true, true);
		// Skill 3 Blessing of light (Gives the team 50% damage reduction for 2 turns)
		temp = new StatUp(0.50f, 3, STATS::Def, nullptr);
		mSkillList[3] = new Buffs("Blessing of Light", "Defense", 15.0f,temp,true,true);
		temp = nullptr;
		tempSkill = nullptr;
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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			if (mSkillList[0]->GetManaCost() <= mCurrentMana)
			{
				mCurrentSkill = mSkillList[0];
				std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
				if (mCurrentSkill->IsAOE())
				{
					if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
						std::cout << "This move will hit the whole enemy party, hit one to confirm use." << std::endl;
					else
						std::cout << "This move will apply to your whole party, hit one to confirm use." << std::endl;
					mCurrentState = STATE::AOECONFIRM;
				}
				else
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
		break;
	}
	case STATE::AOECONFIRM:
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			mCurrentState = STATE::INPROGRESS;
		}
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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D2))
		{
			if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				mCurrentTarget = enemies[1]->getComponent<Character>();
			}
			else
			{
				mCurrentTarget = heros[1]->getComponent<Character>();
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
			if (mCurrentSkill->IsAOE() == false && mCurrentTarget->IsHero() == false)
				mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
			trigger = true;
		}
		
		if (mAnimator->getProgress() > 0.9f)
		{
			if (mCurrentSkill->IsAOE())
			{
				Character* temp = nullptr;
				if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
				{
					for (int i = 0; i < enemies.size(); ++i)
					{
						if (enemies[i] != nullptr)
						{
							temp = enemies[i]->getComponent<Character>();
							if(temp->IsDead() == false)
								mCurrentSkill->Use(*mEntity->getComponent<Character>(), *temp);
						}
					}
				}
				else
				{
					for (int i = 0; i < heros.size(); ++i)
					{
						if (heros[i] != nullptr)
						{
							temp = heros[i]->getComponent<Character>();
							if (temp->IsDead() == false)
								mCurrentSkill->Use(*mEntity->getComponent<Character>(), *temp);
						}
					}
				}
			}
			else
				mCurrentSkill->Use(*mEntity->getComponent<Character>(), *mCurrentTarget);
			mCurrentSkill = nullptr;
			mCurrentTarget = nullptr;
			mCurrentState = STATE::NONE;
			trigger = false;
			ManageStatusEffects(mBuffs);
			ManageStatusEffects(mDebuffs);
			ManageStatusEffects(mSheilds);
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
