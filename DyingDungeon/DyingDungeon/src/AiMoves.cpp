#pragma once
#include "AiMoves.h"
#include "Character.h"
#include "Attack.h"

AIMoves::AIMoves()
{
	mCurrMoveCheck = 0;
	mBestMove = nullptr;
	caster = nullptr;
	mEnemyID = -1;
	mPriorityMove = SKILLTYPE::UNDEFINED;
}

AIMoves::AIMoves(int _enemyID, Character* _caster)
{
	mBestMove = std::make_shared<AIMoves::Move>();
	mCurrMoveCheck = 0;
	mEnemyID = _enemyID;
	caster = _caster;
	mPriorityMove = SKILLTYPE::UNDEFINED;

	switch (_enemyID)
	{
		case 0:
		{
			//Setup Moves
			mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "BasicAttackButBetter", -5, 10, nullptr, false));
			mSkillList.push_back(std::make_shared<Attack>("HitButBigger", "BasicAttackButBetter", 20, 25, nullptr, false));
			break;
		}
		default:
		{
			break;
		}
	}
}

bool AIMoves::FindMove(SKILLTYPE priorityOverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	mPriorityMove = priorityOverride;
	mPlayerTeam = playerTeam;
	mEnemyTeam = enemyTeam;
	bool finished = false;

	//Determine Enemy Type
	switch (mEnemyID)
	{
		case 0:
		{
			//FindBestMove
			if (mPriorityMove == SKILLTYPE::UNDEFINED)
				SkeletonDeterminePriority();
			break;
		};
		default:
		{
			break;
		};
	}

	finished = FindBestMove();
	return finished;
}

bool AIMoves::FindBestMove()
{
	bool finished = false;
	Character* currTarget = nullptr;
	
	for (std::shared_ptr<Odyssey::Entity> t : mPlayerTeam)
	{
		if (t)
		{
			if (currTarget = t->getComponent<Character>())
			{
				if (currTarget->IsHero() == true && currTarget->IsDead() == false)
				{
					break;
				}
			}
		}
	}

	for (; mCurrMoveCheck < mSkillList.size();)
	{
		float score = ScoreMove(mSkillList[mCurrMoveCheck], currTarget);
		
		if (score > mBestMove->score)
		{
			mBestMove->skill = mSkillList[mCurrMoveCheck];
			mBestMove->target = currTarget;
			mBestMove->score = score;
		}

		if (mCurrMoveCheck >= mSkillList.size() - 1)
		{
			mCurrMoveCheck = 0;
			finished = true;
			break;
		}

		mCurrMoveCheck++;
		break;
	}

	return finished;
}

float AIMoves::ScoreMove(std::shared_ptr<Skills> skill, Character* target)
{
	float score = 0;
	
	//Do we have enough mana to actually use the skill
	if (skill->GetManaCost() > caster->GetMana())
		return -10000;


	if (skill->GetTypeId() == mPriorityMove)
		score += 100;

	return score;
}

void AIMoves::SkeletonDeterminePriority()
{
	mPriorityMove = SKILLTYPE::ATTACK;
}

void AIMoves::UngaAttackDeterminePriority()
{
	mPriorityMove = SKILLTYPE::ATTACK;
}

///////GET FUNCTIONS///////

std::shared_ptr<AIMoves::Move> AIMoves::GetMove()
{
	return mBestMove;
}