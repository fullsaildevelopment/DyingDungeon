#include "HeroComponent.h"
#include "Entity.h"
#include "Transform.h"
#include "EnemyComponent.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "Attack.h"
#include "Buffs.h"
#include "Heal.h"
#include "Bleed.h"
#include "StatUp.h"
#include "StatDown.h"
#include "Regens.h"
#include "Stun.h"
#include "Shields.h"
#include "Provoked.h"
#include "Clense.h"
#include "CharacterHUDElements.h"
#include "JudgementMover.h"
#include <memory>

CLASS_DEFINITION(Character, HeroComponent)

// Constructor
HeroComponent::HeroComponent(GameplayTypes::HEROID id)
{
	// Setting default values for member variables //
	////////////////////////////////////////////////
	mCurrentState = STATE::NONE;
	mHero = true;
	mEXP = 0.0f;
	mCurrentLevel = 0;
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	mProvoked = nullptr;
	mBloodEffectPrefab = nullptr;
	mImpactIndicator = nullptr;
	mID = id;
	mHeroList.resize(4);
	mEnemyList.resize(4);
	mIsCheating = false;
	mIsBleeding = false;
	mBleedTimer = 0;
	mIsRegenerating = false;
	mRegenTimer = 0;
	mStunTimer = 0;
	mProvokedTimer = 0;
	mShielding = 0.0f;
	mShieldTimer = 0;
	mThemeColor = { 0.0f,0.0f,0.0f };
	////////////////////////////////////////////////

	// Temp variable for creating status effects
	std::shared_ptr<StatusEffect> temp;

	// Make a temp variable to contain animation data
	AnimationImportData tempAnimationData;
	
	// Switch statment that builds the hero depending on the hero id that gets passed in the constructor
	switch (id)
	{
	case GameplayTypes::HEROID::Paladin:
	{
		// Set the character Model path
		mModel = "assets/models/Paladin.dxm";

		// Set the character name
		mName = L"Paladin";
		
		// Set the character subname
		mSubName = L"Tank";

		// Set the portaits path
		mPortrait = L"assets/images/PaladinPortrait.jpg";

		// Set the characters theme color
		mThemeColor = {255.0f,203.0f,31.0f};
    
		// Sound Clips
		mSoundClips["Hit"] = "PaladinHitReaction";
		mSoundClips["Death"] = "MaleDeath";

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);
		
		// Recieves Buff
		tempAnimationData.mAnimationNickName = "GotBuffed";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_Taunt.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_BasicAttack.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_BigAttack.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_Heal.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4
		tempAnimationData.mAnimationNickName = "Skill_4";
		tempAnimationData.mAnimationPath = "assets/animations/Paladin/Paladin_Defense.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"The last paladin in the Church of Metis, seeking justice. He uses the Goddess’ light to protect his allies.";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 150.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 65.0f; // High
		mBaseDefense = mDefense = 80.0f; // Very High
		mBaseSpeed = mSpeed = 20.0f; // Slow
		////////////////////////////////////

		// Make the character skills // 7 basics to kill a skeleton, can take 11 hits before death
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Skill 1
		temp = std::make_shared<Provoked>(1, this, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Judgement", "Skill_1", 0.45f, -20.0f, 12.0f, temp));
		mSkillList[0]->SetSkillIconPath(L"assets/images/PaladinSkills/Paladin_Skill_1.png");
		mSkillList[0]->SetSoundEffect("Judgement",0.0f);
		mSkillList[0]->SetSkillDescription(L"Strike an enemy with divine power dealing 12 damage provoking him to hit you. Restores 15 mana.");
		// Skill 2
		mSkillList.push_back(std::make_shared<Attack>(L"Smite", "Skill_2", 0.50f, 30.0f, 35.0f, 10.0f));
		mSkillList[1]->SetSkillIconPath(L"assets/images/PaladinSkills/Paladin_Skill_2.png");
		mSkillList[1]->SetSoundEffect("Smite",0.25f);
		mSkillList[1]->SetSkillDescription(L"Smite the enemy with holy light dealing 35 damage and healing the paladin for 20 health. Costs 20 mana.");
		// Skill 3
		temp = std::make_shared<Shields>(25.0f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>(L"Shield of Light", "Skill_3", 0.89f, 25.0f, temp, true, true));
		mSkillList[2]->SetSkillIconPath(L"assets/images/PaladinSkills/Paladin_Skill_3.png");
		mSkillList[2]->SetSoundEffect("ShieldOfLight",0.25f);
		mSkillList[2]->SetSkillDescription(L"A shield of light slams down in front of all team members granting 25 temp health for 3 turns. Costs 20 mana.");
		// Skill 4
		temp = std::make_shared<StatUp>(0.5f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>(L"Blessing of Light", "Skill_4", 0.89f, 15.0f,temp,true, true));
		mSkillList[3]->SetSkillIconPath(L"assets/images/PaladinSkills/Paladin_Skill_4.png");
		mSkillList[3]->SetSoundEffect("BlessingOfLight", 0.25f);
		mSkillList[3]->SetSkillDescription(L"Protects all allies from harm granting them 50% increased defense 3 turns. Costs 15 mana.");
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}

	case GameplayTypes::HEROID::Mage:
	{
		// Set the character Model path
		mModel = "assets/models/Mage.dxm";

		// Set the character name
		mName = L"Mage";

		// Set the character subname
		mSubName = L"Dps";

		// Set the portaits path
		mPortrait = L"assets/images/MagePortrait.jpg";
		
		// Set the characters theme color
		mThemeColor = { 31.0f, 255.0f, 203.0f };

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 150.0f;

		// Sound Clips
		mSoundClips["Hit"] = "MageHitReaction";
		mSoundClips["Death"] = "FemaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 80.0f;
		mBaseDefense = mDefense = 20.0f;
		mBaseSpeed = mSpeed = 60.0f;
		////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Attack_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Attack_2.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Attack_3.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4
		tempAnimationData.mAnimationNickName = "Skill_4";
		tempAnimationData.mAnimationPath = "assets/animations/Mage/Mage_Attack_4.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"A vengeful mage from the Forest of Gelmeur, seeking revenge for her parents. She unleashes destruction on her enemies with magic, but lacks defensive capabilities.";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Make the character skills // kills in 5, can take 6
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Skill 1
		mSkillList.push_back(std::make_shared<Attack>(L"Meteor Shard", "Skill_1", 0.65f, -15.0f, 17.0f));
		mSkillList[0]->SetSkillIconPath(L"assets/images/MageSkills/Mage_Skill_1.png");
		mSkillList[0]->SetSoundEffect("MeteorShard", 0.0f);
		mSkillList[0]->SetSkillDescription(L"Send forth an orb of incredibly destructive chaotic elemental magic inflicting 17 damage. Refunds 15 mana.");
		// Skill 2 
		temp = std::make_shared<StatDown>(0.5f,2,STATS::Spd,nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Wind Slash", "Skill_2", 0.25f, 15.0f, 15.0f, temp, true));
		mSkillList[1]->SetSkillIconPath(L"assets/images/MageSkills/Mage_Skill_2.png");
		mSkillList[1]->SetSoundEffect("WindSlash", 0.0f);
		mSkillList[1]->SetStatusChance(0.5f);
		mSkillList[1]->SetSkillDescription(L"Slash all enemies with a burst of wind dealing 15 damage per hit with a 50% chance to inflict speed down. Costs 15 mana.");
		// Skill 3
		temp = std::make_shared<Bleed>(0.10f, 1, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"FireStorm", "Skill_3", 0.60f, 30.0f, 30.0f, temp, true));
		mSkillList[2]->SetSkillIconPath(L"assets/images/MageSkills/Mage_Skill_3.png");
		mSkillList[2]->SetSoundEffect("FireStorm", 0.0f);
		mSkillList[2]->SetSkillDescription(L"Conjure a hellstorm dealing 30 damage to all enemies and inflicting burn. Costs 30 mana.");
		// Skill 4
		temp = std::make_shared<Stun>(1,nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Lightning Bolt", "Skill_4", 0.25f, 25.0f, 25.0f,temp));
		mSkillList[3]->SetSkillIconPath(L"assets/images/MageSkills/Mage_Skill_4.png");
		mSkillList[3]->SetSoundEffect("LightningBolt", 0.0f);
		mSkillList[3]->SetStatusChance(0.33f);
		mSkillList[3]->SetSkillDescription(L"Channel a bolt of lightning dealing 25 damage to a single enemy with a 33% chance of inflicting a stun. Costs 35 mana.");
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}

	case GameplayTypes::HEROID::Bard:
	{
		// Set the character Model path
		mModel = "assets/models/Bard.dxm";

		// Set the character name
		mName = L"Bard";

		// Set the character subname
		mSubName = L"Literal God";

		// Set the portaits path
		mPortrait = L"assets/images/BardPortrait.png";

		// Set the characters theme color
		mThemeColor = { 31.0f, 255.0f, 75.0f };

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 125.0f;

		// Sound Clips
		mSoundClips["Hit"] = "BardHitReaction";
		mSoundClips["Death"] = "FemaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 30.0f;
		mBaseDefense = mDefense = 30.0f;
		mBaseSpeed = mSpeed = 60.0f;
		////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Bard/Bard_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Bard/Bard_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Bard/Bard_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Bard/Bard_Attack.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Bard/Bard_Spell_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3

		// Skill 4

		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"A tavern bard, sick of her old life. She seeks fame and fortune. She supports and heals her allies using her many songs.";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Make the character skills //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Skill 1
		temp = std::make_shared<StatDown>(0.15f, 3, STATS::Spd, nullptr); 
		mSkillList.push_back(std::make_shared<Attack>(L"Starfire Arrow", "Skill_1", 0.60f, -10.0f, 10.0f, temp));
		mSkillList[0]->SetSkillIconPath(L"assets/images/BardSkills/Bard_Skill_1.png");
		mSkillList[0]->SetSoundEffect("StarFireArrow", 0.0f);
		mSkillList[0]->SetStatusChance(0.25f);
		mSkillList[0]->SetSkillDescription(L"Fire a magical arrow at a single target dealing 10 damage, with a 25% chance to inflict a 50% speed down. Returns 10 mana.");
		// Skill 2
		temp = std::make_shared<StatDown>(0.15f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Song of Misery", "Skill_1", 0.60f, 10.0f, 15.0f, temp, true));
		mSkillList[1]->SetSkillIconPath(L"assets/images/BardSkills/Bard_Skill_2.png");
		mSkillList[1]->SetSoundEffect("SongOfMisery", 0.25f);
		mSkillList[1]->SetStatusChance(0.5f);
		mSkillList[1]->SetSkillDescription(L"Fill the air with miserable music dealing 15 damage to all enemies, with a 50% chance to inflict a 15% defense down. Costs 15 mana.");
		// Skill 3
		temp = std::make_shared<StatUp>(0.25f, 3, STATS::Atk, nullptr);
		mSkillList.push_back(std::make_shared<Heal>(L"Song of Hope", "Skill_2", 0.60f, 30.0f, 25.0f, true));
		mSkillList[2]->SetStatusEffect(temp);
		mSkillList[2]->SetSkillIconPath(L"assets/images/BardSkills/Bard_Skill_3.png");
		mSkillList[2]->SetSoundEffect("SongOfHope", 0.0f);
		mSkillList[2]->SetSkillDescription(L"Play a delightful song giving a ally hope, healing for 35 health, and giving a 30% attack up. Costs 20 mana.");
		// Skill 4
		temp = std::make_shared<Clense>(1, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>(L"Purify", "Skill_2", 0.25f, 35.0f, temp, true, true));
		mSkillList[3]->SetSkillIconPath(L"assets/images//BardSkills/Bard_Skill_4.png");
		mSkillList[3]->SetSoundEffect("Purify", 0.0f);
		mSkillList[3]->SetSkillDescription(L"Does nothing but looks cool :D");
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}

	case GameplayTypes::HEROID::Warrior:
	{
		// Set the character Model path
		mModel = "assets/models/Warrior.dxm";

		// Set the character name
		mName = L"Warrior";

		// Set the character subname
		mSubName = L"Berserker";

		// Set the portaits path
		mPortrait = L"assets/images/WarriorPortrait.png";

		// Set the characters theme color
		mThemeColor = { 255.0f, 35.0f, 35.0f };

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 200.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 75.0f;

		// Sound Clips
		mSoundClips["Hit"] = "WarriorHitReaction";
		mSoundClips["Death"] = "MaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 50.0f;
		mBaseDefense = mDefense = 50.0f;
		mBaseSpeed = mSpeed = 30.0f;
		////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Dead.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned
		tempAnimationData.mAnimationNickName = "Stun";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Stun.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff
		tempAnimationData.mAnimationNickName = "GotBuffed";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Buff.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Skill_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Skill_2.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Skill_3.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4
		tempAnimationData.mAnimationNickName = "Skill_4";
		tempAnimationData.mAnimationPath = "assets/animations/Warrior/Warrior_Skill_4.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"An ex royal guard and a drunkard, seeking redemption in battle. He rushes headfirst into battle, unleashing devastating attacks with his axe.";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Make the character skills //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Skill 1
		mSkillList.push_back(std::make_shared<Attack>(L"Splitting Strike", "Skill_1", 0.4f, -10.0f, 15.0f));
		mSkillList[0]->SetSkillIconPath(L"assets/images/WarriorSkills/Warrior_Skill_1.png");
		mSkillList[0]->SetSoundEffect("SplittingStrike", 0.1f);
		mSkillList[0]->SetSkillDescription(L"Attempt to split a single target in half, dealing 15 damage. Returns 10 mana.");
		// Skill 2 
		temp = std::make_shared<StatDown>(0.5f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Armor Buster", "Skill_2", 0.4f, 20.0f, 15.0f,temp, false));
		mSkillList[1]->SetSkillIconPath(L"assets/images/WarriorSkills/Warrior_Skill_2.png");
		mSkillList[1]->SetSoundEffect("ArmorBuster", 0.15f);
		mSkillList[1]->SetSkillDescription(L"Crush an opponent's armor lowering their defense 50% and dealing 35 damage. Cost 20 mana.");
		// Skill 3 
		temp = std::make_shared<Provoked>(1, this, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Cleave", "Skill_3", 0.3f, 15.0f, 20.0f ,temp, true));
		mSkillList[2]->SetSkillIconPath(L"assets/images/WarriorSkills/Warrior_Skill_3.png");
		mSkillList[2]->SetSoundEffect("Cleave", 0.1f);
		mSkillList[2]->SetStatusChance(0.75f);
		mSkillList[2]->SetSkillDescription(L"Cleave through the enemy party, dealing 20 damage per hit with a 75% chance to provoke. Cost 15 mana.");
		// Skill 4 
		mSkillList.push_back(std::make_shared<Attack>(L"RAGE!", "Skill_4", 0.3f, 30.0f, 35.0f));
		mSkillList[3]->SetSkillIconPath(L"assets/images/WarriorSkills/Warrior_Skill_4.png");
		mSkillList[3]->SetSoundEffect("Rage", 0.1f);
		mSkillList[3]->SetSkillDescription(L"Go into a bloodthirsty rage, hitting the target for 45 damage. Cost 30 mana.");
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}

	case GameplayTypes::HEROID::Monk:
	{
		// Set the character Model path
		mModel = "assets/models/Monk.dxm";

		// Set the character name
		mName = L"Monk";

		// Set the character subname
		mSubName = L"Dps";

		// Set the portaits path
		mPortrait = L"assets/images/MonkPortrait.png";

		// Set the characters theme color
		mThemeColor = { 169.0f, 69.0f, 0.0f };

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 150.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Sound Clips
		mSoundClips["Hit"] = "MonkHitReaction";
		mSoundClips["Death"] = "FemaleDeath";

		// Set the stats for the character //
		////////////////////////////////////
		mBaseAttack = mAttack = 80.0f;
		mBaseDefense = mDefense = 20.0f;
		mBaseSpeed = mSpeed = 70.0f;
		////////////////////////////////////

		// Set the animation paths //
		////////////////////////////////////////////////////////////////////////////////////////////

		// Idle
		tempAnimationData.mAnimationNickName = "Idle";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Idle.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Dead
		tempAnimationData.mAnimationNickName = "Dead";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Death.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Is Stunned
		tempAnimationData.mAnimationNickName = "Stun";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Stun.dxanim";
		tempAnimationData.mIsLooping = false;
		mAnimations.push_back(tempAnimationData);

		// Recieves Hit
		tempAnimationData.mAnimationNickName = "Hit";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Hit.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Recieves Buff
		//tempAnimationData.mAnimationNickName = "GotBuffed";
		//tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Taunt.dxanim";
		//tempAnimationData.mIsLooping = true;
		//mAnimations.push_back(tempAnimationData);

		// Skill 1
		tempAnimationData.mAnimationNickName = "Skill_1";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Skill_1.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 2
		tempAnimationData.mAnimationNickName = "Skill_2";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Skill_2.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 3
		tempAnimationData.mAnimationNickName = "Skill_3";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Skill_3.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);

		// Skill 4
		tempAnimationData.mAnimationNickName = "Skill_4";
		tempAnimationData.mAnimationPath = "assets/animations/Monk/Monk_Skill_2.dxanim";
		tempAnimationData.mIsLooping = true;
		mAnimations.push_back(tempAnimationData);
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set the description for the character //
		////////////////////////////////////////////////////////////////////////////////////////////
		mDescription = L"One of Ganfaul’s generals turned rogue, seeking revenge for her sister. She can unleash quick, mighty blows in hand to hand combat, but won’t last long without support.";
		////////////////////////////////////////////////////////////////////////////////////////////

		// Make the character skills //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Skill 1
		temp = std::make_shared<StatDown>(0.5f, 3, STATS::Spd, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Jab", "Skill_1", 0.25f, -15.0f, 12.0f,temp));
		mSkillList[0]->SetSkillIconPath(L"assets/images/MonkSkills/Monk_Skill_1.png");
		mSkillList[0]->SetSoundEffect("Jab", 0.0f);
		mSkillList[0]->SetStatusChance(0.5f);
		mSkillList[0]->SetSkillDescription(L"dealing 15 damage with 50% chance to inflict a speed down. Refunds 15 mana.");
		// Skill 2 
		temp = std::make_shared<StatDown>(0.5f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Armor Break", "Skill_2", 0.15f, 20.0f, 20.0f, temp));
		mSkillList[1]->SetSkillIconPath(L"assets/images/MonkSkills/Monk_Skill_2.png");
		mSkillList[1]->SetSoundEffect("ArmorBreak", 0.05f);
		mSkillList[1]->SetSkillDescription(L"dealing 15 damage and inflicting defense down for 3 turns. Cost 20 mana.");
		// Skill 3 
		temp = std::make_shared<StatDown>(0.5f, 3, STATS::Atk, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Pressure Point", "Skill_3", 0.4f, 15.0f, 15.0f, temp, true));
		mSkillList[2]->SetSkillIconPath(L"assets/images/MonkSkills/Monk_Skill_3.png");
		mSkillList[2]->SetSoundEffect("PressurePoint", 0.1f);
		mSkillList[2]->SetSkillDescription(L"delaing 25 lowering their attack for 3 turns. Cost 15 mana.");
		// Skill 4
		temp = std::make_shared<Bleed>(0.15f, 2, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Break Ribs", "Skill_4", 0.15f, 30.0f, 25.0f,temp));
		mSkillList[3]->SetSkillIconPath(L"assets/images/MonkSkills/Monk_Skill_4.png");
		mSkillList[3]->SetSoundEffect("BreakRibs", 0.1f);
		mSkillList[3]->SetSkillDescription(L"dealing 30 damage and inflicting bleed for 3 turns. Cost 30 mana.");
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	// Default case, should never get hit
	default:
	{
		break;
	}
	}

#ifdef _DEBUG
		mSkillList.push_back(std::make_shared<Attack>(L"The Funny Move", "Skill_1", 0.5f, 0.0f, 1000.0f, temp));
		mSkillList.push_back(std::make_shared<Heal>(L"The other funny move", "Skill_2", 0.5f, 0.0f, -1000.0f));
#endif // _DEBUG

	

}

// Destructor
HeroComponent::~HeroComponent()
{
}

// Set up for clickable UI
void HeroComponent::SetupClickableUI(Odyssey::Sprite2D* _skillImage1, Odyssey::Sprite2D* _skillImage2, Odyssey::Sprite2D* _skillImage3, Odyssey::Sprite2D* _skillImage4)
{
	// Set up sprite's callback function for the Skill1Callback
	_skillImage1->registerCallback("onMouseClick", this, &HeroComponent::Skill1Callback);
	mSkillSprites.push_back(_skillImage1);
	// Set up sprite's callback function for the Skill2Callback
	_skillImage2->registerCallback("onMouseClick", this, &HeroComponent::Skill2Callback);
	mSkillSprites.push_back(_skillImage2);
	// Set up sprite's callback function for the Skill3Callback
	_skillImage3->registerCallback("onMouseClick", this, &HeroComponent::Skill3Callback);
	mSkillSprites.push_back(_skillImage3);
	// Set up sprite's callback function for the Skill4Callback
	_skillImage4->registerCallback("onMouseClick", this, &HeroComponent::Skill4Callback);
	mSkillSprites.push_back(_skillImage4);
}

// Function that allows the player to take thier turn, Character Controler
bool HeroComponent::TakeTurn(EntityList heros, EntityList enemies)
{
	// State machine to navigate while its the players turn.
	switch (mCurrentState)
	{
	// If the player is stunned manage all his effects and exit the loop.
	case STATE::STUNNED:
	{
		for (int i = 0; i < heros.size(); ++i)
		{
			if (heros[i] != nullptr)
				mHeroList[i] = heros[i];
		}
		for (int i = 0; i < enemies.size(); ++i)
		{
			if (enemies[i] != nullptr)
				mEnemyList[i] = enemies[i];
		}
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
		for (int i = 0; i < heros.size(); ++i)
		{
			if(heros[i] != nullptr)
				mHeroList[i] = heros[i];
		}
		for (int i = 0; i < enemies.size(); ++i)
		{
			if (enemies[i] != nullptr)
				mEnemyList[i] = enemies[i];
		}
		ManageCastedEffects();
		ManageTOREffects();
		if (mCurrentHP <= 0.0f)
			Die();
		else
			mCurrentState = STATE::SELECTMOVE;
		break;
	}

	// Here the player will be able to select from his four options for skills
	case STATE::SELECTMOVE:
	{
		if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::C))
			mIsCheating = !mIsCheating;

		// Cheat code to instakill a target
		if (mIsCheating && Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D0))
		{
			// Set temp variable to the selected move
			mCurrentSkill = mSkillList[4].get();

			// Change state to target selection
			mCurrentState = STATE::SELECTTARGET;
		}

		if (mIsCheating && Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D9))
		{
			// Set temp variable to the selected move
			mCurrentSkill = mSkillList[5].get();

			// Change state to target selection
			mCurrentState = STATE::SELECTTARGET;
		}

		// Use ability 1
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			SelctionState(0);
		}

		// Use ability 2
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D2))
		{
			SelctionState(1);
		}

		// Use ability 3
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D3))
		{
			SelctionState(2);
		}

		// Use ability 4
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D4))
		{
			SelctionState(3);
		}

		// Reset back to move selection
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			ResetToSelection();
		}
		break;
	}

	// If the player character's selection is not Aoe or not currently provoked they will then select thier target
	case STATE::SELECTTARGET:
	{
		// Static int used for targeting
		static int tempIndex = -1;

		// If an attack or debuff highlight applicable enemy target else look for applicable hero to target
		if (mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
		{
			if (tempIndex == -1)
			{
				// Loop till i find a non-dead target 
				for (int i = 0; i < enemies.size(); ++i)
				{
					if (enemies[i] != nullptr && enemies[i]->getComponent<Character>()->GetState() != STATE::DEAD)
					{
						tempIndex = i;
						break;
					}
				}
			}
			// Returns true when the player hits enter to confirm a target
			if(SelectTarget(enemies, tempIndex))
				tempIndex = -1;
		}
		else
		{
			if (tempIndex == -1)
			{
				// Loop till i find a non - dead target
				for (int i = 0; i < heros.size(); ++i)
				{
					if (heros[i] != nullptr && heros[i]->getComponent<Character>()->GetState() != STATE::DEAD)
					{
						tempIndex = i;
						break;
					}
				}
			}
			// Returns true when the player hits enter to confirm a target
			if (SelectTarget(heros, tempIndex))
				tempIndex = -1;
		}
		// Reset back to move selection
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			// Set the new cursor
			Odyssey::EventManager::getInstance().publish(new Odyssey::ChangeMouseCursorEvent(L"assets/images/Cursor/Cursor_Basic.cur"));
			ResetToSelection();
			tempIndex = -1;
		}
		break;
	}

	// The move animation has begun, this will run until the animation is over,
	case STATE::INPROGRESS:
	{
		// Static bool used to track whenever its time to play the recipent animation ie hit or be buffed, or particle effect 
		static bool particleTrigger = false;
		static bool animeTrigger = false;
		static bool soundTrigger = false;

		if (soundTrigger == false && mCurrentSkill->GetSoundEffectTiming() <= mAnimator->getProgress())
		{
			RedAudioManager::Instance().PlaySFX(mCurrentSkill->GetSoundEffectName().c_str());
			soundTrigger = true;
		}

		// Fire particle effects when the timming is right
		if (mCurrentSkill->GetParticleSystem() != nullptr && !particleTrigger && mAnimator->getProgress() > mCurrentSkill->GetPSFiringTime())
		{
			DirectX::XMFLOAT3 tempPos1;
			DirectX::XMFLOAT3 tempPos2;
			DirectX::XMVECTOR position = { 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR rotation = DirectX::XMLoadFloat3(&mCurrentSkill->GetParticleSystem()->getComponent<Odyssey::Transform>()->getEulerRotation());
			Odyssey::Entity* temp = nullptr;
			if (mCurrentSkill->IsAOE())
			{
				if (mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
				{
					for (Odyssey::Entity* c : enemies)
					{
						if (c == nullptr)
							continue;
						tempPos1 = c->getComponent<Odyssey::Transform>()->getPosition();
						tempPos2 = mCurrentSkill->GetParticleOffset();
						tempPos1.x += tempPos2.x;
						tempPos1.y += tempPos2.y;
						tempPos1.z += tempPos2.z;
						position = DirectX::XMLoadFloat3(&(tempPos1));
						Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mCurrentSkill->GetParticleSystem(), &temp, position, rotation));
					}
				}
				else
				{
					for (Odyssey::Entity* c : heros)
					{
						tempPos1 = c->getComponent<Odyssey::Transform>()->getPosition();
						tempPos2 = mCurrentSkill->GetParticleOffset();
						tempPos1.x += tempPos2.x + 0.5f;
						tempPos1.y += tempPos2.y + 0.5f;
						tempPos1.z += tempPos2.z + 0.5f;
						position = DirectX::XMLoadFloat3(&(tempPos1));
						position = DirectX::XMLoadFloat3(&(c->getComponent<Odyssey::Transform>()->getPosition()));
						Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mCurrentSkill->GetParticleSystem(), &temp, position, rotation));
					}
				}
			}
			else
			{
				if (mCurrentSkill->SetPartilceIsProjectile())
				{
					tempPos1 = mEntity->getComponent<Odyssey::Transform>()->getPosition();
					tempPos2 = mCurrentSkill->GetParticleOffset();
					tempPos1.x += tempPos2.x;
					tempPos1.y += tempPos2.y;
					tempPos1.z += tempPos2.z;
					tempPos2 = mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
					tempPos2 = { tempPos2.x - tempPos1.x,tempPos2.y - tempPos1.y + 3.0f,tempPos2.z - tempPos1.z};
					position = DirectX::XMLoadFloat3(&(tempPos1));
					Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mCurrentSkill->GetParticleSystem(), &temp, position, rotation));
					temp->getComponent<JudgementMover>()->SetVelocity(tempPos2, 10.0f);
					if (mID == GameplayTypes::HEROID::Mage)
						temp->getComponent<JudgementMover>()->SetSpinOnX(false);
				}
				else
				{
					tempPos1 = mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
					tempPos2 = mCurrentSkill->GetParticleOffset();
					tempPos1.x += tempPos2.x;
					tempPos1.y += tempPos2.y;
					tempPos1.z += tempPos2.z;
					position = DirectX::XMLoadFloat3(&(tempPos1));
					Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mCurrentSkill->GetParticleSystem(), &temp, position, rotation));
				}
			}

			// Set static bool to true to prevent repeating this effect
			particleTrigger = true;
		}

		// Play animation when the timming is right
		if (!animeTrigger && mAnimator->getProgress() > mCurrentSkill->GetAnimationTiming())
		{
			// If its ment for the enemy party play the hit animation to time with the animation timing, else play the animations and particle effects to the hero party
			if (mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// For each party member
					for (Odyssey::Entity* c : enemies)
					{
						// If they arnt dead play thier animations and particle effects
						if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							// Play "Hit" animation
							c->getComponent<Odyssey::Animator>()->playClip("Hit"); 

							// Play particle effect
							c->getComponent<Character>()->SpawnBloodEffect();
							RedAudioManager::Instance().PlaySFX(c->getComponent<Character>()->GetSoundClipName("Hit").c_str());
						}
					}
					// Play audio "hit" sound effect
				}
				else if (mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD)
				{
					// Play "Hit" animation
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
					RedAudioManager::Instance().PlaySFX(mCurrentTarget->GetSoundClipName("Hit").c_str());
					// Play particle effect
					mCurrentTarget->SpawnBloodEffect();
				}
			}
			else
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// For each party member
					for (Odyssey::Entity* c : heros)
					{
						// If the target is not dead, and not the caster
						if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD && c->getComponent<HeroComponent>() != this)
						{
							// Play "GotBuffed" animation
							c->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
							//RedAudioManager::Instance().PlaySFX(c->getComponent<Character>()->GetSoundClipName("Buffed").c_str());
						}
					}
				}
				else if (mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD && mCurrentTarget != this)
				{
					// Play "GotBuffed" animation
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
					//RedAudioManager::Instance().PlaySFX(mCurrentTarget->GetSoundClipName("Buffed").c_str());
				}
			}
			// Set trigger to true to avoid repeating the recipents animation
			animeTrigger = true;
		}

		// Once the animation is nearly finished use the skill and apply the effects
		if (mAnimator->getProgress() > 0.9f)
		{
			// Depleate the caster mana
			DepleteMana(mCurrentSkill->GetManaCost());

			// If its ment for the enemies apply the effect to the enemy party, else apply the effect to the hero party
			if (mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// Temp character pointer to help loop through list of targets
					Character* temp = nullptr;

					// For each party member
					for (Odyssey::Entity* c : enemies)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c->getComponent<Character>();

							// If thier not dead, apply the skills effect to them
							if (temp->GetState() != STATE::DEAD)
								mCurrentSkill->Use(*this, *temp);
						}
					}
				}
				else if(mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD)
					mCurrentSkill->Use(*this, *mCurrentTarget);
			}
			else
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// Temp character pointer to help loop through list of targets
					Character* temp = nullptr;

					// For each party member
					for (Odyssey::Entity* c : heros)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c->getComponent<Character>();

							// If thier not dead, apply the skills effect to them
							if (temp->GetState() != STATE::DEAD)
								mCurrentSkill->Use(*this, *temp);
						}
					}
				}
				else if(mCurrentTarget != nullptr)
					mCurrentSkill->Use(*this, *mCurrentTarget);
			}

			// Reset static bools
			animeTrigger = false;
			particleTrigger = false;
			soundTrigger = false;

			// Set this characters state to finished unless im cheating
			if (!mIsCheating)
				mCurrentState = STATE::FINISHED;
			else
				ResetToSelection();
		}
		break;
	}

	// Once here loop through remaining status effects and reset current state
	case STATE::FINISHED:
	{
		// Turns off all targeting and 
		ResetToSelection();

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

		// Get the list of skill bg rectangles
		std::vector<Odyssey::Rectangle2D*> bgRects = GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSkillBackgroundList();
		// Turn off the skill selected indicator rectangles
		for (int i = 0; i < 4; i++)
		{
			bgRects[i]->setVisible(false);
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
void HeroComponent::Die()
{
	// Play death sound effect
	RedAudioManager::Instance().PlaySFX(mSoundClips["Death"].c_str());

	// Clear all remaining status effects
	ClearStatusEffects();
	
	// Play the death animation
	mAnimator->playClip("Dead");

	// Stop all active particle effects
	//StopParticleEffects();

	// Set state to dead
	mCurrentState = STATE::DEAD;
}

// Returns the characters skill list
std::vector<std::shared_ptr<Skills>> HeroComponent::GetSkills()
{
	return mSkillList;
}
 
// Returns this hero hero id
GameplayTypes::HEROID HeroComponent::GetID()
{
	return mID;
}

// Dumb event
void HeroComponent::ClickOnEnemy(SetNewTargetEvent* targetIndex)
{
	if (this->mCurrentState == STATE::SELECTTARGET)
	{
		int temp = static_cast<int>(targetIndex->mPlayerToTarget);
		if (mCurrentSkill->GetSkillTypeId() != GameplayTypes::SKILLTYPE::ATTACK && mCurrentSkill->GetSkillTypeId() != GameplayTypes::SKILLTYPE::DEBUFF && temp < 3)
		{
			mCurrentTarget = mHeroList[temp]->getComponent<Character>();
			if (mCurrentTarget->GetState() == STATE::DEAD)
				return;
			for (Odyssey::Entity* t : mHeroList)
			{
				// If the entity if valid and the character is no dead, turn off targeter
				if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
					t->getComponent<Character>()->mImpactIndicator->setActive(false);
			}
			BeginAttack(mHeroList);
		}
		else if((mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == GameplayTypes::SKILLTYPE::DEBUFF) && temp > 2)
		{
			mCurrentTarget = mEnemyList[(temp - 3)]->getComponent<Character>();
			if (mCurrentTarget->GetState() == STATE::DEAD)
				return;
			for (Odyssey::Entity* t : mEnemyList)
			{
				// If the entity if valid and the character is no dead, turn off targeter
				if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
					t->getComponent<Character>()->mImpactIndicator->setActive(false);
			}
			BeginAttack(mEnemyList);
		}
	}

}

// Function that gets called to manage the state in which the player is selecting a skill to use
void HeroComponent::SelctionState(int moveIndex)
{
	// If i have enough mana to use the skill take me to the target selection state
	if (mSkillList[moveIndex]->GetManaCost() <= mCurrentMana)
	{
		// Set temp variable to the selected move
		mCurrentSkill = mSkillList[moveIndex].get();

		// Change state to target selection
		mCurrentState = STATE::SELECTTARGET;
	}
}

// Function that gets called to manage the state in which the player is selecting a target to use its skill on
bool HeroComponent::SelectTarget(EntityList targets, int& targetIndex)
{
	// Static refrence to old character
	static Character* prevChar = nullptr;

	//If its a single target move set the indicator over the first applicable party memeber, else target all apllicable party members 
	if (!mCurrentSkill->IsAOE())
	{
		// If left arrow key down, go up the party list
		if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::Left))
		{
			// Reduce the index
			targetIndex--;

			// If index is going out of bounds set it to size - 1
			if (targetIndex < 0)
				targetIndex = (targets.size() - 1);

			// Loop till i am over a applicable target again
			while (targets[targetIndex] == nullptr || targets[targetIndex]->getComponent<Character>()->GetState() == STATE::DEAD)
			{
				// Reduce the index
				targetIndex--;

				// If index is going out of bounds set it to size - 1
				if (targetIndex < 0)
					targetIndex = (targets.size() - 1);
			}
		}
		// If right arrow key down, go down the party list
		if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::Right))
		{
			// Increase the index
			targetIndex++;

			// If index is going out of bounds set it to 0
			if (targetIndex >= targets.size())
				targetIndex = 0;

			// Loop till i am over a applicable target again
			while (targets[targetIndex] == nullptr || targets[targetIndex]->getComponent<Character>()->GetState() == STATE::DEAD)
			{
				// Increase the index
				targetIndex++;

				// If index is going out of bounds set it to 0
				if (targetIndex >= targets.size())
					targetIndex = 0;
			}
		}

		// Set my temp target storage to the highleted target
		mCurrentTarget = targets[targetIndex]->getComponent<Character>();

		// Turn on thier targeter
		mCurrentTarget->mImpactIndicator->setActive(true);

		// if my prev targert doesnt equal my current target
		if (prevChar != mCurrentTarget)
		{
			// If there is an old target, turn off targeter
			if (prevChar != nullptr)
				prevChar->mImpactIndicator->setActive(false);

			// Set the old target to mt current one
			prevChar = mCurrentTarget;
		}
	}
	else if (mCurrentTarget == nullptr)
	{
		// If its an aoe skill then turn all applicable partys indicators on
		for (Odyssey::Entity* t : targets)
		{
			// If the entity is valid and  the character is not dead, turn on targeter
			if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
				t->getComponent<Character>()->mImpactIndicator->setActive(true);
		}

		// Set current target to something to avoid repeating this loop
		mCurrentTarget = targets[targetIndex]->getComponent<Character>();
	}

	// Spit out current targets stats
	if (mIsCheating && Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D8))
	{
		std::cout << "Name: " << mCurrentTarget->GetName().c_str() << std::endl;
		std::cout << "Attack: " << mCurrentTarget->GetAtk() << std::endl;
		std::cout << "Defense: " << mCurrentTarget->GetDef() << std::endl;
		std::cout << "Speed: " << mCurrentTarget->GetSpeed() << std::endl;
		std::cout << "Health: " << mCurrentTarget->GetHP() << std::endl;
		std::cout << "Mana: " << mCurrentTarget->GetMana() << std::endl;
	}

	// If enter is hit set state to in progress and begin playing animations for caster
	if (Odyssey::InputManager::getInstance().getKeyDown(KeyCode::Enter))
	{
		// Reset temp target storage
		prevChar = nullptr;

		// For each applicable entity turn off targeters
		for (Odyssey::Entity* t : targets)
		{
			// If the entity if valid and the character is no dead, turn off targeter
			if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
				t->getComponent<Character>()->mImpactIndicator->setActive(false);
		}

		// Que animations, and set variables for particle effect locations
		BeginAttack(targets);

		// Return true for the characters turn to be over
		return true;
	}

	// Return false while the player is taking thier turn 
	return false;
}

