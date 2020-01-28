#include "EnemyComponent.h"
#include "Entity.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

EnemyComponent::EnemyComponent(GameplayTypes::ENEMYID _enemyID)
{
	// Setting default values for member variables //
	////////////////////////////////////////////////
	mCurrentState = STATE::NONE;
	mHero = false;
	mEXP = 0.0f;
	mCurrentLevel = 0;
	mProvoked = nullptr;
	mBloodParticleEffect = nullptr;
	mImpactIndicator = nullptr;
	mMechPtr = nullptr;
	mMoveOverride = GameplayTypes::SKILLTYPE::NONE;
	mCurrentState = STATE::NONE;
	mMoves = AIMoves(static_cast<int>(_enemyID), this);
	////////////////////////////////////////////////

	// Make a temp variable to contain animation data
	AnimationImportData tempAnimationData;

	// Switch statment that builds the hero depending on the hero id that gets passed in the constructor
	switch (_enemyID)
	{
	case GameplayTypes::ENEMYID::Skeleton:
	{
		// Set the character Model path
		mModel = "assets/models/Skeleton.dxm";

		// Set the character name
		mName = L"Skeleton";

		// Set the character subname
		mSubName = L"Skelly Boi";

		// Set the portaits path
		mPortrait = L"assets/images/SkeletonPortrait.jpg";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 125.0f;

		// Set the stats for the character //
		////////////////////////////////////
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.15f;
		mBaseSpeed = mSpeed = 20.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff
		tempAnimationData.mAnimationNickName = "GotBuffed";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_Yell.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_SpinKick.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_FwdKick.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_BasicAttack.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4

		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"One Skelly boi, who wants to rattle your bones";
		////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}
	case GameplayTypes::ENEMYID::Ganfaul:
	{
		// Set the character Model path
		mModel = "assets/models/Ganfaul.dxm";

		// Set the character name
		mName = L"Ganfaul";

		// Set the character subname
		mSubName = L"Killer of chickens";

		// Set the portaits path
		mPortrait = L"assets/images/GanfaulPortrait.jpg";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 300.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 300.0f;

		// Set the stats for the character //
		////////////////////////////////////
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.25f;
		mBaseSpeed = mSpeed = 45.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;
		
		// Set mechanic pointer
		mMechPtr = &EnemyComponent::GanfaulPhaseMechanic;

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"The Big Bad Evil Guy who wants to kill your chickens";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Ganfaul_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Ganfaul_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Ganfaul_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Ganfaul_Attack.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2

		// Skill 3

		// Skill 4

		////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	default:
		break;
	}
}

EnemyComponent::~EnemyComponent()
{
}

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	// Find my best option
	switch (mCurrentState)
	{
	case STATE::STUNNED:
	{
		mCurrentState = STATE::NONE;
		ManageAllEffects();
		return true;
	}
	case STATE::NONE:
	{
		if(mMechPtr)
			(this->*mMechPtr)();
		//Lane was here :D
		ManageStatusEffects(mRegens);
		ManageStatusEffects(mBleeds);
		if (mCurrentHP <= 0.0f)
			Die();
		else
			mCurrentState = STATE::SELECTMOVE;
		break;
	}
	case STATE::SELECTMOVE:
	{
		if (mMoves.FindMove(mMoveOverride, playerTeam, enemyTeam) && mMoves.GetMove()->target != nullptr)
		{
			mCurrentState = STATE::INPROGRESS;
			if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
				BeginAttack(playerTeam);
			else
				BeginAttack(enemyTeam);
		}
		break;
	}
	case STATE::INPROGRESS:
	{
		static bool particleTriggerButBetter = false;
		static bool triggerButBetter = false;
		if (mMoves.GetMove()->skill->GetParticleSystem() != nullptr && !particleTriggerButBetter && mAnimator->getProgress() > mMoves.GetMove()->skill->GetPSFiringTime())
		{
			// Turn particle effect on
			mMoves.GetMove()->skill->GetParticleSystem()->play();
			if (!mMoves.GetMove()->skill->IsAOE())
				mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->setActive(true);
			particleTriggerButBetter = true;
		}
		if (!triggerButBetter && mAnimator->getProgress() > mMoves.GetMove()->skill->GetAnimationTiming())
		{
			// Static bool used to track whenever its time to play the recipent animation ie hit or be buffed 
			static bool trigger = false;
			if (!trigger && mAnimator->getProgress() > mMoves.GetMove()->skill->GetAnimationTiming())
			{
				// If its ment for the enemies play the hit animation to time with the animation timing
				if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
				{
					if (mMoves.GetMove()->skill->IsAOE())
					{
						for (std::shared_ptr<Odyssey::Entity> c : playerTeam)
						{
							if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
							{
								c.get()->getComponent<Odyssey::Animator>()->playClip("Hit");
								DirectX::XMFLOAT3 t = c.get()->getComponent<Odyssey::Transform>()->getPosition();
								c.get()->getComponent<Character>()->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);
								c.get()->getComponent<Character>()->GetPSBlood()->play();
							}
						}
					}
					else if (mMoves.GetMove()->target != nullptr)
					{
						mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
						DirectX::XMFLOAT3 t = mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
						mMoves.GetMove()->target->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);
						mMoves.GetMove()->target->GetPSBlood()->play();
					}
				}
				else
				{
					// If its ment for the players characters play the recived buff animation to time with the animation timing
					if (mMoves.GetMove()->skill->IsAOE())
					{
						for (std::shared_ptr<Odyssey::Entity> c : enemyTeam)
						{
							if (c != nullptr && c.get()->getComponent<EnemyComponent>() != this && c->getComponent<Character>()->GetState() != STATE::DEAD)
							{
								c.get()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
								DirectX::XMFLOAT3 t = c.get()->getComponent<Odyssey::Transform>()->getPosition();
								c.get()->getComponent<Character>()->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);
								c.get()->getComponent<Character>()->GetPSBlood()->play();
							}
						}
					}
					else if (mMoves.GetMove()->target != nullptr && mMoves.GetMove()->target != this)
					{
						mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
						DirectX::XMFLOAT3 t = mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
						mMoves.GetMove()->target->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);
						mMoves.GetMove()->target->GetPSBlood()->play();
					}
				}
				// Set trigger to true to avoid looping the recipents animation
				triggerButBetter = true;
			}
		}
		if (mAnimator->getProgress() > 0.9f)
		{
			DepleteMana(mMoves.GetMove()->skill->GetManaCost());
			if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
			{
				if (mMoves.GetMove()->skill->IsAOE())
				{
					Character* temp = nullptr;
					for (std::shared_ptr<Odyssey::Entity> c : playerTeam)
					{
						if (c != nullptr)
						{
							temp = c.get()->getComponent<Character>();
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
					mMoves.GetMove()->skill->Use(*this, *mMoves.GetMove()->target);
			}
			else
			{
				if (mMoves.GetMove()->skill->IsAOE())
				{
					Character* temp = nullptr;
					for (std::shared_ptr<Odyssey::Entity> c : enemyTeam)
					{
						if (c != nullptr)
						{
							temp = c.get()->getComponent<Character>();
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
					mMoves.GetMove()->skill->Use(*this, *mMoves.GetMove()->target);
			}
			mMoves.ResetMove();
			triggerButBetter = false;
			particleTriggerButBetter = false;
			mCurrentState = STATE::FINISHED;
		}
		break;
	}
	case STATE::FINISHED:
	{
		mCurrentState = STATE::NONE;
		ManageStatusEffects(mBuffs);
		ManageStatusEffects(mDebuffs);
		ManageStatusEffects(mSheilds);
		return true;
		break;
	}
	case STATE::DEAD:
	{
		if (mAnimator->getProgress() > 0.8f)
			return true;
		break;
	}
	default:
		break;
	}
	return false;
}
void EnemyComponent::Die()
{
	mCurrentState = STATE::DEAD;
	ClearStatusEffects();
	mAnimator->playClip("Dead");
	//pTurnNumber->setText(L"X");
}

void EnemyComponent::BeginAttack(std::vector<std::shared_ptr<Odyssey::Entity>> targets)
{
	mAnimator->playClip(mMoves.GetMove()->skill->GetAnimationId());
	if (mMoves.GetMove()->skill->IsAOE() && mMoves.GetMove()->skill->GetParticleSystem() != nullptr)
	{
		DirectX::XMFLOAT3 aoeSpawn(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 tempTransform(0.0f, 0.0f, 0.0f);
		if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
		{
			for (std::shared_ptr<Odyssey::Entity> t : targets)
			{
				if (t)
				{
					tempTransform = t->getComponent<Odyssey::Transform>()->getPosition();
					aoeSpawn.x += tempTransform.x;
					aoeSpawn.y += tempTransform.y;
					aoeSpawn.z += tempTransform.z;
				}
			}
			aoeSpawn.x /= static_cast<float>(targets.size());
			aoeSpawn.y /= static_cast<float>(targets.size());
			aoeSpawn.z /= static_cast<float>(targets.size());
		}
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(aoeSpawn.x, aoeSpawn.y, aoeSpawn.z);
	}
	else if (mMoves.GetMove()->target != nullptr && mMoves.GetMove()->skill->GetParticleSystem() != nullptr)
	{
		// Set position to start in desired postion
		DirectX::XMFLOAT3 temp(mEntity->getComponent<Odyssey::Transform>()->getPosition());
		temp.x += mMoves.GetMove()->skill->GetPosOffset().x;
		temp.y += mMoves.GetMove()->skill->GetPosOffset().y;
		temp.z += mMoves.GetMove()->skill->GetPosOffset().z;
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetOrigin(temp);
		// Set target position
		DirectX::XMFLOAT3 temp2(mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Transform>()->getPosition());
		temp2.y += 3.0f;
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetTargetPos(temp2);
		// Use velocity to calc lifetime
		DirectX::XMFLOAT3 tempVelocity = mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->GetVelocity();
		float tempLifeTime = sqrtf((powf((temp2.x - temp.x), 2) + powf((temp2.y - temp.y), 2) + powf((temp2.z - temp.z), 2))) / sqrtf((powf(tempVelocity.x, 2) + powf(tempVelocity.y, 2) + powf(tempVelocity.z, 2)));
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetLifeTime(tempLifeTime);
	}
	mCurrentState = STATE::INPROGRESS;
}

std::vector<std::shared_ptr<Skills>> EnemyComponent::GetSkills()
{
	return mMoves.GetSkillList();
}


void EnemyComponent::GanfaulPhaseMechanic()
{
	std::cout << "I DID IT!!!!" << std::endl;
}

