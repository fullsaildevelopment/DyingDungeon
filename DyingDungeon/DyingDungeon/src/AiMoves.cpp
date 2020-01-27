#pragma once
#include "AiMoves.h"
#include "Character.h"
///////
#include "Attack.h"
#include "Buffs.h"
//////
#include "Provoked.h"
#include "Bleed.h"
#include "Regens.h"
#include "StatDown.h"
#include "StatUp.h"
#include "Stun.h"

// Default Constructor
AIMoves::AIMoves()
{
	mCurrMoveCheck = 0;
	mBestMove = nullptr;
	mCaster = nullptr;
	mEnemyID = -1;
	mPriorityMove = -1;
}

// Constructor
AIMoves::AIMoves(int _enemyID, Character* _caster)
{
	//Priority Set To None
	mPriorityMove = -1;
	
	//Best Move Initialization
	mBestMove = std::make_shared<AIMoves::Move>();
	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;

	//Previous Move Initialization
	mPrevMove.score = -10000;
	mPrevMove.target = nullptr;
	mPrevMove.skill = nullptr;

	// Initialize Deciding Moves
	ResetDecidingMoves();

	//My ID and Who I am
	mEnemyID = _enemyID;
	mCaster = _caster;
	
	mCurrMoveCheck = 0;
	std::shared_ptr<StatusEffect> debuff;
	switch (_enemyID)
	{
		//Skeleton
		case 0:
		{
			////Setup Moves
			// Debuff to slow speed
			debuff = std::make_shared<StatDown>(10.0f,2,STATS::Spd,nullptr);
			// Basic attack. mod dps, spd down
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "BasicAttackButBetter", 0.25f, -15.0f, 10.0f, debuff));
			//Bleed for dot
			debuff = std::make_shared<Bleed>(0.15f,2,nullptr);
			// Skelator slash for big dps, inflicts bleed
			mSkillList.push_back(std::make_shared<Attack>(L"Skelator Slash", "SpinKick", 0.25f, 10.0f, 25.0f, debuff));
			// Debuff to lower attack
			debuff = std::make_shared<StatDown>(0.10f, 2, STATS::Atk, nullptr);
			// Necrotic Infection big aoe dps, atk dwn
			mSkillList.push_back(std::make_shared<Attack>(L"Necrotic Infection", "FwdKick", 0.25f, 40.0f, 20.0f, debuff, true));
			//Test Regen
			//debuff = std::make_shared<Regens>(0.15f, 2, nullptr);
			//mSkillList.push_back(std::make_shared<Buffs>("Necrotic Infection", "FwdKick", 0.25f, 50.0f, 0.0f, debuff, true));
			break;
		}
		//Ganfaul
		case 1:
		{
			// Basic Attack, good dps, stun
			debuff = std::make_shared<Stun>(1,nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Attack", 0.25f, -15.0f, 25.0f, debuff));
			//  good dps, spd down ,aoe
			debuff = std::make_shared<StatDown>(0.25f, 4, STATS::Spd, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"AOE Slow", "Attack", 0.25f, 20.0f, 15.0f, debuff, true));
			//  Big smack
			mSkillList.push_back(std::make_shared<Attack>(L"Big Smack", "Attack", 0.25f, 50.0f, 45.0f));
			// Attack buff
			debuff = std::make_shared<StatUp>(0.30f,3,STATS::Atk, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>(L"Attack Up", "Attack", 0.25f, 20.0f, debuff, true));
			// Regen
			debuff = std::make_shared<Regens>(0.15f, 5, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>(L"Regen", "Attack",0.15f, 10.0f, debuff,true));
			break;
		}
		//You broke something
		default:
		{
			break;
		}
	}
}

//Find the best move depending on what enemy you are
bool AIMoves::FindMove(int priorityOverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	mPriorityMove = priorityOverride;
	bool finished = false;

	//Determine Enemy Type
	switch (mEnemyID)
	{
		//Uses enum ENEMYID.
		//Skeleton
		case 0:
		{
			//FindBestMove
			if (mPriorityMove == -1)
				SkeletonDeterminePriority();
			
			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		};
		//Ganfauls
		case 1:
		{
			GanfaulDeterminePriority();
			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		}
		//You broke something
		default:
		{
			break;
		};
	}
	
	// Return if we finished
	return finished;
}