// Function that gets called to send the player back to the selection state
void HeroComponent::ResetToSelection()
{
	// For each valid entity 
	for (Odyssey::Entity* e : mEnemyList)
	{
		// Turn off targeter
		if (e != nullptr)
			e->getComponent<Character>()->mImpactIndicator->setActive(false);
	}

	// For each valid entity 
	for (Odyssey::Entity* h : mHeroList)
	{
		// Turn off targeter
		if(h != nullptr)
			h->getComponent<Character>()->mImpactIndicator->setActive(false);
	}
	// Reset temp target and skill variables to default
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;

	// Reset state to select move
	mCurrentState = STATE::SELECTMOVE;

	// Turn off all of the character's hud selected skill backgrounds
	for (int i = 0; i < 4; i++)
	{
		GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSkillBackgroundList()[i]->setVisible(false);
	}
}

// Function that sends the state into the inprogress state, queing animations, and setting variables for particle effect locations
void HeroComponent::BeginAttack(EntityList targets)
{
	// Play the skills animation
	mAnimator->playClip(mCurrentSkill->GetAnimationId());

	// Set up particle information

	// Set the current state to inprogress
	mCurrentState = STATE::INPROGRESS;
}

std::shared_ptr<Odyssey::Component> HeroComponent::clone() const
{
	return std::make_shared<HeroComponent>(*this);
}

