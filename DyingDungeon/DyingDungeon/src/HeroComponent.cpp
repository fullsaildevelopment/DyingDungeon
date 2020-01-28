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
#include <memory>

CLASS_DEFINITION(Character, HeroComponent)

// Constructor
HeroComponent::HeroComponent(HEROID id)
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
	mBloodParticleEffect = nullptr;
	////////////////////////////////////////////////

	// Temp variable for creating status effects
	std::shared_ptr<StatusEffect> temp;
	
	// Switch statment that builds the hero depending on the hero id that gets passed in the constructor
	switch (id)
	{
	case HEROID::Paladin:
	{
		// Set the character name
		mName = L"Paladin";

		// Set the character subname
		mSubName = L"Tank";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 150.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 100.0f;

		// Set the stats for the character //
		////////////////////////////////////
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.30f;
		mBaseSpeed = mSpeed = 35.0f;
		////////////////////////////////////

		// Make the character skills //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Basic Attack, Provoke
		temp = std::make_shared<Provoked>(2, this, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "BasicAttack", 0.47f, -5.0f, 15.0f, temp));
		// Skill 1 Judgement (deal damage and heal self)
		mSkillList.push_back(std::make_shared<Attack>(L"Judgement", "BigAttack", 0.50f, 15.0f, 200.0f, 25.0f));
		// Skill 2 Shield of Light (Gives the team 25 temp hp with a shield)
		temp = std::make_shared<Shields>(25.0f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>(L"Shield of Light", "Heal", 0.89f, 20.0f, temp, true, true));
		// Skill 3 Blessing of light (Gives the team 50% damage reduction for 2 turns)
		temp = std::make_shared<StatUp>(1.0f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>(L"Blessing of Light", "Defense", 0.89f, 15.0f,temp,true, true));
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}
	case HEROID::Mage:
	{
		// Set the character name
		mName = L"Mage";

		// Set the character subname
		mSubName = L"Dps";

		// Set the base HP and current HP
		mBaseMaxHP = mCurrentHP = 100.0f;

		// Set the base Mana and current Mana
		mBaseMaxMana = mCurrentMana = 150.0f;

		// Set the stats for the character //
		////////////////////////////////////
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.10f;
		mBaseSpeed = mSpeed = 40.0f;
		////////////////////////////////////

		// Make the character skills //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Basic attack, stun
		temp = std::make_shared<Stun>(1,nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Basic Attack", "OneHandedCast", 0.60f, -10.0f, 10.0f,temp));
		// Wind Slash, aoe dps, speed down 
		temp = std::make_shared<StatDown>(0.5f,2,STATS::Spd,nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"Wind Slash", "OneHandedCast", 0.25f, 10.0f, 15.0f, temp, true));
		// Fire sTrom BIIIIGGGGG DPS with bleed
		temp = std::make_shared<Bleed>(0.10f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Attack>(L"FireStorm", "TwoHandedCast", 0.60f, 30.0f, 50.0f, temp, true));
		// Lighting Bolt BIGGGGG siongle target dps
		mSkillList.push_back(std::make_shared<Attack>(L"Lightning Bolt", "TwoHandedCast", 0.25f, 35.0f, 60.0f));
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}
	case HEROID::Bard:
	{
		// Set the character name
		mName = L"TheBestClassToEverExist";

		// Set the character subname
		mSubName = L"Literal God";

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

		// Make the character skills //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		break;
	}
	default:
		break;
	}
}

