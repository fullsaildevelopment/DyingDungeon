#pragma once
#include "AiMoves.h"
#include "Character.h"
//////
#include "Attack.h"
#include "Buffs.h"
//////
#include "Provoked.h"
#include "Bleed.h"
#include "Regens.h"

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
	//Priority Set To None
	mPriorityMove = SKILLTYPE::UNDEFINED;
	
	//Best Move Initialization
	mBestMove = std::make_shared<AIMoves::Move>();
	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;

	//My ID and Who I am
	mEnemyID = _enemyID;
	caster = _caster;
	
	mCurrMoveCheck = 0;

	switch (_enemyID)
	{
		case 0:
		{
			//Setup Moves
			std::shared_ptr<StatusEffect> temp = std::make_shared<Regens>(0.25f, 1, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>("Basic Attack", "BasicAttackButBetter", 0.25f, -5.0f, temp, true));
			//mSkillList.push_back(std::make_shared<Attack>("HitButBigger", "BasicAttackButBetter", 0.25f, 20.0f, 25.0f));
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
	
	finished = FindBestMove(playerTeam, enemyTeam);
	return finished;
}

bool AIMoves::FindBestMove(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	bool finished = false;
	Character* currTarget = nullptr;
	
	for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
	{
		if (t)
		{
			if (currTarget = t->getComponent<Character>())
			{
				if (currTarget->IsHero() == true && currTarget->GetState() != STATE::DEAD)
				{
					break;
				}
			}
		}
	}

	if (currTarget != nullptr && currTarget->GetState() != STATE::DEAD)
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
	else
	{
		ResetMove();
		finished = true;
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

void AIMoves::ResetMove()
{
	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;
}