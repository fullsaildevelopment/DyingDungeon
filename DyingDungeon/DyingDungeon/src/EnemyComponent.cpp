#include "EnemyComponent.h"
#include "Entity.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

// Constructor
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
	mID = _enemyID;
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
		mPortrait = L"assets/images/SkeletonPortrait.png";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 125.0f;

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 20.0f;
		mBaseDefense = mDefense = 20.0f;
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
		mBaseAttack = mAttack = 35.0f;
		mBaseDefense = mDefense = 50.0f;
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
		tempAnimationData.mAnimationPath = "assets/animations/Ganfaul/Ganfaul_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Ganfaul/Ganfaul_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Ganfaul/Ganfaul_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Ganfaul/Ganfaul_Attack.dxanim";
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

// Destructor
EnemyComponent::~EnemyComponent()
{
}

// Function that allows the player to take thier turn, Character Controler
bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	// State machine to navigate while its the AI takes its turn.
	switch (mCurrentState)
	{
	// If the AI is stunned manage all his effects and exit the loop.
	case STATE::STUNNED:
	{
		mCurrentState = STATE::NONE;
		ManageAllEffects();
		return true;
	}
	// If the player is not stunned enter the start of his turn, all bleed and regen dots will tick. 
	// If the players character dies he will have his state set to dead, else he will start to select his move.
	case STATE::NONE:
	{
		// If i have an additional mechanic do it
		if(mMechPtr)
			(this->*mMechPtr)();
		ManageStatusEffects(mRegens);
		ManageStatusEffects(mBleeds);
		if (mCurrentHP <= 0.0f)
			Die();
		else
			mCurrentState = STATE::SELECTMOVE;
		break;
	}
	// Here the AI detrimens its best move
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
	// The move animation has begun, this will run until the animation is over
	case STATE::INPROGRESS:
	{
		// Static bool used to track whenever its time to play the recipent animation ie hit or be buffed, or particle effect 
		static bool particleTriggerButBetter = false;
		static bool triggerButBetter = false;

		// Fire particle effects when the timming is right
		if (mMoves.GetMove()->skill->GetParticleSystem() != nullptr && !particleTriggerButBetter && mAnimator->getProgress() > mMoves.GetMove()->skill->GetPSFiringTime())
		{
			// Turn particle effect on
			mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->setActive(true);
			mMoves.GetMove()->skill->GetParticleSystem()->play();

			// If its a projectile particle effect turn on its mover
			if (!mMoves.GetMove()->skill->IsAOE())
				mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->setActive(true);

			// Set static bool to true to prevent repeating this effect
			particleTriggerButBetter = true;
		}

		// Play animation when the timming is right
		if (!triggerButBetter && mAnimator->getProgress() > mMoves.GetMove()->skill->GetAnimationTiming())
		{
			// If its ment for the hero party play the hit animation to time with the animation timing, else play the animations and particle effects to the enemy party
			if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mMoves.GetMove()->skill->IsAOE())
				{
					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : playerTeam)
					{
						// If they arnt dead play thier animations and particle effects
						if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							// Play "Hit" animation
							c.get()->getComponent<Odyssey::Animator>()->playClip("Hit");

							// Set up particle effect location
							DirectX::XMFLOAT3 t = c.get()->getComponent<Odyssey::Transform>()->getPosition();
							c.get()->getComponent<Character>()->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);

							// Play particle effect
							c.get()->getComponent<Character>()->GetPSBlood()->play();
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
				{
					// Play "Hit" animation
					mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");

					// Set up particle effect location
					DirectX::XMFLOAT3 t = mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
					mMoves.GetMove()->target->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);

					// Play particle effect
					mMoves.GetMove()->target->GetPSBlood()->play();
				}
			}
			else
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mMoves.GetMove()->skill->IsAOE())
				{
					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : enemyTeam)
					{
						// If the target is not dead, and not the caster
						if (c != nullptr && c.get()->getComponent<EnemyComponent>() != this && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							// Play "GotBuffed" animation
							c.get()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr && mMoves.GetMove()->target != this)
				{
					// Play "GotBuffed" animation
					mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
				}
			}
			// Set trigger to true to avoid repeating the recipents animation
			triggerButBetter = true;
		}

		// Once the animation is nearly finished use the skill and apply the effects
		if (mAnimator->getProgress() > 0.9f)
		{
			// Depleate the caster mana
			DepleteMana(mMoves.GetMove()->skill->GetManaCost());

			// If its ment for the hero apply the effect to the hero party, else apply the effect to the enemy party
			if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mMoves.GetMove()->skill->IsAOE())
				{
					// Temp character pointer to help loop through list of targets
					Character* temp = nullptr;

					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : playerTeam)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c.get()->getComponent<Character>();

							// If thier not dead, apply the skills effect to them
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
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mMoves.GetMove()->skill->IsAOE())
				{
					// Temp character pointer to help loop through list of targets
					Character* temp = nullptr;

					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : enemyTeam)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c.get()->getComponent<Character>();

							// If thier not dead, apply the skills effect to them
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
					mMoves.GetMove()->skill->Use(*this, *mMoves.GetMove()->target);
			}

			// Reset my move best move
			mMoves.ResetMove();

			// Reset static bools
			triggerButBetter = false;
			particleTriggerButBetter = false;

			// Set current state to finished
			mCurrentState = STATE::FINISHED;
		}
		break;
	}

	// Once here loop through remaining status effects and reset current state
	case STATE::FINISHED:
	{
		// Manage all my buffs
		ManageStatusEffects(mBuffs);

		// Manage all my debuffs
		ManageStatusEffects(mDebuffs);

		// Manage all my shields
		ManageStatusEffects(mSheilds);

		// Reset state to default
		mCurrentState = STATE::NONE;

		// Return true
		return true;

		break;
	}

	// If here the character has died and will begin his death animation
	case STATE::DEAD:
	{
		// If the "death" animation is over return true so it skips this character turn
		if (mAnimator->getProgress() > 0.8f)
			return true;

		break;
	}

	// Default case should never hit here
	default:
	{
		break;
	}
	}
	return false;
}

