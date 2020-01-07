#pragma once
#include "Skills.h"

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
		AIMoves() = default;
		AIMoves(ENEMYID _enemyID);
		~AIMoves();
		bool UseBestMove(SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	private:
		//Skeleton
		bool FindBestMove(SKILLTYPE override);
		SKILLTYPE SkeletonDeterminePriority(SKILLTYPE override);
		
		//UngaAttack
		SKILLTYPE UngaAttackDeterminePriority(SKILLTYPE override);

		//Variables
		std::vector<std::shared_ptr<Skills>> skillList;
		Move mBestMove;
		int mCurrMoveCheck;
};