// Iterrate through all skills
bool AIMoves::SkillCheck(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	bool finished = false;

	//Loop through all the skills one at a time
	for (; mCurrMoveCheck < mSkillList.size();)
	{
		// Score the current skill
		ScoreMove(mSkillList[mCurrMoveCheck], playerTeam, enemyTeam);

		// Finished Condition. We have looped through and scored all the skills.
		if (mCurrMoveCheck >= mSkillList.size() - 1)
		{
			mCurrMoveCheck = 0;
			finished = true;
			break;
		}

		mCurrMoveCheck++;
		break;
	}

	// If we have gone through all of our skills determine
	// the best out of the deciding moves based upon our priority.
	if (finished)
	{
		switch (mPriorityMove)
		{
			case 0:
			{
				mBestMove->score = mBestAttack.score;
				mBestMove->target = mBestAttack.target;
				mBestMove->skill = mBestAttack.skill;
				break;
			}
			case 1:
			{
				mBestMove->score = mBestBuff.score;
				mBestMove->target = mBestBuff.target;
				mBestMove->skill = mBestBuff.skill;
				break;
			}
			case 2:
			{
				mBestMove->score = mBestHeal.score;
				mBestMove->target = mBestHeal.target;
				mBestMove->skill = mBestHeal.skill;
				break;
			}
			case 3:
			{
				mBestMove->score = mBestDebuff.score;
				mBestMove->target = mBestDebuff.target;
				mBestMove->skill = mBestDebuff.skill;
				break;
			}
			default:
			{
				//Ya messed up somewhere if you reached this
				break;
			}
		}
	}

	// Return if we have finished or not
	return finished;
}

// Generic score move function that determins which score 
// move function to use in relation to the type of skill.
void AIMoves::ScoreMove(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{	
	//Do we have enough mana to actually use the skill
	if (skill->GetManaCost() > mCaster->GetMana())
		return;


	// Determine how we will score the current skill
	int type = int(skill->GetTypeId());
	switch (type)
	{
		case 0:
		{
			ScoreMoveAttack(skill, playerTeam);
			break;
		}
		case 1:
		{
			ScoreMoveBuff(skill, enemyTeam);
			break;
		}
		case 2:
		{
			
			break;
		}
		case 3:
		{
			
			break;
		}
		default:
		{
			//Yo stop breaking stuff
			break;
		}
	}
}

// Generic score move single target attack
void AIMoves::ScoreMoveAttack(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam)
{
	float attackScore = 0.0f;
	Character* target = nullptr;

	//if the skill is an aoe move then swap how we score.
	if (skill->IsAOE())
		ScoreMoveAttackAOE(skill, playerTeam);
	else
	{
		// If we are not provoked then continue
		if (mCaster->GetProvoked() == nullptr)
		for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
		{
			if (t)
			{
				if (target = t->getComponent<Character>())
				{
					if (target->IsHero() == true && target->GetState() != STATE::DEAD)
					{
						// If this is our priority then increase the score
						if (int(skill->GetTypeId()) == mPriorityMove)
							attackScore += 100.0f;

						// Damage added to the score
						Attack* tempATK = dynamic_cast<Attack*>(skill.get());
						if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
							attackScore += 1000.0f;
						else
							attackScore += tempATK->GetDamage();
						
						// Add status effect score if a status exists to our score
						attackScore += StatusEffectScore(skill, target);

						// If we beat our previous score then set our current move to the attacks best move
						if (attackScore > mBestAttack.score)
						{
							mBestAttack.skill = skill;
							mBestAttack.target = target;
							mBestAttack.score = attackScore;
						}

						target = nullptr;
						attackScore = 0.0f;
					}
				}
			}
		}
		// If we are provoked then set the target to the provoker and continue scoring
		else
		{
			// Is this our skilltype priority
			if (int(skill->GetTypeId()) == mPriorityMove)
				attackScore += 100.0f;

			// Set target to the provoker
			target = mCaster->GetProvoked();

			// Calculate damage
			Attack* tempATK = dynamic_cast<Attack*>(skill.get());
			if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
				attackScore += 1000.0f;
			else
				attackScore += tempATK->GetDamage();

			// Score status effect
			attackScore += StatusEffectScore(skill, target);

			// TODO: TESTING PURPOSES (REMOVE WHEN NOT NEEDED)
			std::cout << skill->GetName().c_str() << " scored: " << attackScore << std::endl;

			// Does our attack score beat our previous score
			if (attackScore >= mBestAttack.score)
			{
				mBestAttack.skill = skill;
				mBestAttack.target = mCaster->GetProvoked();
				mBestAttack.score = attackScore;
			}

			// Reset the score and target
			attackScore = 0.0f;
			target = nullptr;
		}
	}

	// END
}

// Generic score move attack for aoe moves
void AIMoves::ScoreMoveAttackAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam)
{
	float attackAOEScore = 0;
	Character* currTarget = nullptr;

	// If our previous skill is an aoe attack then return
	if (mPrevMove.skill != nullptr && mPrevMove.skill->IsAOE())
		return;

	// Is the priority the same as our skill 
	if (int(skill->GetTypeId()) == mPriorityMove)
		attackAOEScore += 100.0f;
	
	// Loop through all targets adding damage for each to the score
	for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
	{
		if (t)
		{
			if (currTarget = t->getComponent<Character>())
			{
				// Make sure were hitting a hero and hes not dead
				if (currTarget->IsHero() == true && currTarget->GetState() != STATE::DEAD)
				{
					// Calculate damage
					Attack* tempATK = dynamic_cast<Attack*>(skill.get());
					if (currTarget->GetHP() - tempATK->GetDamage() <= 0.0f)
						attackAOEScore += 1000.0f;
					else
						attackAOEScore += tempATK->GetDamage();
					// Add status effect score
					attackAOEScore += StatusEffectScore(skill, currTarget);
				}
			}
		}
	}

	// Do we beat our previous attackScore
	if (attackAOEScore >= mBestAttack.score)
	{
		mBestAttack.skill = skill;
		mBestAttack.target = currTarget;
		mBestAttack.score = attackAOEScore;
	}

	//std::cout << skill->GetName() << " aoe scored " << attackAOEScore << std::endl;
}

