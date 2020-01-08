#pragma once
#include "AiMoves.h"
#include "Character.h"
#include "Attack.h"

AIMoves::AIMoves(ENEMYID _enemyID)
{
	mEnemyID = _enemyID;
	mPriorityOverride = SKILLTYPE::UNDEFINED;

	switch (_enemyID)
	{
		case ENEMYID::Skeleton:
		{
			//Setup Moves
			std::shared_ptr<Skills> temp = std::make_shared<Attack>("Basic Attack", "BasicAttackButBetter", -5, 10, nullptr, false);
			skillList.push_back(temp);
			temp = std::make_shared<Attack>("HitButBigger", "BasicAttackButBetter", 20, 25, nullptr, false);
			skillList.push_back(temp);
			break;
		}
	}
}

bool AIMoves::UseBestMove(SKILLTYPE priorityOverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	mPriorityOverride = priorityOverride;

	//Determine Enemy Type
	switch (mEnemyID)
	{
		case ENEMYID::Skeleton:
		{
			//FindBestMove
			if (mPriorityOverride == SKILLTYPE::UNDEFINED)
				SkeletonDeterminePriority();
			return FindBestMove();
		}
	}
}

bool AIMoves::FindBestMove()
{
	bool finished = false;
	
	for (; mCurrMoveCheck < skillList.size(); mCurrMoveCheck++)
	{

	}

	if (finished == true)
		mCurrMoveCheck = 0;

	return finished;
}

void AIMoves::SkeletonDeterminePriority()
{
	mPriorityOverride = SKILLTYPE::ATTACK;
}

void AIMoves::UngaAttackDeterminePriority()
{
	mPriorityOverride = SKILLTYPE::ATTACK;
}

AIMoves::~AIMoves()
{

}