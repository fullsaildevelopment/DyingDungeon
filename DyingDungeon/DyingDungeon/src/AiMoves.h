#pragma once
#include "Skills.h"
#include "Entity.h"

class Character;

class AIMoves
{
	struct Move
	{
		Character* target = nullptr;
		std::shared_ptr<Skills> skill = nullptr;
		float score = -10000;
	};

	public:
		AIMoves();
		AIMoves(int _enemyID, Character* _caster);
		~AIMoves() = default;
		bool FindMove(SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	private:
		//General Functions
		bool FindBestMove(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		float ScoreMove(std::shared_ptr<Skills> skill, Character* target);

		//Skeleton
		void SkeletonDeterminePriority();
		
		//UngaAttack
		void UngaAttackDeterminePriority();
		
		//Member Variables
		std::vector<std::shared_ptr<Skills>> mSkillList;
		std::vector<Odyssey::Entity> mBestTarget;
		Character* caster;

		std::shared_ptr<Move> mBestMove;
		int mEnemyID;
		int mCurrMoveCheck;
		SKILLTYPE mPriorityMove;


		//////////////////////////GET FUNCTIONS//////////////////////////////////
	public:
		std::shared_ptr<Move> GetMove();

		//////////////////////////SET FUNCTIONS//////////////////////////////////
	public:
		void ResetMove();
};