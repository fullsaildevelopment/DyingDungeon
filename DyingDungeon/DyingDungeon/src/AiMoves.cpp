#pragma once
#include "AiMoves.h"
#include "Character.h"
#include "Attack.h"
#include "Buffs.h"
#include "Provoked.h"
#include "Bleed.h"
#include "Regens.h"
#include "StatDown.h"
#include "StatUp.h"
#include "Stun.h"
#include "Shields.h"

AIMoves::AIMoves()
{
	mCurrMoveCheck = 0;
	mBestMove = nullptr;
	mCaster = nullptr;
	mEnemyID = -1;
	mPriorityMove = GameplayTypes::SKILLTYPE::NONE;
	ResetDecidingMoves();
}

AIMoves::AIMoves(int _enemyID, Character* _caster)
{
	// Priority Set To None
	mPriorityMove = GameplayTypes::SKILLTYPE::NONE;
	
	// Best Move Initialization
	mBestMove = std::make_shared<AIMoves::Move>();
	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;

	// Previous Move Initialization
	mPrevMove.score = -10000;
	mPrevMove.target = nullptr;
	mPrevMove.skill = nullptr;

	// Initizalize all the deciding moves to default moves
	ResetDecidingMoves();

	//My ID and Who I am
	mEnemyID = _enemyID;
	mCaster = _caster;
	
	mCurrMoveCheck = 0;
	std::shared_ptr<StatusEffect> StatusEffect;

	// Setup the enemy type
	switch (_enemyID)
	{
		// Skeleton
		case 0:
		{
			// Skill 1
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Skill_1", 0.25f, -15.0f, 15.0f));
			mSkillList[0]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_1.png");
			// Skill 2
			StatusEffect = std::make_shared<Bleed>(0.15f,2,nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"Skelator Slash", "Skill_2", 0.25f, 10.0f, 25.0f, StatusEffect));
			mSkillList[1]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_2.png");
			// Skill 3
			StatusEffect = std::make_shared < StatDown>(0.5f, 3, STATS::Atk, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"Necrotic Infection", "Skill_3", 0.25f, 40.0f, 30.0f, true));
			mSkillList[2]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_3.png");
			break;
		}
		// Ganfaul
		case 1:
		{
			// Skill 1
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Skill_1", 0.25f, -15.0f, 25.0f));
			mSkillList[0]->SetSkillIconPath(L"assets/images/GanfaulAbilities/Ganfaul_Skill_1.png");
			// Skill 2
			StatusEffect = std::make_shared<StatDown>(0.5f, 3, STATS::Def, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"AOE Def Dwn", "Skill_1", 0.25f, 20.0f, 20.0f, StatusEffect, true));
			mSkillList[1]->SetSkillIconPath(L"assets/images/GanfaulAbilities/Ganfaul_Skill_2.png");
			// Skill 3
			mSkillList.push_back(std::make_shared<Attack>(L"Big Smack", "Skill_1", 0.25f, 50.0f, 45.0f));
			mSkillList[2]->SetSkillIconPath(L"assets/images/GanfaulAbilities/Ganfaul_Skill_3.png");
			// Skill 4
			StatusEffect = std::make_shared<StatUp>(0.50f,2,STATS::Atk, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>(L"Attack Up", "Skill_1", 0.25f, 20.0f, StatusEffect, true));
			mSkillList[3]->SetSkillIconPath(L"assets/images/GanfaulAbilities/Ganfaul_Skill_4.png");
			// Skill 5
			StatusEffect = std::make_shared<Regens>(0.15f, 5, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>(L"Regen", "Skill_1",0.15f, 10.0f, StatusEffect,true));
			mSkillList[4]->SetSkillIconPath(L"assets/images/GanfaulAbilities/Ganfaul_Skill_5.png");
			break;
		}
		// Summoner
		case 2:
		{
			// TODO MAKE REAL MOVES
			// Skill 1
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Skill_1", 0.25f, -20.0f, 15.0f));
			mSkillList[0]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_1.png");
			// Skill 2
			//mSkillList.push_back(std::make_shared<Attack>(L"Summon Caster", "Skill_2", 0.25f, 10.0f, 25.0f, StatusEffect));
			//mSkillList[1]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_2.png");
			// Skill 3
			//mSkillList.push_back(std::make_shared<Attack>(L"Summon Melee", "Skill_3", 0.25f, 40.0f, 30.0f, true));
			//mSkillList[2]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_3.png");
			break;
		}
		// MeleeDemon
		case 3:
		{
			// Skill 1
			StatusEffect = std::make_shared<Provoked>(1, mCaster, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Skill_1", 0.25f, -15.0f, 10.0f, StatusEffect));
			mSkillList[0]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_1.png");
			// Skill 2 
			StatusEffect = std::make_shared<StatDown>(0.35f, 3, STATS::Def, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>(L"Def Down", "Skill_2", 0.25f, 15.0f, StatusEffect, false));
			mSkillList[1]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_2.png");
			// Skill 3 
			mSkillList.push_back(std::make_shared<Attack>(L"mod hit", "Skill_3", 0.25f, 25.0f, 30.0f, true));
			mSkillList[2]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_3.png");
			break;
		}
		// CasterDemon
		case 4:
		{
			// Skill 1 
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Skill_1", 0.25f, -15.0f, 10.0f, true));
			mSkillList[0]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_1.png");
			// Skill 2
			StatusEffect = std::make_shared<StatDown>(0.5f, 3, STATS::Spd, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"AOE Speed dwn", "Skill_2", 0.25f, 10.0f, 15.0f, StatusEffect, true));
			mSkillList[1]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_2.png");
			// Skill 3
			StatusEffect = std::make_shared<Shields>(25.0f, 3, nullptr);
			mSkillList.push_back(std::make_shared<Buffs>(L"Shield", "Skill_3", 0.89f, 20.0f, StatusEffect, true));
			mSkillList[1]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_3.png");
			break;
		}
		// Enemy Mage
		case 5:
		{
			// Skill 1
			mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "Skill_1", 0.25f, -15.0f, 10.0f));
			mSkillList[0]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_1.png");
			// Skill 2
			StatusEffect = std::make_shared<Bleed>(0.15f, 2, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"Skelator Slash", "Skill_2", 0.25f, 10.0f, 25.0f, StatusEffect));
			mSkillList[1]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_2.png");
			// Skill 3
			StatusEffect = std::make_shared<StatDown>(0.5f, 3, STATS::Atk, nullptr);
			mSkillList.push_back(std::make_shared<Attack>(L"Attack down", "Skill_3", 0.25f, 40.0f, 30.0f, true));
			mSkillList[2]->SetSkillIconPath(L"assets/images/SkeletonAbilities/Skeleton_Skill_3.png");
			break;
		}
		default:
		{
			// What did you do to break this?
			break;
		}
	}
}

