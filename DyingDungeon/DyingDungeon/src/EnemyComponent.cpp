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
		// Basic Attack
		std::shared_ptr<StatDown> tempSd = std::make_shared<StatDown>(0.25f, 3, STATS::Atk, nullptr);
		mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "BasicAttackButBetter", -5.0f, 10.0f, nullptr, false));
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

bool EnemyComponent::FindBestMove(std::vector<std::shared_ptr<Odyssey::Entity>> targets)
{
	Character* target = nullptr;
	for (std::shared_ptr<Odyssey::Entity> t : targets)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				if (target->IsHero() == true && target->IsDead() == false)
				{
					break;
				}
			}
		}
	}

	// TODO: MAKE THIS WORK LATER
	// - Changed 4 to 1 in loop
	for (int i = currentSkillMoveCheck; i < 1;)
	{
		if (mSkillList[i] != nullptr)
		{
			float score = ScoreMove(mSkillList[i].get(), target);

			if (score > bestMove.score)
			{
				bestMove.skill = mSkillList[i].get();
				bestMove.target = target;
				bestMove.score = score;
			}

			if (currentSkillMoveCheck >= 0)
			{
				currentSkillMoveCheck = 0;
				return true;
			}
		}
		currentSkillMoveCheck++;
		return false;
	}
}

float EnemyComponent::ScoreMove(Skills* skillOption, Character* target)
{
	/*float score = skillOption.GetDamage();
	if (skillOption.GetName() == "Basic Attack" && GetMana() < 10)
		score += 1000;
	
	if (skillOption.GetName() != "Basic Attack")
		score -= skillOption.GetManaCost();
	if (target->GetHP() - skillOption.GetDamage() == 0 && GetMana() >= skillOption.GetManaCost())
		score += 1000;
	if (GetHP() > 60 && skillOption.GetName() == "Necrotic Infection" && GetMana() >= skillOption.GetManaCost())
		score += 100;
	if (GetHP() > 60 && skillOption.GetName() == "Skeletal Slash" && GetMana() >= skillOption.GetManaCost())
		score += 35;
	if (GetMana() - skillOption.GetManaCost() <= 10)
		score -= 10;*/
	// Lanes crap for testing new skills system 
	float score = 0;
	if (skillOption->GetTypeId() == SKILLTYPE::ATTACK)
		score = 1000;
	return score;
}

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	// Find my best option
	switch (mCurrentState)
	{
	case STATE::NONE:
	{
		if (mStunned)
		{
			std::cout << GetName() << " is stunned!" << std::endl;
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
		//if (FindBestMove(playerTeam))
		if (mMoves.FindMove(mMoveOverride, playerTeam, enemyTeam))
		{
			mCurrentState = STATE::INPROGRESS;
			mAnimator->playClip(mMoves.GetMove()->skill->GetAnimationId());
		}
		break;
	}
	case STATE::INPROGRESS:
	{
		static bool trigger = false;

		if (!trigger && mAnimator->getProgress() > 0.25f)
		{
			if (mMoves.GetMove()->target->IsHero())
				mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
			trigger = true;
		}

		if (mAnimator->getProgress() > 0.9f)
		{
			// Use the best move
			mMoves.GetMove()->skill->Use(*mEntity->getComponent<Character>(), *mMoves.GetMove()->target);
			// If i have any buffs manage them 
			//Reset best move score
			mMoves.GetMove()->score = -10000;
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

void EnemyComponent::Die()
{
	if (GetHP() <= 0)
	{
		mCurrentState = STATE::DEAD;
		ClearStatusEffects();

		//TODO Uncomment for death animation
		mAnimator->playClip("Dead");
	}
}

