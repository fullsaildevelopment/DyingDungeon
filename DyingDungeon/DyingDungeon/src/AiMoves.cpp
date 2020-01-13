#pragma once
#include "AiMoves.h"
#include "Character.h"
//////
#include "Attack.h"
#include "Buffs.h"
//////
#include "Provoked.h"
#include "Bleed.h"
#include "Regens.h"
#include "StatDown.h"
#include "StatUp.h"

AIMoves::AIMoves()
{
	mCurrMoveCheck = 0;
	mBestMove = nullptr;
	caster = nullptr;
	mEnemyID = -1;
	mPriorityMove = SKILLTYPE::UNDEFINED;
}

AIMoves::AIMoves(int _enemyID, Character* _caster)
{
	//Priority Set To None
	mPriorityMove = SKILLTYPE::UNDEFINED;
	
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
			//Setup Moves
			// Debuff to slow speed
			debuff = std::make_shared<StatDown>(10.0f,2,STATS::Spd,nullptr);
			// Basic attack. mod dps, spd down
			mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "BasicAttackButBetter", 0.25f, -15.0f, 10.0f, debuff));
			//Bleed for dot
			debuff = std::make_shared<Bleed>(0.15f,2,nullptr);
			// Skelator slash for big dps, inflicts bleed
			mSkillList.push_back(std::make_shared<Attack>("Skelator Slash", "SpinKick", 0.25f, 10.0f, 25.0f, debuff));
			// Debuff to lower attack
			debuff = std::make_shared<StatDown>(0.10f, 2, STATS::Atk, nullptr);
			// Necrotic Infection big aoe dps, atk dwn
			mSkillList.push_back(std::make_shared<Attack>("Necrotic Infection", "FwdKick", 0.25f, 40.0f, 20.0f, debuff, true));
			break;
		}
		default:
		{
			break;
		}
	}
}

bool AIMoves::FindMove(SKILLTYPE priorityOverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	mPriorityMove = priorityOverride;
	bool finished = false;

	//Determine Enemy Type
	switch (mEnemyID)
	{
		case 0:
		{
			//FindBestMove
			if (mPriorityMove == SKILLTYPE::UNDEFINED)
				SkeletonDeterminePriority();
			break;
		};
		default:
		{
			break;
		};
	}
	
	finished = FindBestMove(playerTeam, enemyTeam);
	
	return finished;
}

//TODO:: ALLOW FOR TARGETING YOURSELF (HEALING/BUFFS) (BRYCE YOU SMALL BRAIN)
bool AIMoves::FindBestMove(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	bool finished = false;
	Character* currTarget = nullptr;
	
	if (caster->GetProvoked() == nullptr)
		for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
		{
			if (t)
			{
				if (currTarget = t->getComponent<Character>())
				{
					if (currTarget->IsHero() == true && currTarget->GetState() != STATE::DEAD)
					{
						break;
					}
				}
			}
		}
	else
		currTarget = caster->GetProvoked();

	if (currTarget != nullptr && currTarget->GetState() != STATE::DEAD)
		for (; mCurrMoveCheck < mSkillList.size();)
		{
			float score = ScoreMove(mSkillList[mCurrMoveCheck], currTarget, playerTeam);

			std::cout << mSkillList[mCurrMoveCheck]->GetName() << " scored: " << score << std::endl;

			if (score >= mBestMove->score)
			{
				mBestMove->skill = mSkillList[mCurrMoveCheck];
				mBestMove->target = currTarget;
				mBestMove->score = score;
			}

			if (mCurrMoveCheck >= mSkillList.size() - 1)
			{
				mCurrMoveCheck = 0;
				finished = true;
				break;
			}

			mCurrMoveCheck++;
			break;
		}
	else
	{
		ResetMove();
		finished = true;
	}

	return finished;
}