// Function that gets called to set the character state to dead, along with all other necessary variables
void EnemyComponent::Die()
{
	// Clear all remaining status effects
	ClearStatusEffects();

	// Play the death animation
	mAnimator->playClip("Dead");

	// Stop all active particle effects
	StopParticleEffects();

	// Set state to dead
	mCurrentState = STATE::DEAD;
}

// Function that sends the state into the inprogress state, queing animations, and setting variables for particle effect locations
void EnemyComponent::BeginAttack(std::vector<std::shared_ptr<Odyssey::Entity>> targets)
{
	// Play the skills animation
	mAnimator->playClip(mMoves.GetMove()->skill->GetAnimationId());

	// If the skill is an AOE move, and it has a valid particle effect
	if (mMoves.GetMove()->skill->IsAOE() && mMoves.GetMove()->skill->GetParticleSystem() != nullptr)
	{
		// Make variables to store position data
		DirectX::XMFLOAT3 aoeSpawn(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 tempTransform(0.0f, 0.0f, 0.0f);

		// If its an attack loop through all enemies to get an avg position, else loop though all the players
		if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
		{
			// For each entity
			for (std::shared_ptr<Odyssey::Entity> t : targets)
			{
				// If valid
				if (t)
				{
					// Get the transforms position
					tempTransform = t->getComponent<Odyssey::Transform>()->getPosition();

					// Add posititon to variable for each entity //
					///////////////////////////////////////////////
					aoeSpawn.x += tempTransform.x;
					aoeSpawn.y += tempTransform.y;
					aoeSpawn.z += tempTransform.z;
					///////////////////////////////////////////////
				}
			}

			// Divid by party size to get the average position //
			/////////////////////////////////////////////////////
			aoeSpawn.x /= static_cast<float>(targets.size());
			aoeSpawn.y /= static_cast<float>(targets.size());
			aoeSpawn.z /= static_cast<float>(targets.size());
			/////////////////////////////////////////////////////
		}

		// Set the skills particle systems postion to be the calculated position
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(aoeSpawn.x, aoeSpawn.y, aoeSpawn.z);
	}
	else if (mMoves.GetMove()->target != nullptr && mMoves.GetMove()->skill->GetParticleSystem() != nullptr)
	{
		// Get my entitys position
		DirectX::XMFLOAT3 temp(mEntity->getComponent<Odyssey::Transform>()->getPosition());

		// Add the effects offset to the entitys position to place the projectile in the proper starting position //
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		temp.x += mMoves.GetMove()->skill->GetPosOffset().x;
		temp.y += mMoves.GetMove()->skill->GetPosOffset().y;
		temp.z += mMoves.GetMove()->skill->GetPosOffset().z;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Set the projectiles position to the calculated postition
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetOrigin(temp);

		// Get the projectiles target position
		DirectX::XMFLOAT3 temp2(mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Transform>()->getPosition());

		// Offset the target position by an amount to have it hit about center mass
		temp2.y += 3.0f;

		// Set the projectiles target position
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetTargetPos(temp2);

		// Use projectiles velocity to calc lifetime to target //
		/////////////////////////////////////////////////////////
		DirectX::XMFLOAT3 tempVelocity = mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->GetVelocity();
		float tempLifeTime = sqrtf((powf((temp2.x - temp.x), 2) + powf((temp2.y - temp.y), 2) + powf((temp2.z - temp.z), 2))) / sqrtf((powf(tempVelocity.x, 2) + powf(tempVelocity.y, 2) + powf(tempVelocity.z, 2)));
		mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetLifeTime(tempLifeTime + 0.1f);
		/////////////////////////////////////////////////////////
	}

	// Set the current state to inprogress
	mCurrentState = STATE::INPROGRESS;
}

// Returns the characters skill list
std::vector<std::shared_ptr<Skills>> EnemyComponent::GetSkills()
{
	return mMoves.GetSkillList();
}

GameplayTypes::ENEMYID EnemyComponent::GetID()
{
	return mID;
}

// Ganfouls game mechanic function
void EnemyComponent::GanfaulPhaseMechanic()
{
	static unsigned int CURRENTPHASE = 0;
	if (CURRENTPHASE != 3 && mCurrentHP <= (mBaseMaxHP * 0.25f))
	{
		CURRENTPHASE = 3;
		mAttack += mBaseAttack;
	}
	else if (CURRENTPHASE != 2 && mCurrentHP <= (mBaseMaxHP * 0.50f))
	{
		CURRENTPHASE = 2;
		mDefense += mBaseDefense;
	}
	else if (CURRENTPHASE != 1 && mCurrentHP <= (mBaseMaxHP * 0.75f))
	{
		CURRENTPHASE = 1;
		mSpeed += mBaseSpeed;
	}
}