//Find the best move depending on what enemy you are
bool AIMoves::FindMove(GameplayTypes::SKILLTYPE priorityOverride, std::vector<Odyssey::Entity*> playerTeam, std::vector<Odyssey::Entity*> enemyTeam)
{
	// Our priority can be overriden here
	mPriorityMove = priorityOverride;
	bool finished = false;

	//Determine Enemy Type
	switch (mEnemyID)
	{
		// Skeleton
		case 0:
		{
			//FindBestMove
			if (mPriorityMove == GameplayTypes::SKILLTYPE::NONE)
				SkeletonDeterminePriority();
			
			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		};
		// Ganfaul
		case 1:
		{
			GanfaulDeterminePriority();
			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		}
		// 
		case 2:
		{
			//FindBestMove
			if (mPriorityMove == GameplayTypes::SKILLTYPE::NONE)
				SkeletonDeterminePriority();

			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		}
		// 
		case 3:
		{
			//FindBestMove
			if (mPriorityMove == GameplayTypes::SKILLTYPE::NONE)
				SkeletonDeterminePriority();

			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		}
		// 
		case 4:
		{
			//FindBestMove
			if (mPriorityMove == GameplayTypes::SKILLTYPE::NONE)
				SkeletonDeterminePriority();

			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		}
		// 
		case 5:
		{
			//FindBestMove
			if (mPriorityMove == GameplayTypes::SKILLTYPE::NONE)
				SkeletonDeterminePriority();

			finished = SkillCheck(playerTeam, enemyTeam);
			break;
		}
		default:
		{
			break;
		};
	}
	
	// Return if we finished or not
	return finished;
}

