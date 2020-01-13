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
		float ScoreMove(std::shared_ptr<Skills> skill, Character* target, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);
		float ScoreMoveAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);

		//Skeleton
		void SkeletonDeterminePriority();
		
		//UngaAttack
		void UngaAttackDeterminePriority();
		
		//Member Variables
		std::vector<std::shared_ptr<Skills>> mSkillList;
		std::vector<Odyssey::Entity> mBestTarget;
		Character* caster;

		std::shared_ptr<AIMoves::Move> mBestMove;
		int mEnemyID;
		int mCurrMoveCheck;
		SKILLTYPE mPriorityMove;


		//////////////////////////GET FUNCTIONS//////////////////////////////////
	public:
		std::shared_ptr <AIMoves::Move> GetMove();
		AIMoves::Move mPrevMove;

		//////////////////////////SET FUNCTIONS//////////////////////////////////
	public:
		void ResetMove();

	private:
		void SetPrevMove();
};