// Init function called when object loads into scene
void HeroComponent::initialize()
{
	mAnimator = mEntity->getComponent<Odyssey::Animator>();
	Odyssey::EventManager::getInstance().subscribe(this, &HeroComponent::ClickOnEnemy);
}

void HeroComponent::onDestroy()
{
	for (int i = 0; i < mSkillSprites.size(); ++i)
	{
		mSkillSprites[i]->unregisterCallback("onMouseClick");
	}
}

// Skill Call backs for clickable UI
void HeroComponent::Skill1Callback()
{
	if (mCurrentState == STATE::SELECTMOVE || mCurrentState == STATE::SELECTTARGET)
	{
		// If i have enough mana to use the skill take me to the target selection state
		if (mSkillList[0]->GetManaCost() <= mCurrentMana)
		{
			ResetToSelection();
			// Set temp variable to the selected move
			mCurrentSkill = mSkillList[0].get();

			// Change state to target selection
			mCurrentState = STATE::SELECTTARGET;

			// Turn on the skill selected indicator rectangle
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSkillBackgroundList()[0]->setVisible(true);
		}
	}
}
void HeroComponent::Skill2Callback()
{
	if (mCurrentState == STATE::SELECTMOVE || mCurrentState == STATE::SELECTTARGET)
	{
		ResetToSelection();
		// If i have enough mana to use the skill take me to the target selection state
		if (mSkillList[1]->GetManaCost() <= mCurrentMana)
		{
			// Set temp variable to the selected move
			mCurrentSkill = mSkillList[1].get();

			// Change state to target selection
			mCurrentState = STATE::SELECTTARGET;

			// Turn on the skill selected indicator rectangle
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSkillBackgroundList()[1]->setVisible(true);
		}
	}
}
void HeroComponent::Skill3Callback()
{
	if (mCurrentState == STATE::SELECTMOVE || mCurrentState == STATE::SELECTTARGET)
	{
		ResetToSelection();
		// If i have enough mana to use the skill take me to the target selection state
		if (mSkillList[2]->GetManaCost() <= mCurrentMana)
		{
			// Set temp variable to the selected move
			mCurrentSkill = mSkillList[2].get();

			// Change state to target selection
			mCurrentState = STATE::SELECTTARGET;

			// Turn on the skill selected indicator rectangle
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSkillBackgroundList()[2]->setVisible(true);
		}
	}
}
void HeroComponent::Skill4Callback()
{
	if (mCurrentState == STATE::SELECTMOVE || mCurrentState == STATE::SELECTTARGET)
	{
		ResetToSelection();
		// If i have enough mana to use the skill take me to the target selection state
		if (mSkillList[3]->GetManaCost() <= mCurrentMana)
		{
			// Set temp variable to the selected move
			mCurrentSkill = mSkillList[3].get();

			// Change state to target selection
			mCurrentState = STATE::SELECTTARGET;

			// Turn on the skill selected indicator rectangle
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSkillBackgroundList()[3]->setVisible(true);
		}
	}
}
