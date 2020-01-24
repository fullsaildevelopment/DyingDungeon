#include "HeroComponent.h"
#include "Entity.h"
#include "Transform.h"
#include "EnemyComponent.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
/// Check if better way
#include "Attack.h"
#include "Buffs.h"
#include "Heal.h"
//////////////////
#include "Bleed.h"
#include "StatUp.h"
#include "StatDown.h"
#include "Regens.h"
#include "Stun.h"
#include "Shields.h"
#include "Provoked.h"
#include <memory>

CLASS_DEFINITION(Character, HeroComponent)

HeroComponent::HeroComponent(HEROID id)
{
	SetHero(true);
	mEXP = 0;
	mShielding = 0.0f;
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	mCurrentState = STATE::NONE;
	mProvoked = nullptr;
	std::shared_ptr<StatusEffect> temp;
	switch (id)
	{
	case HEROID::Paladin:
	{
		mName = "Paladin";
		mBaseMaxHP = mCurrentHP = 150.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.30f;
		mBaseSpeed = mSpeed = 35.0f;
		// Basic Attack, Provoke
		temp = std::make_shared<Provoked>(2, this, nullptr);
		mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "BasicAttack", 0.47f, -5.0f, 15.0f, temp));
		// Skill 1 Judgement (deal damage and heal self)
		mSkillList.push_back(std::make_shared<Attack>("Judgement", "BigAttack", 0.50f, 15.0f, 200.0f, 25.0f));
		// Skill 2 Shield of Light (Gives the team 25 temp hp with a shield)
		temp = std::make_shared<Shields>(25.0f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>("Shield of Light", "Heal", 0.89f, 20.0f, temp, true, true));
		// Skill 3 Blessing of light (Gives the team 50% damage reduction for 2 turns)
		temp = std::make_shared<StatUp>(1.0f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>("Blessing of Light", "Defense", 0.89f, 15.0f,temp,true, true));
		break;
	}
	case HEROID::Mage:
	{
		mName = "Mage";
		mBaseMaxHP = mCurrentHP = 100.0f;
		mBaseMaxMana = mCurrentMana = 150.0f;
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.10f;
		mBaseSpeed = mSpeed = 40.0f;
		// Basic attack, stun
		temp = std::make_shared<Stun>(1,nullptr);
		mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "OneHandedCast", 0.60f, -10.0f, 10.0f,temp));
		// Wind Slash, aoe dps, speed down 
		temp = std::make_shared<StatDown>(0.5f,2,STATS::Spd,nullptr);
		mSkillList.push_back(std::make_shared<Attack>("Wind Slash", "OneHandedCast", 0.25f, 10.0f, 15.0f, temp, true));
		// Fire sTrom BIIIIGGGGG DPS with bleed
		temp = std::make_shared<Bleed>(0.10f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Attack>("FireStorm", "TwoHandedCast", 0.60f, 30.0f, 50.0f, temp, true));
		// Lighting Bolt BIGGGGG siongle target dps
		mSkillList.push_back(std::make_shared<Attack>("Lightning Bolt", "TwoHandedCast", 0.25f, 35.0f, 60.0f));
		break;
	}
	default:
		break;
	}
}
HeroComponent::~HeroComponent()
{
}

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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			SelctionState(heros, enemies, 0);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D2))
		{
			SelctionState(heros, enemies, 1);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D3))
		{
			SelctionState(heros, enemies, 2);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D4))
		{
			SelctionState(heros, enemies, 3);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			ResetToSelection(heros, enemies);
		}
		break;
	}
	// If the player character's selection is not Aoe or currently provoked they will then select thier target
	case STATE::SELECTTARGET:
	{
		static int tempIndex = -1;
		if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
		{
			if (tempIndex == -1)
			{
				for (int i = 0; i < enemies.size(); ++i)
				{
					if (enemies[i] != nullptr && enemies[i]->getComponent<Character>()->GetState() != STATE::DEAD)
					{
						tempIndex = i;
						break;
					}
				}
			}
			if(SelectTarget(enemies, tempIndex))
				tempIndex = -1;
		}
		else
		{
			if (tempIndex == -1)
			{
				for (int i = 0; i < heros.size(); ++i)
				{
					if (heros[i] != nullptr && heros[i]->getComponent<Character>()->GetState() != STATE::DEAD)
					{
						tempIndex = i;
						break;
					}
				}
			}
			if (SelectTarget(heros, tempIndex))
				tempIndex = -1;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			ResetToSelection(heros, enemies);
			tempIndex = -1;
		}
		break;
	}
	// The move animation has begun, this will run until the animation is over
	case STATE::INPROGRESS:
	{
		// Static bool used to track whenever its time to play the recipent animation ie hit or be buffed 
		static bool particleTrigger = false;
		static bool animeTrigger = false;
		if (mCurrentSkill->GetParticleSystem() != nullptr && !particleTrigger && mAnimator->getProgress() > mCurrentSkill->GetPSFiringTime())
		{
			// Turn particle effect on
			mCurrentSkill->GetParticleSystem()->play();
			mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->setActive(true);
			particleTrigger = true;
		}
		if (!animeTrigger && mAnimator->getProgress() > mCurrentSkill->GetAnimationTiming())
		{
			// If its ment for the enemies play the hit animation to time with the animation timing
			if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				if (mCurrentSkill->IsAOE())
				{
					for (std::shared_ptr<Odyssey::Entity> c : enemies)
					{
						if (c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
						{
							c.get()->getComponent<Odyssey::Animator>()->playClip("Hit"); 
							DirectX::XMFLOAT3 t = c.get()->getComponent<Odyssey::Transform>()->getPosition();
							c.get()->getComponent<Character>()->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x,t.y,t.z);
							c.get()->getComponent<Character>()->GetPSBlood()->play();
						}
					}
				}
				else if (mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD)
				{
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
					DirectX::XMFLOAT3 t = mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition();
					mCurrentTarget->GetPSBlood()->getEntity()->getComponent<Odyssey::Transform>()->setPosition(t.x, t.y, t.z);
					mCurrentTarget->GetPSBlood()->play();
				}
			}
			else
			{
				// If its ment for the players characters play the recived buff animation to time with the animation timing
				if (mCurrentSkill->IsAOE())
				{
					for (std::shared_ptr<Odyssey::Entity> c : heros)
					{
						if(c != nullptr && c.get()->getComponent<Character>()->GetState() != STATE::DEAD && c.get()->getComponent<HeroComponent>() != this)
							c.get()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
					}
				}
				else if (mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD && mCurrentTarget != this)
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
			}
			// Set trigger to true to avoid looping the recipents animation
			animeTrigger = true;
		}
		// Once the animation is nearly finished use the skill and apply the effects
		if (mAnimator->getProgress() > 0.9f)
		{
			DepleteMana(mCurrentSkill->GetManaCost());
			if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				if (mCurrentSkill->IsAOE())
				{
					Character* temp = nullptr;
					for (std::shared_ptr<Odyssey::Entity> c : enemies)
					{
						if (c != nullptr)
						{
							temp = c.get()->getComponent<Character>();
							if (temp->GetState() != STATE::DEAD)
								mCurrentSkill->Use(*this, *temp);
						}
					}
				}
				else if(mCurrentTarget != nullptr)
					mCurrentSkill->Use(*this, *mCurrentTarget);
			}
			else
			{
				if (mCurrentSkill->IsAOE())
				{
					Character* temp = nullptr;
					for (std::shared_ptr<Odyssey::Entity> c : heros)
					{
						if (c != nullptr)
						{
							temp = c.get()->getComponent<Character>();
							if (temp->GetState() != STATE::DEAD)
								mCurrentSkill->Use(*this, *temp);
						}
					}
				}
				else if(mCurrentTarget != nullptr)
					mCurrentSkill->Use(*this, *mCurrentTarget);
			}
			mCurrentSkill = nullptr;
			mCurrentTarget = nullptr;
			animeTrigger = false;
			particleTrigger = false;
			mCurrentState = STATE::FINISHED;			
		}
	}
	break;
		// Once here loop through remaining status effects and reset current state
	case STATE::FINISHED:
	{
		mCurrentState = STATE::NONE;
		ManageStatusEffects(mBuffs);
		ManageStatusEffects(mDebuffs);
		ManageStatusEffects(mSheilds);
		ResetToSelection(heros, enemies);
		return true;
		break;
	}
		// If here the character has died and will begin his death animation
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