// Destructor
HeroComponent::~HeroComponent()
{
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
		mCurrentState = STATE::NONE;
		ManageAllEffects();
		return true;
	}

	// If the player is not stunned enter the start of his turn, all bleed and regen dots will tick. 
	// If the players character dies he will have his state set to dead, else he will start to select his move.
	case STATE::NONE:
	{
		ManageStatusEffects(mRegens);
		ManageStatusEffects(mBleeds);
		if (mCurrentHP <= 0.0f)
			Die();
		else
			mCurrentState = STATE::SELECTMOVE;
		break;
	}

	// Here the player will be able to select from his four options for skills
	case STATE::SELECTMOVE:
	{
		// Use ability 1
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			SelctionState(heros, enemies, 0);
		}

		// Use ability 2
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D2))
		{
			SelctionState(heros, enemies, 1);
		}

		// Use ability 3
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D3))
		{
			SelctionState(heros, enemies, 2);
		}

		// Use ability 4
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D4))
		{
			SelctionState(heros, enemies, 3);
		}

		// Reset back to move selection
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			ResetToSelection(heros, enemies);
		}
		break;
	}

	// If the player character's selection is not Aoe or not currently provoked they will then select thier target
	case STATE::SELECTTARGET:
	{
		// Static int used for targeting
		static int tempIndex = -1;
		// If an attack or debuff highlight applicable enemy target else look for applicable hero to target
		if (mCurrentSkill->GetSkillTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == SKILLTYPE::DEBUFF)
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
			ResetToSelection(heros, enemies);
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

		// Fire particle effects when the timming is right
		if (mCurrentSkill->GetParticleSystem() != nullptr && !particleTrigger && mAnimator->getProgress() > mCurrentSkill->GetPSFiringTime())
		{
			// Turn particle effect on
			mCurrentSkill->GetParticleSystem()->play();

			// If its a projectile particle effect turn on its mover
			if(!mCurrentSkill->IsAOE())
				mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->setActive(true);

			// Set static bool to true to prevent repeating this effect
			particleTrigger = true;
		}

		// Play animation when the timming is right
		if (!animeTrigger && mAnimator->getProgress() > mCurrentSkill->GetAnimationTiming())
		{
			// If its ment for the enemy party play the hit animation to time with the animation timing, else play the animations and particle effects to the hero party
			if (mCurrentSkill->GetSkillTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : enemies)
					{
						// If they arnt dead play thier animations and particle effects
						if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							// Play "Hit" animation
							c.get()->getComponent<Odyssey::Animator>()->playClip("Hit"); 

							// Set up particle effect location
							DirectX::XMFLOAT3 t = c.get()->getComponent<Odyssey::Transform>()->getPosition();
							c.get()->getComponent<Character>()->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x,t.y,t.z);

							// Play particle effect
							c.get()->getComponent<Character>()->GetPSBlood()->play();
						}
					}
				}
				else if (mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD)
				{
					// Play "Hit" animation
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");

					// Set up particle effect location
					DirectX::XMFLOAT3 t = mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
					mCurrentTarget->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);

					// Play particle effect
					mCurrentTarget->GetPSBlood()->play();
				}
			}
			else
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : heros)
					{
						// If the target is not dead, and not the caster
						if (c != nullptr && c.get()->getComponent<Character>()->GetState() != STATE::DEAD && c.get()->getComponent<HeroComponent>() != this)
						{
							// Play "GotBuffed" animation
							c.get()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
						}
					}
				}
				else if (mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD && mCurrentTarget != this)
				{
					// Play "GotBuffed" animation
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
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
			if (mCurrentSkill->GetSkillTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == SKILLTYPE::DEBUFF)
			{
				// If the skill is aoe hit the whole party with it, otherwise hit just the intended target
				if (mCurrentSkill->IsAOE())
				{
					// Temp character pointer to help loop through list of targets
					Character* temp = nullptr;

					// For each party member
					for (std::shared_ptr<Odyssey::Entity> c : enemies)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c.get()->getComponent<Character>();

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
					for (std::shared_ptr<Odyssey::Entity> c : heros)
					{
						// If the entity is valid
						if (c != nullptr)
						{
							// Get the character from the entity
							temp = c.get()->getComponent<Character>();

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

			// Set this characters state to finished
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

		// Turns off all targeting and 
		ResetToSelection(heros, enemies);

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
void HeroComponent::Die()
{
	// Set state to dead
	mCurrentState = STATE::DEAD;

	// Clear all remaining status effects
	ClearStatusEffects();
	
	// Play the death animation
	mAnimator->playClip("Dead");
}

// Returns the characters skill list
std::vector<std::shared_ptr<Skills>> HeroComponent::GetSkills()
{
	return mSkillList;
}

// Function that gets called to manage the state in which the player is selecting a skill to use
void HeroComponent::SelctionState(EntityList heros, EntityList enemies, int moveIndex)
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
		for (std::shared_ptr<Odyssey::Entity> t : targets)
		{
			// If the entity is valid and  the character is not dead, turn on targeter
			if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
				t->getComponent<Character>()->mImpactIndicator->setActive(true);
		}

		// Set current target to something to avoid repeating this loop
		mCurrentTarget = targets[targetIndex]->getComponent<Character>();
	}

	// If enter is hit set state to in progress and begin playing animations for caster
	if (Odyssey::InputManager::getInstance().getKeyDown(KeyCode::Enter))
	{
		// Set state to inprogress
		mCurrentState = STATE::INPROGRESS;

		// Reset temp target storage
		prevChar = nullptr;

		// For each applicable entity turn off targeters
		for (std::shared_ptr<Odyssey::Entity> t : targets)
		{
			// If the entity if valid and the character is no dead, turn off targeter
			if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
				t->getComponent<Character>()->mImpactIndicator->setActive(false);
		}

		// 
		BeginAttack(targets);
		return true;
	}
	return false;
}

// Function that gets called to send the player back to the selection state
void HeroComponent::ResetToSelection(EntityList heros, EntityList enemies)
{
	for (std::shared_ptr<Odyssey::Entity> e : enemies)
	{
		if (e != nullptr)
			e->getComponent<Character>()->mImpactIndicator->setActive(false);
	}
	for (std::shared_ptr<Odyssey::Entity> h : heros)
	{
		if(h != nullptr)
			h->getComponent<Character>()->mImpactIndicator->setActive(false);
	}
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	mCurrentState = STATE::SELECTMOVE;
	std::cout << "Reselect A Skill..." << std::endl;
}

// Function that sends the state into the inprogress state, queing animations, and setting varia bles for particle effect locations
void HeroComponent::BeginAttack(EntityList targets)
{
	mAnimator->playClip(mCurrentSkill->GetAnimationId());
	if (mCurrentSkill->IsAOE() && mCurrentSkill->GetParticleSystem() != nullptr)
	{
		DirectX::XMFLOAT3 aoeSpawn(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 tempTransform(0.0f, 0.0f, 0.0f);
		if (mCurrentSkill->GetSkillTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetSkillTypeId() == SKILLTYPE::DEBUFF)
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
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(aoeSpawn.x,aoeSpawn.y,aoeSpawn.z);
	}
	else if (mCurrentTarget != nullptr && mCurrentSkill->GetParticleSystem() != nullptr)
	{
		// Set position to start in desired postion
		DirectX::XMFLOAT3 temp(mEntity->getComponent<Odyssey::Transform>()->getPosition());
		temp.x += mCurrentSkill->GetPosOffset().x;
		temp.y += mCurrentSkill->GetPosOffset().y;
		temp.z += mCurrentSkill->GetPosOffset().z;
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetOrigin(temp);
		// Set target position
		DirectX::XMFLOAT3 temp2(mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition());
		temp2.y += 3.0f;
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetTargetPos(temp2);
		// Use velocity to calc lifetime
		DirectX::XMFLOAT3 tempVelocity = mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->GetVelocity();
		float tempLifeTime = sqrtf((powf((temp2.x - temp.x), 2) + powf((temp2.y - temp.y), 2) + powf((temp2.z - temp.z), 2))) / sqrtf((powf(tempVelocity.x, 2) + powf(tempVelocity.y, 2) + powf(tempVelocity.z, 2)));
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetLifeTime(tempLifeTime + 0.1f);
	}
	mCurrentState = STATE::INPROGRESS;
}
