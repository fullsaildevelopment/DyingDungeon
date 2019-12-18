#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

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
		// Basic Attack
		mSkillList[0] = Skills(15, -25, true, Buffs(STATS::NONE, -5, 0, false, nullptr), "Basic Attack", "Idle");
		// Skill 1 (Bleed)
		mSkillList[1] = Skills(10, 15, true, Buffs(STATS::HP, 0.05f, 2, true, nullptr), "Skeletal Slash", "Idle");
		// Skill 2 (Big Damage & Bleed)
		mSkillList[2] = Skills(25, 40, true, Buffs(STATS::HP, 0.10f, 3, true, nullptr), "Necrotic Infection", "Idle");
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
			bestMove.skill = &mSkillList[i];
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

float EnemyComponent::ScoreMove(Skills skillOption, Character* target)
{
	float score = skillOption.GetDamage() - skillOption.GetManaCost();
	if (target->GetHP() - skillOption.GetDamage() == 0 && GetMana() >= skillOption.GetManaCost())
		score += 1000;
	if (GetHP() > 60 && skillOption.GetName() == "Necrotic Infection" && GetMana() >= skillOption.GetManaCost())
		score += 25;
	if (GetMana() - skillOption.GetManaCost() <= 10)
		score -= 10;
	return score;
}

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> enemyTeam)
{
	// Find my best option
	switch (mCurrentState)
	{
	case STATE::SELECTMOVE:
		break;
	case STATE::SELECTTARGET:
		break;
	case STATE::INPROGRESS:
		break;
	case STATE::FINISHED:
		break;
	default:
		break;
	}
	bool done = FindBestMove(playerTeam);

	if (done)
	{
		// Use the best move
		bestMove.skill->Use(*mGameObject->getComponent<Character>(), *bestMove.target);
		// If i have any buffs manage them 
		ManageStatusEffects();
		//Reset best move score
		bestMove.score = -1000;
		//Return true if we finished our turn
		return true;
	}

	return false;
}

void EnemyComponent::Die()
{
	if (GetHP() <= 0)
	{
		SetDead(true);
		//TODO Uncomment for death animation
		//mGameObject->getComponent<Odyssey::Animator>()->setAnimationClip("Death");
	}
}

