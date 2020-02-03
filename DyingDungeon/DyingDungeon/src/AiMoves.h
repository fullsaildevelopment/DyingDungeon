#pragma once
#include "Skills.h"
#include "Entity.h"

class Character;

class AIMoves
{
	//Move struct
	struct Move
	{
		Character* target = nullptr;
		std::shared_ptr<Skills> skill = nullptr;
		float score = -10000;
	};

	public:
		// Default constructor
		AIMoves();

		// Constructor
		AIMoves(int _enemyID, Character* _caster);

		// Default Deconstructor
		~AIMoves() = default;
		
		
		// Called outside to find the best move. Return true if finished.
		bool FindMove(GameplayTypes::SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	private:
		// Itterates through all the skills until we are done. breaking after each one.
		bool SkillCheck(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		
		// Scores the current move against all targets
		void ScoreMove(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		
		// Score the current move if it's a single target attack
		void ScoreMoveAttack(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);

		// Score the current move if it's a aoe attack
		void ScoreMoveAttackAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);

		// Socr ethe current move if it's a single target buff
		void ScoreMoveBuff(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		
		// Score the current move if it's a aoe buff
		void ScoreMoveBuffAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

		// Score the moves status effects if any
		float StatusEffectScore(std::shared_ptr<Skills> skill, Character* target);

		// Skeleton
		void SkeletonDeterminePriority();
		
		// UngaAttack
		void UngaAttackDeterminePriority();

		// Ganfaul
		void GanfaulDeterminePriority();
		
		// Member Variables
		//-------------------------------------------------------//

		// Our Skills
		std::vector<std::shared_ptr<Skills>> mSkillList;

		// Who we are
		Character* mCaster;

		// Best Overall move found
		std::shared_ptr<AIMoves::Move> mBestMove;

		// Best Attack found
		AIMoves::Move mBestAttack;

		// Best Buff found
		AIMoves::Move mBestBuff;

		// Best Debuff found
		AIMoves::Move mBestDebuff;

		// Best Heal found
		AIMoves::Move mBestHeal;

		// Previous Best Move
		AIMoves::Move mPrevMove;
		
		// Move Priority
		GameplayTypes::SKILLTYPE mPriorityMove;

		// Current skill index for finding best move
		int mCurrMoveCheck;

		// Our EnemyID
		int mEnemyID;

		//////////////////////////GET FUNCTIONS//////////////////////////////////
	public:
		// Gets the best move
		std::shared_ptr <AIMoves::Move> GetMove();

		// Gets the skill list of the enemy
		std::vector<std::shared_ptr<Skills>> GetSkillList();

		//////////////////////////SET FUNCTIONS//////////////////////////////////
	public:

		// Reset the best move found
		void ResetMove();

	private:
		// Set the previous move to the current move
		void SetPrevMove();

		// Reset the deciding moves that help decide the best move
		void ResetDecidingMoves();
};