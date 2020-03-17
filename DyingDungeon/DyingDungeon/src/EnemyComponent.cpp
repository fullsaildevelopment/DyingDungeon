#include "EnemyComponent.h"
#include "Entity.h"
#include "Transform.h"
#include "CharacterHUDElements.h"

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
	mBloodEffectPrefab = nullptr;
	mImpactIndicator = nullptr;
	mMechPtr = nullptr;
	mMoveOverride = GameplayTypes::SKILLTYPE::NONE;
	mCurrentState = STATE::NONE;
	mID = _enemyID;
	mIsBleeding = false;
	mBleedTimer = 0;
	mIsRegenerating = false;
	mRegenTimer = 0;
	mStunTimer = 0;
	mProvokedTimer = 0;
	mShielding = 0.0f;
	mShieldTimer = 0;
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

		// Sound Clips
		mSoundClips["Hit"] = "SkeletonHit";
		mSoundClips["Death"] = "SkeletonDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 40.0f;
		mBaseDefense = mDefense = 40.0f;
		mBaseSpeed = mSpeed = 10.0f;
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
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_BasicAttack.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_FwdKick.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Skeleton/Skeleton_SpinKick.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4

		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"";
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

		// Sound Clips
		mSoundClips["Hit"] = "MaleHitReaction";
		mSoundClips["Death"] = "MaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 30.0f;
		mBaseDefense = mDefense = 30.0f;
		mBaseSpeed = mSpeed = 40.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;
		
		// Set mechanic pointer
		mMechPtr = &EnemyComponent::GanfaulPhaseMechanic;

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"";
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
	case GameplayTypes::ENEMYID::Summoner:
	{
		// Set the character Model path
		mModel = "assets/models/Summoner.dxm";

		// Set the character name
		mName = L"Della";

		// Set the character subname
		mSubName = L"";

		// Set the portaits path
		mPortrait = L"assets/images/SummonerPortrait.png";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Sound Clips
		mSoundClips["Hit"] = "FemaleHitReaction";
		mSoundClips["Death"] = "FemaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 20.0f;
		mBaseDefense = mDefense = 20.0f;
		mBaseSpeed = mSpeed = 20.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;

		// Set mechanic pointer
		mMechPtr = nullptr;

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Attack_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Attack_2.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Attack_3.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4
		tempAnimationData.mAnimationNickName = "Skill_4";
		tempAnimationData.mAnimationPath = "assets/animations/Summoner/Summoner_Attack_4.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	case GameplayTypes::ENEMYID::MeleeDemon:
	{
		// Set the character Model path
		mModel = "assets/models/MeleeDemon.dxm";

		// Set the character name
		mName = L"MeleeDemon";

		// Set the character subname
		mSubName = L"";

		// Set the portaits path
		mPortrait = L"assets/images/MeleeDemonPortrait.png";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 150.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Sound Clips
		mSoundClips["Hit"] = "MaleHitReaction";
		mSoundClips["Death"] = "MaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 60.0f;
		mBaseDefense = mDefense = 40.0f;
		mBaseSpeed = mSpeed = 15.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;

		// Set mechanic pointer
		mMechPtr = nullptr;

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned
		tempAnimationData.mAnimationNickName = "Stun";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Stun.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff
		tempAnimationData.mAnimationNickName = "GotBuffed";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Buff.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Skill_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/MeleeDemon/MeleeDemon_Skill_3.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);
		// Skill 4

		////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	case GameplayTypes::ENEMYID::CasterDemon:
	{
		// Set the character Model path
		mModel = "assets/models/CasterDemon.dxm";

		// Set the character name
		mName = L"CasterDemon";

		// Set the character subname
		mSubName = L"";

		// Set the portaits path
		mPortrait = L"assets/images/CasterDemonPortrait.png";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 125.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Sound Clips
		mSoundClips["Hit"] = "MaleHitReaction";
		mSoundClips["Death"] = "MaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 40.0f;
		mBaseDefense = mDefense = 20.0f;
		mBaseSpeed = mSpeed = 30.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;

		// Set mechanic pointer
		mMechPtr = nullptr;

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned
		tempAnimationData.mAnimationNickName = "Stun";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Stun.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff
		tempAnimationData.mAnimationNickName = "GotBuffed";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Buff.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Skill_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Skill_2.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Skill_3.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4
		tempAnimationData.mAnimationNickName = "Skill_4";
		tempAnimationData.mAnimationPath = "assets/animations/CasterDemon/CasterDemon_Skill_4.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	case GameplayTypes::ENEMYID::EnemyMage:
	{
		// Set the character Model path
		mModel = "assets/models/EnemyMage.dxm";

		// Set the character name
		mName = L"EnemyMage";

		// Set the character subname
		mSubName = L"";

		// Set the portaits path
		mPortrait = L"assets/images/EnemyMagePortrait.png";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Sound Clips
		mSoundClips["Hit"] = "FemaleHitReaction";
		mSoundClips["Death"] = "FemaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 20.0f;
		mBaseDefense = mDefense = 20.0f;
		mBaseSpeed = mSpeed = 20.0f;
		////////////////////////////////////

		// Set move overide for AI
		mMoveOverride = GameplayTypes::SKILLTYPE::ATTACK;

		// Set mechanic pointer
		mMechPtr = nullptr;

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/EnemyMage/EnemyMage_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/EnemyMage/EnemyMage_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/EnemyMage/EnemyMage_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/EnemyMage/EnemyMage_Skill_1.dxanim";
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
bool EnemyComponent::TakeTurn(std::vector<Odyssey::Entity*> playerTeam, std::vector<Odyssey::Entity*> enemyTeam)
{
	// State machine to navigate while its the AI takes its turn.
	switch (mCurrentState)
	{
	// If the AI is stunned manage all his effects and exit the loop.
	case STATE::STUNNED:
	{
		ManageCastedEffects();
		ManageTOREffects();
		mStunTimer--;
		if (mStunTimer <= 0)
		{
			if (mCurrentState != STATE::DEAD)
			{
				mCurrentState = STATE::NONE;
				mEntity->getComponent<Odyssey::Animator>()->playClip("Idle");
				GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetStunBuff()->setVisible(false);
			}
		}
		if (mProvoked != nullptr)
		{
			mProvokedTimer--;
			if (mProvokedTimer <= 0)
			{
				mProvoked = nullptr;
				mProvokedTimer = 0;
				GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetProvokeBuff()->setVisible(false);
			}
		}
		return true;
	}
	// If the player is not stunned enter the start of his turn, all bleed and regen dots will tick. 
	// If the players character dies he will have his state set to dead, else he will start to select his move.
	case STATE::NONE:
	{
		// If i have an additional mechanic do it
		if(mMechPtr)
			(this->*mMechPtr)();
		ManageCastedEffects();
		ManageTOREffects();
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
		static bool soundTriggerButBetter = false;

		if (soundTriggerButBetter == false && mMoves.GetMove()->skill->GetSoundEffectTiming() <= mAnimator->getProgress())
		{
			RedAudioManager::Instance().PlaySFX(mMoves.GetMove()->skill->GetSoundEffectName().c_str());
			soundTriggerButBetter = true;
		}
		// Fire particle effects when the timming is right
		if (mMoves.GetMove()->skill->GetParticleSystem() != nullptr && !particleTriggerButBetter && mAnimator->getProgress() > mMoves.GetMove()->skill->GetPSFiringTime())
		{
			// Turn particle effect on
			//mMoves.GetMove()->skill->GetParticleSystem()->getEntity()->setActive(true);
			//mMoves.GetMove()->skill->GetParticleSystem()->play();

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
					for (Odyssey::Entity* c : playerTeam)
					{
						// If they arnt dead play thier animations and particle effects
						if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							// Play "Hit" animation
							c->getComponent<Odyssey::Animator>()->playClip("Hit");
							RedAudioManager::Instance().PlaySFX(c->getComponent<Character>()->GetSoundClipName("Hit").c_str());
							// Play particle effect
							c->getComponent<Character>()->SpawnBloodEffect();
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
				{
					// Play "Hit" animation
					mMoves.GetMove()->target->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
					RedAudioManager::Instance().PlaySFX(mMoves.GetMove()->target->GetSoundClipName("Hit").c_str());
					// Play particle effect
					mMoves.GetMove()->target->SpawnBloodEffect();
				}
			}
			else
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mMoves.GetMove()->skill->IsAOE())
				{
					// For each party member
					for (Odyssey::Entity* c : enemyTeam)
					{
						// If the target is not dead, and not the caster
						if (c != nullptr && c->getComponent<EnemyComponent>() != this && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							// Play "GotBuffed" animation
							c->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
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

			// If its meant for the hero apply the effect to the hero party, else apply the effect to the enemy party
			if (mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mMoves.GetMove()->skill->IsAOE())
				{
					// Temp character pointer to help loop through list of targets
					Character* temp = nullptr;

					// For each party member
					for (Odyssey::Entity* c : playerTeam)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c->getComponent<Character>();

							// If thier not dead, apply the skills effect to them
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);

							// Reset Impact Indicators (AOE ONLY)
							temp->GetInpactIndicator()->setActive(false);
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
					for (Odyssey::Entity* c : enemyTeam)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c->getComponent<Character>();

							// If they're not dead, apply the skills effect to them
							if (temp->GetState() != STATE::DEAD)
								mMoves.GetMove()->skill->Use(*this, *temp);

							// Reset Impact Indicators (AOE ONLY)
							temp->GetInpactIndicator()->setActive(false);
						}
					}
				}
				else if (mMoves.GetMove()->target != nullptr)
					mMoves.GetMove()->skill->Use(*this, *mMoves.GetMove()->target);
			}
			
			// Reset my move best move // Also turns impact indicator off
			mMoves.ResetMove();

			// Reset static bools
			triggerButBetter = false;
			particleTriggerButBetter = false;
			soundTriggerButBetter = false;

			// Set current state to finished
			mCurrentState = STATE::FINISHED;
		}
		break;
	}

	// Once here loop through remaining status effects and reset current state
	case STATE::FINISHED:
	{
		// Reset state to default
		mCurrentState = STATE::NONE;

		if (mProvoked != nullptr)
		{
			mProvokedTimer--;
			if (mProvokedTimer <= 0)
			{
				mProvoked = nullptr;
				mProvokedTimer = 0;
				GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetProvokeBuff()->setVisible(false);
			}
		}

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
	// Play death sound effect
	RedAudioManager::Instance().PlaySFX(mSoundClips["Death"].c_str());

	// Clear all remaining status effects
	ClearStatusEffects();

	// Play the death animation
	mEntity->getComponent<Odyssey::Animator>()->playClip("Dead");

	// Stop all active particle effects
	//StopParticleEffects();

	// Set state to dead
	mCurrentState = STATE::DEAD;
}

// Function that sends the state into the inprogress state, queing animations, and setting variables for particle effect locations
void EnemyComponent::BeginAttack(std::vector<Odyssey::Entity*> targets)
{
	// Play the skills animation
	mAnimator->playClip(mMoves.GetMove()->skill->GetAnimationId());

	if (mMoves.GetMove()->target != nullptr && mMoves.GetMove()->skill->GetParticleSystem() != nullptr)
	{

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

std::shared_ptr<Odyssey::Component> EnemyComponent::clone() const
{
	return std::make_shared<EnemyComponent>(*this);
}

// Init function called when object loads into scene
void EnemyComponent::initialize()
{
	mAnimator = mEntity->getComponent<Odyssey::Animator>();
	mMoves = AIMoves(static_cast<int>(mID), this);
}