// Iterrate through all the skills one at a time then break
// once done decide the best move among the best moves found.
bool AIMoves::SkillCheck(std::vector<Odyssey::Entity*> playerTeam, std::vector<Odyssey::Entity*> enemyTeam)
{
	bool finished = false;

	// Iterrate through skills
	for (; mCurrMoveCheck < mSkillList.size();)
	{
		// Score the move
		ScoreMove(mSkillList[mCurrMoveCheck], playerTeam, enemyTeam);

		// Have we finished
		if (mCurrMoveCheck >= mSkillList.size() - 1)
		{
			mCurrMoveCheck = 0;
			finished = true;
			break;
		}

		mCurrMoveCheck++;
		break;
	}

	// If we have finished pick a move
	if (finished)
	{
		// Pick a move depending on our priority
		switch (mPriorityMove)
		{
			case GameplayTypes::SKILLTYPE::ATTACK:
			{
				mBestMove->score = mBestAttack.score;
				mBestMove->target = mBestAttack.target;
				mBestMove->skill = mBestAttack.skill;
				break;
			}
			case GameplayTypes::SKILLTYPE::HEAL:
			{
				mBestMove->score = mBestHeal.score;
				mBestMove->target = mBestHeal.target;
				mBestMove->skill = mBestHeal.skill;
				break;
			}
			case GameplayTypes::SKILLTYPE::BUFF:
			{
				mBestMove->score = mBestBuff.score;
				mBestMove->target = mBestBuff.target;
				mBestMove->skill = mBestBuff.skill;
				break;
			}
			case GameplayTypes::SKILLTYPE::DEBUFF:
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

	// Return if we finished or not
	return finished;
}

void AIMoves::ScoreMove(std::shared_ptr<Skills> skill, std::vector<Odyssey::Entity*> playerTeam, std::vector<Odyssey::Entity*> enemyTeam)
{	
	//Do we have enough mana to actually use the skill
	if (skill->GetManaCost() > mCaster->GetMana())
		return;

	// Determine how we will score the current skill
	switch (skill->GetSkillTypeId())
	{
		case GameplayTypes::SKILLTYPE::ATTACK:
		{
			ScoreMoveAttack(skill, playerTeam);
			break;
		}
		case GameplayTypes::SKILLTYPE::HEAL:
		{
			
			break;
		}
		case GameplayTypes::SKILLTYPE::BUFF:
		{
			ScoreMoveBuff(skill, enemyTeam);
			break;
		}
		case GameplayTypes::SKILLTYPE::DEBUFF:
		{
			
			break;
		}
		default:
		{
			break;
		}
	}
}

void AIMoves::ScoreMoveAttack(std::shared_ptr<Skills> skill, std::vector<Odyssey::Entity*> playerTeam)
{
	float attackScore = 0.0f;
	Character* target = nullptr;

	// Swap to aoe scoring
	if (skill->IsAOE())
		ScoreMoveAttackAOE(skill, playerTeam);
	else
	{
		// If we are not provoked then continue
		if (mCaster->GetProvoked() == nullptr)
		for (Odyssey::Entity* t : playerTeam)
		{
			if (t)
			{
				if (target = t->getComponent<Character>())
				{
					// Make sure the target is a hero and that they are not dead
					if (target->IsHero() == true && target->GetState() != STATE::DEAD)
					{
						// If this is our priority then increase the score
						if (skill->GetSkillTypeId() == mPriorityMove)
							attackScore += 100.0f;
						// Determine score based upon attack
						Attack* tempATK = dynamic_cast<Attack*>(skill.get());
						if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
							attackScore += 1000.0f;
						else
							attackScore += tempATK->GetDamage();

						// Score Status Effects
						attackScore += StatusEffectScore(skill, target);

						// Do we beat our previous attack score
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
		// If we are provoked
		else
		{
			if (mCaster->GetProvoked()->GetState() != STATE::DEAD)
			{

				// Is this our skilltype priority
				if (skill->GetSkillTypeId() == mPriorityMove)
					attackScore += 100.0f;

				// Make our target the provoker
				target = mCaster->GetProvoked();

				// Get the damage and add it to our score
				Attack* tempATK = dynamic_cast<Attack*>(skill.get());
				// See if the skill will kill the target
				if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
					attackScore += 1000.0f;
				else
					attackScore += tempATK->GetDamage();

				// Add any status effect scores
				attackScore += StatusEffectScore(skill, target);

				// Do we beat our previous attack score
				if (attackScore >= mBestAttack.score)
				{
					mBestAttack.skill = skill;
					mBestAttack.target = mCaster->GetProvoked();
					mBestAttack.score = attackScore;
				}

				// Reset values
				attackScore = 0.0f;
				target = nullptr;
			}
		}
	}
}

// Score aoe attacks
void AIMoves::ScoreMoveAttackAOE(std::shared_ptr<Skills> skill, std::vector<Odyssey::Entity*> playerTeam)
{
	float attackAOEScore = 0;
	Character* currTarget = nullptr;

	if (mPrevMove.skill != nullptr && mPrevMove.skill->IsAOE())
		return;

	// Is the priority the same as our skill 
	if (skill->GetSkillTypeId() == mPriorityMove)
		attackAOEScore += 100.0f;
	
	// loop through all targets
	for (Odyssey::Entity* t : playerTeam)
	{
		if (t)
		{
			if (currTarget = t->getComponent<Character>())
			{
				// Make sure the target is a hero and not dead
				if (currTarget->IsHero() == true && currTarget->GetState() != STATE::DEAD)
				{
					// Downcast
					Attack* tempATK = dynamic_cast<Attack*>(skill.get());
					// See if the skill will kill the target
					if (currTarget->GetHP() - tempATK->GetDamage() <= 0.0f)
						attackAOEScore += 1000.0f;
					else
						attackAOEScore += tempATK->GetDamage();

					// Add to our aoe score
					attackAOEScore += StatusEffectScore(skill, currTarget);

					// Did we beat our previous score
					if (attackAOEScore >= mBestAttack.score)
					{
						mBestAttack.skill = skill;
						mBestAttack.target = currTarget;
						mBestAttack.score = attackAOEScore;
					}
				}
			}
		}
	}

	
}

// Score single target buffs
void AIMoves::ScoreMoveBuff(std::shared_ptr<Skills> skill, std::vector<Odyssey::Entity*> enemyTeam)
{
	float buffScore = 0.0f;
	Character* target = nullptr;

	// Did we not use a buff
	if (mPrevMove.skill != nullptr && mPrevMove.skill->GetSkillTypeId() != GameplayTypes::SKILLTYPE::BUFF)
		buffScore += 50.0f;

	for (Odyssey::Entity* t : enemyTeam)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				if (target->IsHero() == false && target->GetState() != STATE::DEAD)
				{
					if (target != mCaster)
						buffScore += 20.0f;

					// Add any status effect scores
					buffScore += StatusEffectScore(skill, target);

					// Did we beat our previous buff score
					if (buffScore > mBestBuff.score)
					{
						mBestBuff.score = buffScore;
						mBestBuff.skill = skill;
						mBestBuff.target = target;
					}


					// Reset Scores
					buffScore = 0.0f;
					target = nullptr;

					if (mPrevMove.skill != nullptr && mPrevMove.skill->GetSkillTypeId() != GameplayTypes::SKILLTYPE::BUFF)
						buffScore += 50.0f;
				}
			}
		}
	}
	
}

// Score aoe buffs
void AIMoves::ScoreMoveBuffAOE(std::shared_ptr<Skills> skill, std::vector<Odyssey::Entity*> enemyTeam)
{
	float buffAOEScore = 0.0f;
	Character* target = nullptr;

	// Didn't previouly use a buff
	if (mPrevMove.skill != nullptr && mPrevMove.skill->GetSkillTypeId() != GameplayTypes::SKILLTYPE::BUFF)
		buffAOEScore += 50.0f;

	// Loop through all friendlies
	for (Odyssey::Entity* t : enemyTeam)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				// Make sure were not buffing a hero and that our friendly is not dead
				if (target->IsHero() == false && target->GetState() != STATE::DEAD)
				{
					if (target != mCaster)
						buffAOEScore += 20.0f;

					buffAOEScore += StatusEffectScore(skill, target);

					// Do we beat our previous buff score
					if (buffAOEScore > mBestBuff.score)
					{
						mBestBuff.score = buffAOEScore;
						mBestBuff.skill = skill;
						mBestBuff.target = target;
					}
				}
			}
		}
	}
}

