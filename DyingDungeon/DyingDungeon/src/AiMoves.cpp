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

AIMoves::AIMoves()
{
	mCurrMoveCheck = 0;
	mBestMove = nullptr;
	caster = nullptr;
	mEnemyID = -1;
	mPriorityMove = -1;
}

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

	//My ID and Who I am
	mEnemyID = _enemyID;
	caster = _caster;
	
	mCurrMoveCheck = 0;
	std::shared_ptr<StatusEffect> debuff;
	switch (_enemyID)
	{
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
		case 0:
		{
			//FindBestMove
			if (mPriorityMove == -1)
				SkeletonDeterminePriority();
			
			finished = FindBestMove(playerTeam, enemyTeam);
			break;
		};
		case 1:
		{
			BossDeterminePriority();
			finished = FindBestMove(playerTeam, enemyTeam);
			break;
		}
		default:
		{
			break;
		};
	}
	
	
	return finished;
}

bool AIMoves::FindBestMove(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	bool finished = false;

	for (; mCurrMoveCheck < mSkillList.size();)
	{
		float score = ScoreMove(mSkillList[mCurrMoveCheck], playerTeam, enemyTeam);

		if (mCurrMoveCheck >= mSkillList.size() - 1)
		{
			mCurrMoveCheck = 0;
			finished = true;
			break;
		}

		mCurrMoveCheck++;
		break;
	}

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

	return finished;
}

float AIMoves::ScoreMove(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	float score = 0.0f;
	
	//Do we have enough mana to actually use the skill
	if (skill->GetManaCost() > caster->GetMana())
		return -10000.0f;

	// Determine how we will score the current skill
	int type = int(skill->GetSkillTypeId());
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
			break;
		}
	}

	return score;
}

void AIMoves::ScoreMoveAttack(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam)
{
	float attackScore = 0.0f;
	Character* target = nullptr;

	if (skill->IsAOE())
		ScoreMoveAttackAOE(skill, playerTeam);
	else
	{
		if (caster->GetProvoked() == nullptr)
		for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
		{
			if (t)
			{
				if (target = t->getComponent<Character>())
				{
					if (target->IsHero() == true && target->GetState() != STATE::DEAD)
					{
						// If this is our priority then increase the score
						if (int(skill->GetSkillTypeId()) == mPriorityMove)
							attackScore += 100.0f;

						Attack* tempATK = dynamic_cast<Attack*>(skill.get());
						if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
							attackScore += 1000.0f;
						else
							attackScore += tempATK->GetDamage();

						attackScore += StatusEffectScore(skill, target);

						//std::cout << skill->GetName() << " scored: " << attackScore << std::endl;

						if (attackScore > mBestAttack.score)
						{
							mBestAttack.skill = skill;
							mBestAttack.target = target;
							mBestAttack.score = attackScore;

							//std::cout << "BEST ATTACK CHANGED" << std::endl;
						}

						target = nullptr;
						attackScore = 0.0f;
					}
				}
			}
		}
		else
		{
			// Is this our skilltype priority
			if (int(skill->GetSkillTypeId()) == mPriorityMove)
				attackScore += 100.0f;

			target = caster->GetProvoked();

			Attack* tempATK = dynamic_cast<Attack*>(skill.get());
			if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
				attackScore += 1000.0f;
			else
				attackScore += tempATK->GetDamage();

			attackScore += StatusEffectScore(skill, target);


			if (attackScore >= mBestAttack.score)
			{
				mBestAttack.skill = skill;
				mBestAttack.target = caster->GetProvoked();
				mBestAttack.score = attackScore;
			}

			attackScore = 0.0f;
			target = nullptr;
		}
	}
}

void AIMoves::ScoreMoveAttackAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam)
{
	float attackAOEScore = 0;
	Character* currTarget = nullptr;

	if (mPrevMove.skill != nullptr && mPrevMove.skill->IsAOE())
		return;

	// Is the priority the same as our skill 
	if (int(skill->GetSkillTypeId()) == mPriorityMove)
		attackAOEScore += 100.0f;
	
	for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
	{
		if (t)
		{
			if (currTarget = t->getComponent<Character>())
			{
				if (currTarget->IsHero() == true && currTarget->GetState() != STATE::DEAD)
				{
					Attack* tempATK = dynamic_cast<Attack*>(skill.get());
					if (currTarget->GetHP() - tempATK->GetDamage() <= 0.0f)
						attackAOEScore += 1000.0f;
					else
						attackAOEScore += tempATK->GetDamage();

					attackAOEScore += StatusEffectScore(skill, currTarget);
				}
			}
		}
	}

	if (attackAOEScore >= mBestAttack.score)
	{
		mBestAttack.skill = skill;
		mBestAttack.target = currTarget;
		mBestAttack.score = attackAOEScore;
	}

	//std::cout << skill->GetName() << " aoe scored " << attackAOEScore << std::endl;
}

void AIMoves::ScoreMoveBuff(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	float buffScore = 0.0f;
	Character* target = nullptr;

	if (mPrevMove.skill != nullptr && mPrevMove.skill->GetSkillTypeId() != SKILLTYPE::BUFF)
		buffScore += 50.0f;

	for (std::shared_ptr<Odyssey::Entity> t : enemyTeam)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				if (target->IsHero() == false && target->GetState() != STATE::DEAD)
				{
					if (target != caster)
						buffScore += 20.0f;

					buffScore += StatusEffectScore(skill, target);

					if (buffScore > mBestBuff.score)
					{
						mBestBuff.score = buffScore;
						mBestBuff.skill = skill;
						mBestBuff.target = target;
					}

					buffScore = 0.0f;
					target = nullptr;
				}
			}
		}
	}
	
}

void AIMoves::ScoreMoveBuffAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	float buffAOEScore = 0.0f;
	Character* target = nullptr;

	if (mPrevMove.skill != nullptr && mPrevMove.skill->GetSkillTypeId() != SKILLTYPE::BUFF)
		buffAOEScore += 50.0f;

	for (std::shared_ptr<Odyssey::Entity> t : enemyTeam)
	{
		if (t)
		{
			if (target = t->getComponent<Character>())
			{
				if (target->IsHero() == false && target->GetState() != STATE::DEAD)
				{
					if (target != caster)
						buffAOEScore += 20.0f;

					buffAOEScore += StatusEffectScore(skill, target);
				}
			}
		}
	}

	if (buffAOEScore > mBestBuff.score)
	{
		mBestBuff.score = buffAOEScore;
		mBestBuff.skill = skill;
		mBestBuff.target = target;
	}
}

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
				if (target == caster)
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

void AIMoves::UngaAttackDeterminePriority()
{
	mPriorityMove = 0;
}

void AIMoves::BossDeterminePriority()
{
	mPriorityMove = 0;
}

///////GET FUNCTIONS///////

std::shared_ptr<AIMoves::Move> AIMoves::GetMove()
{
	return mBestMove;
}

std::vector<std::shared_ptr<Skills>> AIMoves::GetSkillList()
{
	return mSkillList;
}

///////SET FUNCTIONS///////

void AIMoves::ResetMove()
{
	SetPrevMove();
	ResetDecidingMoves();

	mBestMove->skill = nullptr;
	mBestMove->target = nullptr;
	mBestMove->score = -10000;
}

void AIMoves::SetPrevMove()
{
	mPrevMove.skill = mBestMove->skill;
	mPrevMove.target = mBestMove->target;
	mPrevMove.score = mBestMove->score;
}

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