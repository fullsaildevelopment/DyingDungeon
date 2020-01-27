#pragma once
//#include "Skills.h"
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
		float ScoreMove(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		
		void ScoreMoveAttack(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);
		void ScoreMoveAttackAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);

		void ScoreMoveBuff(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		void ScoreMoveBuffAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

		float StatusEffectScore(std::shared_ptr<Skills> skill, Character* target);

		void ResetDecidingMoves();

		//Skeleton
		void SkeletonDeterminePriority();
		
		//UngaAttack
		void UngaAttackDeterminePriority();

		void BossDeterminePriority();
		
		//Member Variables
		std::vector<std::shared_ptr<Skills>> mSkillList;
		std::vector<Odyssey::Entity> mBestTarget;
		Character* caster;

		//Moves
		std::shared_ptr<AIMoves::Move> mBestMove;
		AIMoves::Move mBestAttack;
		AIMoves::Move mBestBuff;
		AIMoves::Move mBestDebuff;
		AIMoves::Move mBestHeal;

		int mCurrMoveCheck;
		SKILLTYPE mPriorityMove;

		int mEnemyID;

		//////////////////////////GET FUNCTIONS//////////////////////////////////
	public:
		std::shared_ptr <AIMoves::Move> GetMove();
		AIMoves::Move mPrevMove;
		std::vector<std::shared_ptr<Skills>> GetSkillList();

		//////////////////////////SET FUNCTIONS//////////////////////////////////
	public:
		void ResetMove();

	private:
		void SetPrevMove();
};