// Generic score move for single target buffs
void AIMoves::ScoreMoveBuff(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	float buffScore = 0.0f;
	Character* target = nullptr;

	// Make sure we didn't previously buff someone. 
	if (mPrevMove.skill != nullptr && mPrevMove.skill->GetTypeId() != SKILLTYPE::BUFF)
		buffScore += 50.0f;

	// Loop through all friendlies
	for (std::shared_ptr<Odyssey::Entity> t : enemyTeam)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				if (target->IsHero() == false && target->GetState() != STATE::DEAD)
				{
					// Less score if targeting self since we have to wait till next turn
					if (target != mCaster)
						buffScore += 20.0f;

					// Status effect scoring
					buffScore += StatusEffectScore(skill, target);

					// Does our score beat the best buff score
					if (buffScore > mBestBuff.score)
					{
						mBestBuff.score = buffScore;
						mBestBuff.skill = skill;
						mBestBuff.target = target;
					}

					// Reset score and target
					buffScore = 0.0f;
					target = nullptr;
				}
			}
		}
	}
	
}

// Score move function for aoe buffs
void AIMoves::ScoreMoveBuffAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	float buffAOEScore = 0.0f;
	Character* target = nullptr;

	// Make sure we didn't use an aoe buff before
	if (mPrevMove.skill != nullptr && mPrevMove.skill->GetTypeId() != SKILLTYPE::BUFF)
		buffAOEScore += 50.0f;

	// Loop through all friendlies adding score for each.
	for (std::shared_ptr<Odyssey::Entity> t : enemyTeam)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				// Make sure were not buffing a hero
				if (target->IsHero() == false && target->GetState() != STATE::DEAD)
				{
					// When were not targeting ourself add 20
					if (target != mCaster)
						buffAOEScore += 20.0f;

					// Score the status effect
					buffAOEScore += StatusEffectScore(skill, target);
				}
			}
		}
	}

	// Do we beat our previous buff score
	if (buffAOEScore > mBestBuff.score)
	{
		mBestBuff.score = buffAOEScore;
		mBestBuff.skill = skill;
		mBestBuff.target = target;
	}
}