// Score status effects if any
float AIMoves::StatusEffectScore(std::shared_ptr<Skills> skill, Character* target)
{
	float statScore = 0.0f;

	if (skill->GetStatusEffect() != nullptr)
	{
		StatusEffect* statEffect = skill->GetStatusEffect();
		EFFECTTYPE type = skill->GetStatusEffect()->GetTypeId();

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
			// Downcast to get the stat effected
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
			// Downcast to get the stat effected
			StatDown* temp = dynamic_cast<StatDown*>(statEffect);
			switch (temp->GetStatId())
			{
			case STATS::Atk:
			{
				if (target->GetAtk() >= 0.0f)
					statScore += 20.0f;
				break;
			}
			case STATS::Def:
			{
				if (target->GetDef() >= target->GetBaseDef())
					statScore += 20.0f;
				break;
			}
			case STATS::Spd:
			{
				if (target->GetSpeed() >= target->GetBaseSpeed())
					statScore += 20.0f;
				if (target == mCaster)
					statScore += 10.0f;

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

	return statScore;
}

// Determine skeleton's move priority
void AIMoves::SkeletonDeterminePriority()
{
	// If I'm provoked then attack them
	if (mCaster->GetProvoked() != nullptr)
		mPriorityMove = GameplayTypes::SKILLTYPE::ATTACK;

	// If a priority was not set then find a priority
	if (mPriorityMove == GameplayTypes::SKILLTYPE::NONE)
	{
		mPriorityMove = GameplayTypes::SKILLTYPE::ATTACK;
	}
}

// Just attack already
void AIMoves::UngaAttackDeterminePriority()
{
	mPriorityMove = GameplayTypes::SKILLTYPE::ATTACK;
}

// Determine ganfaul's move priority
void AIMoves::GanfaulDeterminePriority()
{
	mPriorityMove = GameplayTypes::SKILLTYPE::ATTACK;
}

///////GET FUNCTIONS///////

// Get the best move found
std::shared_ptr<AIMoves::Move> AIMoves::GetMove()
{
	return mBestMove;
}

// Get our skills
std::vector<std::shared_ptr<Skills>> AIMoves::GetSkillList()
{
	return mSkillList;
}

///////SET FUNCTIONS///////

// Reset the best move
void AIMoves::ResetMove()
{
	SetPrevMove();
	ResetDecidingMoves();

	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;
}

// Set the previous move to the current best move
void AIMoves::SetPrevMove()
{
	mPrevMove.skill = mBestMove->skill;
	mPrevMove.target = mBestMove->target;
	mPrevMove.score = mBestMove->score;
}


// Reset all the deciding moves
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