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
		//Constructors
		AIMoves();
		AIMoves(int _enemyID, Character* _caster);

		//Deconstructor
		~AIMoves() = default;

		// Called outside to find the best move. Return true if finished.
		bool FindMove(SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	private:
		// Iterrates through skills calling respective score move. Returns 
		bool SkillCheck(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		
		// Calls correct score move functions depending on enemy type
		void ScoreMove(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		
		// Score Move For Attacks
		void ScoreMoveAttack(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);
		void ScoreMoveAttackAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam);

		// Score Move For Buffs
		void ScoreMoveBuff(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
		void ScoreMoveBuffAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

		// Score Move For Status Effects
		float StatusEffectScore(std::shared_ptr<Skills> skill, Character* target);

		// Resets the best attack, buff, debuff, heal move.
		void ResetDecidingMoves();

		// Determins skeletons priority reguarding skill move type
		void SkeletonDeterminePriority();
		
		// Makes anyone that calls this always attack
		void UngaAttackDeterminePriority();

		// Determins ganfauls priority reguarding skill move type
		void GanfaulDeterminePriority();
		
		// Enemy Skill List
		std::vector<std::shared_ptr<Skills>> mSkillList;

		// Best Target
		std::vector<Odyssey::Entity> mBestTarget;

		// Best Overall Move
		std::shared_ptr<AIMoves::Move> mBestMove;

		// Best Attack Move
		AIMoves::Move mBestAttack;

		// Best Buff Move
		AIMoves::Move mBestBuff;

		// Best Debuff Move
		AIMoves::Move mBestDebuff;

		// Best Heal Move
		AIMoves::Move mBestHeal;

		// Previous Best Move
		AIMoves::Move mPrevMove;
		
		// Move Priority
		SKILLTYPE mPriorityMove;

		// Who this is attatched too
		Character* mCaster;

		// Current skill index for finding best move
		int mCurrMoveCheck;

		// Type Of Enemy
		int mEnemyID;

		//////////////////////////GET FUNCTIONS//////////////////////////////////
	public:
		// Get the best move
		std::shared_ptr <AIMoves::Move> GetMove();
		// Get the skill enemy skill list
		std::vector<std::shared_ptr<Skills>> GetSkillList();

		//////////////////////////SET FUNCTIONS//////////////////////////////////
	public:
		// Reset All Moves
		void ResetMove();

	private:
		// Sets the previous move
		void SetPrevMove();
};