// Generic score move for status effects
float AIMoves::StatusEffectScore(std::shared_ptr<Skills> skill, Character* target)
{
	float statScore = 0.0f;

	// Don't continue if no status effect exists
	if (skill->GetStatusEffect() != nullptr)
	{
		// Get the status effect
		StatusEffect* statEffect = skill->GetStatusEffect();

		// Determine what type of status effect it is
		EFFECTTYPE type = skill->GetStatusEffect()->GetTypeId();

		// Swap depending on effect type
		switch (type)
		{
		case EFFECTTYPE::Bleed:
		{
			float totalDamage = target->GetMaxHP() * statEffect->GetAmountOfEffect() * statEffect->GetDuration();
			statScore += totalDamage * 0.35f;
			break;
		}
		case EFFECTTYPE::Regen:
		{
			float totalHeal = target->GetMaxHP() * statEffect->GetAmountOfEffect() * statEffect->GetDuration();
			statScore += totalHeal * 0.35f;
			break;
		}
		case EFFECTTYPE::StatUp:
		{
			// Down cast to a stat up
			StatUp* temp = dynamic_cast<StatUp*>(statEffect);
			switch (temp->GetStatId())
			{
			case STATS::Atk:
			{
				if (target->GetAtk() <= 0.0f)
					statScore += 20.0f;
				break;
			}
			case STATS::Def:
			{
				if (target->GetDef() <= target->GetBaseDef())
					statScore += 20.0f;
				break;
			}
			case STATS::Spd:
			{
				if (target->GetSpeed() <= target->GetBaseSpeed())
					statScore += 20.0f;
				break;
			}
			default:
			{
				//Why you no work
				break;
			}
			}
			break;
		}
		case EFFECTTYPE::StatDown:
		{
			// Downcast to a statdown
			StatDown* temp = dynamic_cast<StatDown*>(statEffect);
			switch (temp->GetStatId())
			{
			case STATS::Atk:
			{
				if (target->GetAtk() >= 0.0f)
					statScore -= 20.0f;
				break;
			}
			case STATS::Def:
			{
				if (target->GetDef() >= target->GetBaseDef())
					statScore -= 20.0f;
				break;
			}
			case STATS::Spd:
			{
				if (target->GetSpeed() >= target->GetBaseSpeed())
					statScore -= 20.0f;
				if (target == mCaster)
					statScore -= 10.0f;

				break;
			}
			default:
			{
				//Why you no work
				break;
			}
			}
			break;
		}
		case EFFECTTYPE::Stun:
		{
			statScore += 30.0f * statEffect->GetDuration();
			break;
		}
		case EFFECTTYPE::Shield:
		{
			statScore += statEffect->GetAmountOfEffect();
			break;
		}
		case EFFECTTYPE::Provoke:
		{
			statScore += statEffect->GetDuration() * 10.0f;
			break;
		}
		default:
		{
			//why you  no work
			break;
		}
		}
	}

	// Return our score
	return statScore;
}

// Determin what type of move the skeleton should prioritize
void AIMoves::SkeletonDeterminePriority()
{
	// If I'm provoked then attack them
	if (mCaster->GetProvoked() != nullptr)
		mPriorityMove = 0;

	// If a priority was not set then find a priority
	if (mPriorityMove == -1)
	{
		mPriorityMove = 0;
	}
}

// Always prioritize attacking over everything else
void AIMoves::UngaAttackDeterminePriority()
{
	mPriorityMove = 0;
}

// Determins ganfauls move priority
void AIMoves::GanfaulDeterminePriority()
{
	mPriorityMove = 0;
}

///////GET FUNCTIONS///////

// Returns the best move for the AI
std::shared_ptr<AIMoves::Move> AIMoves::GetMove()
{
	return mBestMove;
}

// Returns the skill list for the enemy
std::vector<std::shared_ptr<Skills>> AIMoves::GetSkillList()
{
	return mSkillList;
}

///////SET FUNCTIONS///////


// Resets every move 
void AIMoves::ResetMove()
{
	SetPrevMove();
	ResetDecidingMoves();

	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;
}

// Sets the previous move to the current best move
void AIMoves::SetPrevMove()
{
	mPrevMove.skill = mBestMove->skill;
	mPrevMove.target = mBestMove->target;
	mPrevMove.score = mBestMove->score;
}

// Resets the skill deciding moves
void AIMoves::ResetDecidingMoves()
{
	mBestAttack.score = 0.0f;
	mBestAttack.skill = nullptr;
	mBestAttack.target = nullptr;

	mBestBuff.score = 0.0f;
	mBestBuff.skill = nullptr;
	mBestBuff.target = nullptr;

	mBestDebuff.score = 0.0f;
	mBestDebuff.skill = nullptr;
	mBestDebuff.target = nullptr;

	mBestHeal.score = 0.0f;
	mBestHeal.skill = nullptr;
	mBestHeal.target = nullptr;
}