void HeroComponent::Die()
{
	mCurrentState = STATE::DEAD;
	ClearStatusEffects();
	mAnimator->playClip("Dead");
	pTurnNumber->setText(L"X");
}

void HeroComponent::SelctionState(EntityList heros, EntityList enemies, int moveIndex)
{
	if (mSkillList[moveIndex]->GetManaCost() <= mCurrentMana)
	{
		mCurrentSkill = mSkillList[moveIndex].get();
		mCurrentState = STATE::SELECTTARGET;
	}
}

bool HeroComponent::SelectTarget(EntityList targets, int& targetIndex)
{
	// Static refrence to old character
	static Character* prevChar = nullptr;
	//If its a single target move set the indicator over the first applicable party memeber 
	if (!mCurrentSkill->IsAOE())
	{
		// If left then go up the party list
		if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::Left))
		{
			targetIndex--;
			if (targetIndex < 0)
				targetIndex = (targets.size() - 1);
			while (targets[targetIndex] == nullptr)
			{
				targetIndex--;
				if (targetIndex < 0)
					targetIndex = (targets.size() - 1);
			}
			while(targets[targetIndex]->getComponent<Character>()->GetState() == STATE::DEAD)
			{
				targetIndex--;	
				if (targetIndex < 0)
					targetIndex = (targets.size() - 1);
			}
		}
		// If right go down the party list
		if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::Right))
		{
			targetIndex++;
			if (targetIndex >= targets.size())
				targetIndex = 0;
			while (targets[targetIndex] == nullptr)
			{
				targetIndex++;
				if (targetIndex >= targets.size())
					targetIndex = 0;
			}
			while (targets[targetIndex]->getComponent<Character>()->GetState() == STATE::DEAD)
			{
				targetIndex++;
				if (targetIndex >= targets.size())
					targetIndex = 0;
			}

		}
		mCurrentTarget = targets[targetIndex]->getComponent<Character>();
		mCurrentTarget->mImpactIndicator->setActive(true);
		if (prevChar != mCurrentTarget)
		{
			if (prevChar != nullptr)
				prevChar->mImpactIndicator->setActive(false);
			prevChar = mCurrentTarget;
		}
	}
	else if (mCurrentTarget == nullptr)
	{
		// If its an aoe skill then turn all applicable partys indicators on
		for (std::shared_ptr<Odyssey::Entity> t : targets)
		{
			if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
				t->getComponent<Character>()->mImpactIndicator->setActive(true);
		}
		mCurrentTarget = targets[targetIndex]->getComponent<Character>();
	}
	// If enter is hit set state to in progress and begin playing animations for caster
	if (Odyssey::InputManager::getInstance().getKeyDown(KeyCode::Enter))
	{
		mCurrentState = STATE::INPROGRESS;
		prevChar = nullptr;
		for (std::shared_ptr<Odyssey::Entity> t : targets)
		{
			if (t != nullptr && t->getComponent<Character>()->GetState() != STATE::DEAD)
				t->getComponent<Character>()->mImpactIndicator->setActive(false);
		}
		BeginAttack(targets);
		return true;
	}
	return false;
}

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

void HeroComponent::BeginAttack(EntityList targets)
{
	mAnimator->playClip(mCurrentSkill->GetAnimationId());
	if (mCurrentSkill->IsAOE() && mCurrentSkill->GetParticleSystem() != nullptr)
	{
		DirectX::XMFLOAT3 aoeSpawn(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 tempTransform(0.0f, 0.0f, 0.0f);
		if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
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
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetLifeTime(10.0f);
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetOrigin(aoeSpawn);
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
		mCurrentSkill->GetParticleSystem()->getEntity()->getComponent<ParticleMover>()->SetLifeTime(tempLifeTime);
	}
	mCurrentState = STATE::INPROGRESS;
}
