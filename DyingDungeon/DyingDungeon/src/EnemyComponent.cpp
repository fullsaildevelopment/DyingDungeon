#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"
/// Check if better way
#include "Attack.h"

CLASS_DEFINITION(Character, EnemyComponent)

EnemyComponent::EnemyComponent(ENEMYID _enemyID)
{
	SetHero(false);
	mCurrentState = STATE::SELECTMOVE;
	switch (_enemyID)
	{
	case ENEMYID::Skeleton:
	{
		mName = "Skeleton";
		//fScoreMove = ScoreMove;
		mBaseMaxHP = mCurrentHP = 100.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.15f;
		mDefense = 0.05f;
		mSpeed = 20;
		mSkillList = new Skills* [TOTALSKILLS];
		// Basic Attack
		mSkillList[0] = new Attack(SKILLTYPE::ATTACK, "Basic Attack", "BasicAttackButBetter", -5, 10, nullptr);
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

bool EnemyComponent::FindBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{
	Character* target = nullptr;
	for (std::shared_ptr<Odyssey::GameObject> t : targets)
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

	for (int i = currentSkillMoveCheck; i < 4;)
	{
		float score = ScoreMove(mSkillList[i], target);

		if (score > bestMove.score)
		{
			bestMove.skill = mSkillList[i];
			bestMove.target = target;
			bestMove.score = score;
		}

		currentSkillMoveCheck++;
		if (currentSkillMoveCheck >= 4)
		{
			currentSkillMoveCheck = 0;
			return true;
		}

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

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> enemyTeam)
{
	
	if (mStunned && mCurrentState != STATE::DEAD)
	{
		std::cout << GetName() << " is stunned!" << std::endl;
		ManageAllEffects();
		return true;
	}

	// Find my best option
	switch (mCurrentState)
	{
	case STATE::SELECTMOVE:
	{
		if (FindBestMove(playerTeam))
		{
			mCurrentState = STATE::INPROGRESS;
			//ManageStatusEffects();
			if(mCurrentState == STATE::INPROGRESS)
				mAnimator->playClip(bestMove.skill->GetAnimationId());
		}
		break;
	}
	case STATE::INPROGRESS:
	{
		static bool trigger = false;

		if (!trigger && mAnimator->getProgress() > 0.25f)
		{
			if (bestMove.target->IsHero())
				bestMove.target->getGameObject()->getComponent<Odyssey::Animator>()->playClip("Hit");
			trigger = true;
		}

		if (mAnimator->getProgress() > 0.9f)
		{
			// Use the best move
			bestMove.skill->Use(*mGameObject->getComponent<Character>(), *bestMove.target);
			// If i have any buffs manage them 
			//Reset best move score
			bestMove.score = -1000;
			mCurrentState = STATE::SELECTMOVE;
			trigger = false;
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

