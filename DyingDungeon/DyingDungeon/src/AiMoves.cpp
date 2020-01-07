#pragma once
#include "AiMoves.h"
#include "Character.h"
#include "Attack.h"

AIMoves::AIMoves(ENEMYID _enemyID)
{
	switch (_enemyID)
	{
		case ENEMYID::Skeleton:
		{
			std::shared_ptr<Skills> temp = std::make_shared<Attack>("Basic Attack", "BasicAttackButBetter", -5, 10, nullptr);
			skillList.push_back(temp);
			break;
		}
	}
}

bool AIMoves::UseBestMove(SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	
}

SKILLTYPE AIMoves::SkeletonDeterminePriority(SKILLTYPE override)
{
	
}

SKILLTYPE AIMoves::UngaAttackDeterminePriority(SKILLTYPE override)
{
	
}