float AIMoves::ScoreMove(std::shared_ptr<Skills> skill, Character* target, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam)
{
	float score = 0.0f;
	
	//Do we have enough mana to actually use the skill
	if (skill->GetManaCost() > caster->GetMana())
		return -10000.0f;

	if (skill->GetTypeId() == mPriorityMove)
		score += 100.0f;

	if (skill->IsAOE())
		return ScoreMoveAOE(skill, playerTeam) + score;

	if (skill->GetTypeId() == SKILLTYPE::ATTACK)
	{
		Attack* tempATK = dynamic_cast<Attack*>(skill.get());
		if (target->GetHP() - tempATK->GetDamage() <= 0.0f)
			return 1000.0f;
		else
			score += tempATK->GetDamage();
	}

	//Actually Do Stuff
	if (skill->GetStatusEffect() != nullptr)
	{
		StatusEffect* statEffect = skill->GetStatusEffect();
		EFFECTTYPE type = skill->GetStatusEffect()->GetTypeId();

		switch (type)
		{
			case EFFECTTYPE::Bleed:
			{
				float totalDamage = target->GetMaxHP() * statEffect->GetAmountOfEffect() * statEffect->GetDuration();
				score += totalDamage * 0.35f;
				break;
			}
			case EFFECTTYPE::Regen:
			{
				float totalHeal = target->GetMaxHP() * statEffect->GetAmountOfEffect() * statEffect->GetDuration();
				score += totalHeal * 0.35f;
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
							score += 20.0f;
						break;
					}
					case STATS::Def:
					{
						if (target->GetDef() <= target->GetBaseDef())
							score += 20.0f;
						break;
					}
					case STATS::Spd:
					{
						if (target->GetSpeed() <= target->GetBaseSpeed())
							score += 20.0f;
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
							score += 20.0f;
						break;
					}
					case STATS::Def:
					{
						if (target->GetDef() >= target->GetBaseDef())
							score += 20.0f;
						break;
					}
					case STATS::Spd:
					{
						if (target->GetSpeed() >= target->GetBaseSpeed())
							score += 20.0f;
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
				score += 30.0f * statEffect->GetDuration();
				break;
			}
			case EFFECTTYPE::Shield:
			{
				score += statEffect->GetAmountOfEffect();
				break;
			}
			case EFFECTTYPE::Provoke:
			{
				score += statEffect->GetDuration() * 10.0f;
				break;
			}
			default:
			{
				//why you  no work
				break;
			}
		}
	}

	return score;
}

float AIMoves::ScoreMoveAOE(std::shared_ptr<Skills> skill, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam)
{
	float score2 = 0;
	Character* currTarget = nullptr;

	if (skill->GetManaCost() > caster->GetMana())
		return -10000;

	
	for (std::shared_ptr<Odyssey::Entity> t : playerTeam)
	{
		if (t)
		{
			if (currTarget = t->getComponent<Character>())
			{
				if (currTarget->IsHero() == true && currTarget->GetState() != STATE::DEAD)
				{
					if (skill->GetTypeId() == SKILLTYPE::ATTACK)
					{
						Attack* tempATK = dynamic_cast<Attack*>(skill.get());
						if (currTarget->GetHP() - tempATK->GetDamage() <= 0.0f)
							score2 += 1000.0f;
						else
							score2 += tempATK->GetDamage();
					}

					if (skill->GetStatusEffect() != nullptr)
					{
						StatusEffect* statEffect = skill->GetStatusEffect();
						EFFECTTYPE type = skill->GetStatusEffect()->GetTypeId();

						switch (type)
						{
						case EFFECTTYPE::Bleed:
						{
							float totalDamage = currTarget->GetMaxHP() * statEffect->GetAmountOfEffect() * statEffect->GetDuration();
							score2 += totalDamage * 0.35f;
							break;
						}
						case EFFECTTYPE::Regen:
						{
							float totalHeal = currTarget->GetMaxHP() * statEffect->GetAmountOfEffect() * statEffect->GetDuration();
							score2 += totalHeal * 0.35f;
							break;
						}
						case EFFECTTYPE::StatUp:
						{
							StatUp* temp = dynamic_cast<StatUp*>(statEffect);
							switch (temp->GetStatId())
							{
							case STATS::Atk:
							{
								if (currTarget->GetAtk() <= 0.0f)
									score2 += 20.0f;
								break;
							}
							case STATS::Def:
							{
								if (currTarget->GetDef() <= currTarget->GetBaseDef())
									score2 += 20.0f;
								break;
							}
							case STATS::Spd:
							{
								if (currTarget->GetSpeed() <= currTarget->GetBaseSpeed())
									score2 += 20.0f;
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
								if (currTarget->GetAtk() >= 0.0f)
									score2 += 20.0f;
								break;
							}
							case STATS::Def:
							{
								if (currTarget->GetDef() >= currTarget->GetBaseDef())
									score2 += 20.0f;
								break;
							}
							case STATS::Spd:
							{
								if (currTarget->GetSpeed() >= currTarget->GetBaseSpeed())
									score2 += 20.0f;
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
							score2 += 30.0f * statEffect->GetDuration();
							break;
						}
						case EFFECTTYPE::Shield:
						{
							score2 += statEffect->GetAmountOfEffect();
							break;
						}
						case EFFECTTYPE::Provoke:
						{
							score2 += statEffect->GetDuration() * 10.0f;
							break;
						}
						default:
						{
							//why you  no work
							break;
						}
						}

					}
				}
			}
		}
	}
	return score2;
}

void AIMoves::SkeletonDeterminePriority()
{
	mPriorityMove = SKILLTYPE::ATTACK;
}

void AIMoves::UngaAttackDeterminePriority()
{
	mPriorityMove = SKILLTYPE::ATTACK;
}

///////GET FUNCTIONS///////

std::shared_ptr<AIMoves::Move> AIMoves::GetMove()
{
	return mBestMove;
}

///////SET FUNCTIONS///////

void AIMoves::ResetMove()
{
	SetPrevMove();

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