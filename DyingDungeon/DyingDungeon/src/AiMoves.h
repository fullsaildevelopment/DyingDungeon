#pragma once
#include "Skills.h"
#include "Character.h"

class Character;

class AIMoves
{
	struct Move
	{
		Character* target;
		Skills* skill;
		float score = -10000;
	};

	public:
		AIMoves();
		AIMoves(ENEMYID _enemyID);
		~AIMoves();
		bool UseBestMove(SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	private:
		//Skeleton
		bool FindBestMove();
		void SkeletonDeterminePriority();
		
		//UngaAttack
		void UngaAttackDeterminePriority();

		//Team Lists
		std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam;
		std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam;
		
		//Member Variables
		std::vector<std::shared_ptr<Skills>> skillList;
		Move mBestMove;
		ENEMYID mEnemyID;
		int mCurrMoveCheck;
		SKILLTYPE mPriorityOverride;
};