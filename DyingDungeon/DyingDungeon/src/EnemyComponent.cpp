#include "EnemyComponent.h"
#include "Entity.h"
#include "Transform.h"
/// Check if better way
#include "Attack.h"
#include "Buffs.h"
#include "Heal.h"
//////////////////
#include "Bleed.h"
#include "StatUp.h"
#include "StatDown.h"

CLASS_DEFINITION(Character, EnemyComponent)

EnemyComponent::EnemyComponent(ENEMYID _enemyID)
{
	mDead = false;
	SetHero(false);
	mMoveOverride = SKILLTYPE::UNDEFINED;
	mCurrentState = STATE::NONE;
	mMoves = AIMoves(static_cast<int>(_enemyID), this);

	switch (_enemyID)
	{
	case ENEMYID::Skeleton:
	{
		// TODO: FIX YOUR SHIT - WHOEVER
		mName = "Skeleton";
		//fScoreMove = ScoreMove;
		mBaseMaxHP = mCurrentHP = 100.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.15f;
		mDefense = 0.05f;
		mSpeed = 20;
		mMoveOverride = SKILLTYPE::ATTACK;
		
		//// Basic Attack
		//mSkillList[0] = Skills(15, -25, true, Buffs(STATS::NONE, -5, 0, false, nullptr), "Basic Attack", "BasicAttackButBetter");
		//// Skill 1 (Bleed)
		//mSkillList[1] = Skills(10, 15, true, Buffs(STATS::HP, 0.05f, 2, true, nullptr), "Skeletal Slash", "FwdKick");
		//// Skill 2 (Big Damage & Bleed)
		//mSkillList[2] = Skills(25, 40, true, Buffs(STATS::HP, 0.10f, 3, true, nullptr), "Necrotic Infection", "SpinKick");
		break;
	}
	default:
		break;
	}
}

EnemyComponent::~EnemyComponent()
{
}

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	// Find my best option
	switch (mCurrentState)
	{
	case STATE::STUNNED:
	{
		mCurrentState = STATE::NONE;
		ManageAllEffects();
		return true;
	}
	case STATE::NONE:
	{
		//TODO: Change to lanes new stuff... Lane was here :D
		ManageStatusEffects(mRegens);
		ManageStatusEffects(mBleeds);
		if (mCurrentHP <= 0.0f)
			Die();
		else
			mCurrentState = STATE::SELECTMOVE;
		break;
	}
	case STATE::SELECTMOVE:
	{
		//if (FindBestMove(playerTeam))
		if (mMoves.FindMove(mMoveOverride, playerTeam, enemyTeam) && mMoves.GetMove()->target != nullptr)
		{
			mCurrentState = STATE::INPROGRESS;
			mAnimator->playClip(mMoves.GetMove()->skill->GetAnimationId());
		}
		break;
	}
	case STATE::INPROGRESS:
	{
		static bool triggerButBetter = false;
		if (!triggerButBetter && mAnimator->getProgress() > mMoves.GetMove()->skill->GetAnimationTiming())
		{
			// Static bool used to track whenever its time to play the recipent animation ie hit or be buffed 
			static bool trigger = false;
			if (!trigger && mAnimator->getProgress() > mMoves.GetMove()->skill->GetAnimationTiming())
			{
				// If its ment for the enemies play the hit animation to time with the animation timing
				if (mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::DEBUFF)
				{
					if (mMoves.GetMove()->skill->IsAOE())
					{
						for (std::shared_ptr<Odyssey::Entity> c : playerTeam)
						{
							if (c != nullptr)
								c.get()->getComponent<Odyssey::Animator>()->playClip("Hit");
						}
					}
					else if (mMoves.GetMove()->target != nullptr)
						mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
				}
				else
				{
					// If its ment for the players characters play the recived buff animation to time with the animation timing
					if (mMoves.GetMove()->skill->IsAOE())
					{
						for (std::shared_ptr<Odyssey::Entity> c : enemyTeam)
						{
							if (c != nullptr && c.get()->getComponent<EnemyComponent>() != this)
								c.get()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
						}
					}
					else if (mMoves.GetMove()->target != nullptr)
						mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
				}
				// Set trigger to true to avoid looping the recipents animation
				triggerButBetter = true;
			}
		}
		if (mAnimator->getProgress() > 0.9f)
		{
			if (mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				if (mMoves.GetMove()->skill->IsAOE())
				{
					Character* temp = nullptr;
					for (std::shared_ptr<Odyssey::Entity> c : playerTeam)
					{
						if (c != nullptr)
						{
							temp = c.get()->getComponent<Character>();
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
					mMoves.GetMove()->skill->Use(*this, *mMoves.GetMove()->target);
			}
			else
			{
				if (mMoves.GetMove()->skill->IsAOE())
				{
					Character* temp = nullptr;
					for (std::shared_ptr<Odyssey::Entity> c : enemyTeam)
					{
						if (c != nullptr)
						{
							temp = c.get()->getComponent<Character>();
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
					mMoves.GetMove()->skill->Use(*this, *mMoves.GetMove()->target);
			}
			mMoves.ResetMove();
			triggerButBetter = false;
			mCurrentState = STATE::FINISHED;
		}
		break;
	}
	case STATE::FINISHED:
	{
		mCurrentState = STATE::NONE;
		ManageStatusEffects(mBuffs);
		ManageStatusEffects(mDebuffs);
		ManageStatusEffects(mSheilds);
		return true;
		break;
	}
	case STATE::DEAD:
	{
		if (mAnimator->getProgress() > 0.8f)
			return true;
		break;
	}
	default:
		break;
	}
	return false;
}
void EnemyComponent::Die()
{
	mCurrentState = STATE::DEAD;
	ClearStatusEffects();
	mAnimator->playClip("Dead");
	pTurnNumber->setText(L"X");
}

