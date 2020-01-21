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
		mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "OneHandedCast", 0.25f, -10.0f, 10.0f,temp));
		// Wind Slash, aoe dps, speed down 
		temp = std::make_shared<StatDown>(0.5f,2,STATS::Spd,nullptr);
		mSkillList.push_back(std::make_shared<Attack>("Wind Slash", "OneHandedCast", 0.25f, 10.0f, 15.0f, temp, true));
		// Fire sTrom BIIIIGGGGG DPS with bleed
		temp = std::make_shared<Bleed>(0.10f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Attack>("FireStorm", "TwoHandedCast", 0.25f, 30.0f, 50.0f, temp, true));
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
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			SelectTarget(heros,enemies,0);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D2))
		{
			SelectTarget(heros, enemies, 1);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D3))
		{
			SelectTarget(heros, enemies, 2);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D4))
		{
			SelectTarget(heros, enemies, 3);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			ResetToSelection(heros, enemies);
		}
		break;
	}
	// Player will confirm that this is thier desired move
	case STATE::CONFIRM:
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad0))
		{
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			std::shared_ptr<Odyssey::Entity> testMePls = mCurrentSkill->GetParticleSystem();
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetLifeTime(1000.0f);
			mCurrentSkill->GetParticleSystem()->setActive(true);
			mCurrentSkill->GetParticleSystem()->setVisible(true);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad8))
		{
			mAnimator->playClip(mCurrentSkill->GetAnimationId());
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad3))
		{
			DirectX::XMFLOAT3 temp(mCurrentSkill->GetPosOffset());
			temp.x += 0.1f;
			mCurrentSkill->SetParticleOffset(temp);
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad5))
		{
			DirectX::XMFLOAT3 temp(mCurrentSkill->GetPosOffset());
			temp.y += 0.1f;
			mCurrentSkill->SetParticleOffset(temp);
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad9))
		{
			DirectX::XMFLOAT3 temp(mCurrentSkill->GetPosOffset());
			temp.z += 0.1f;
			mCurrentSkill->SetParticleOffset(temp);
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad1))
		{
			DirectX::XMFLOAT3 temp(mCurrentSkill->GetPosOffset());
			temp.x -= 0.1f;
			mCurrentSkill->SetParticleOffset(temp);
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad2))
		{
			DirectX::XMFLOAT3 temp(mCurrentSkill->GetPosOffset());
			temp.y -= 0.1f;
			mCurrentSkill->SetParticleOffset(temp);
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad7))
		{
			DirectX::XMFLOAT3 temp(mCurrentSkill->GetPosOffset());
			temp.z -= 0.1f;
			mCurrentSkill->SetParticleOffset(temp);
			DirectX::XMFLOAT3 testing((mEntity->getComponent<Odyssey::Transform>()->getPosition()));
			testing.x += mCurrentSkill->GetPosOffset().x;
			testing.y += mCurrentSkill->GetPosOffset().y;
			testing.z += mCurrentSkill->GetPosOffset().z;
			mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(testing);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad4))
		{
			mCurrentSkill->SetParticleFiringTime(mCurrentSkill->GetPSFiringTime() + 0.1f);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::NumPad6))
		{
			mCurrentSkill->SetParticleFiringTime(mCurrentSkill->GetPSFiringTime() - 0.1f);
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D0))
		{
			mAnimator->playClip(mCurrentSkill->GetAnimationId());
			if (mCurrentSkill->IsAOE())
			{
				DirectX::XMFLOAT3 aoeSpawn(0.0f,0.0f,0.0f);
				DirectX::XMFLOAT3 tempTransform(0.0f,0.0f,0.0f);
				if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
				{
					for (std::shared_ptr<Odyssey::Entity> c : enemies)
					{
						tempTransform = c->getComponent<Odyssey::Transform>()->getPosition();
						aoeSpawn.x += tempTransform.x;
						aoeSpawn.y += tempTransform.y;
						aoeSpawn.z += tempTransform.z;
					}
					aoeSpawn.x /= static_cast<float>(enemies.size());
					aoeSpawn.y /= static_cast<float>(enemies.size());
					aoeSpawn.z /= static_cast<float>(enemies.size());
				}
				else
				{
					for (std::shared_ptr<Odyssey::Entity> c : heros)
					{
						tempTransform = c->getComponent<Odyssey::Transform>()->getPosition();
						aoeSpawn.x += tempTransform.x;
						aoeSpawn.y += tempTransform.y;
						aoeSpawn.z += tempTransform.z;
					}
					aoeSpawn.x /= static_cast<float>(heros.size());
					aoeSpawn.y /= static_cast<float>(heros.size());
					aoeSpawn.z /= static_cast<float>(heros.size());
				}
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetLifeTime(10.0f);
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(aoeSpawn);
				// Turn particle effect on
				mCurrentSkill->GetParticleSystem()->setActive(true);
				mCurrentSkill->GetParticleSystem()->setVisible(true);
			}
			else if (mCurrentSkill->GetParticleSystem() != nullptr)
			{
				// Set position to start in desired postion
				DirectX::XMFLOAT3 temp(mEntity->getComponent<Odyssey::Transform>()->getPosition());
				temp.x += mCurrentSkill->GetPosOffset().x;
				temp.y += mCurrentSkill->GetPosOffset().y;
				temp.z += mCurrentSkill->GetPosOffset().z;
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(temp);
				// Set target position
				DirectX::XMFLOAT3 temp2(mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition());
				temp2.y += 3.0f;
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetTargetPos(temp2);
				// Use velocity to calc lifetime
				DirectX::XMFLOAT3 tempVelocity = mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->GetVelocity();
				float tempLifeTime = sqrtf((powf((temp2.x - temp.x), 2) + powf((temp2.y - temp.y), 2) + powf((temp2.z - temp.z), 2))) / sqrtf((powf(tempVelocity.x, 2) + powf(tempVelocity.y, 2) + powf(tempVelocity.z, 2)));
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetLifeTime(tempLifeTime);
				// Turn particle effect on
				mCurrentSkill->GetParticleSystem()->setActive(true);
				mCurrentSkill->GetParticleSystem()->setVisible(true);
			}
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			mAnimator->playClip(mCurrentSkill->GetAnimationId());
			if (mCurrentTarget != nullptr && mCurrentSkill->GetParticleSystem() != nullptr)
			{
				// Set position to start in desired postion
				DirectX::XMFLOAT3 temp(mEntity->getComponent<Odyssey::Transform>()->getPosition());
				temp.x += mCurrentSkill->GetPosOffset().x;
				temp.y += mCurrentSkill->GetPosOffset().y;
				temp.z += mCurrentSkill->GetPosOffset().z;
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetOrigin(temp);
				// Set target position
				DirectX::XMFLOAT3 temp2(mCurrentTarget->getEntity()->getComponent<Odyssey::Transform>()->getPosition());
				temp2.y += 3.0f;
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetTargetPos(temp2);
				// Use velocity to calc lifetime
				DirectX::XMFLOAT3 tempVelocity = mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->GetVelocity();
				float tempLifeTime = sqrtf((powf((temp2.x - temp.x), 2) + powf((temp2.y - temp.y), 2) + powf((temp2.z - temp.z), 2))) / sqrtf((powf(tempVelocity.x, 2) + powf(tempVelocity.y, 2) + powf(tempVelocity.z, 2)));
				mCurrentSkill->GetParticleSystem()->getComponent<ParticleMover>()->SetLifeTime(tempLifeTime);
			}
			mCurrentState = STATE::INPROGRESS;
		}
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Escape))
		{
			ResetToSelection(heros, enemies);
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
			mCurrentSkill->GetParticleSystem()->setActive(true);
			mCurrentSkill->GetParticleSystem()->setVisible(true);
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
						if(c != nullptr && c->getComponent<Character>()->GetState() != STATE::DEAD)
							c.get()->getComponent<Odyssey::Animator>()->playClip("Hit");
					}
				}
				else if(mCurrentTarget != nullptr && mCurrentTarget->GetState() != STATE::DEAD)
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
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
		std::cout << mCurrentSkill->GetName() << " Selected" << std::endl;
		if (mCurrentSkill->IsAOE())
		{
			if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				std::cout << "This will hit the entire enemy party. Press 1 to confirm, escape to go back." << std::endl;
				for (std::shared_ptr<Odyssey::Entity> e : enemies)
				{
					if(e != nullptr && e->getComponent<Character>()->GetState() != STATE::DEAD)
						e->getComponent<Character>()->mImpactIndicator->setActive(true);
				}
			}
			else
			{
				std::cout << "This will affect your entire party. Press 1 to confirm, escape to go back." << std::endl;
				for (std::shared_ptr<Odyssey::Entity> h : heros)
				{
					if (h != nullptr && h->getComponent<Character>()->GetState() != STATE::DEAD)
						h->getComponent<Character>()->mImpactIndicator->setActive(true);
				}
			}
			mCurrentState = STATE::CONFIRM;
		}
		else if (mProvoked == nullptr)
		{
			mCurrentState = STATE::SELECTTARGET;
		}
		else
		{
			mCurrentTarget = mProvoked;
			mCurrentState = STATE::CONFIRM;
			std::cout << "This will hit " << mCurrentTarget->GetName() << ". Press 1 to confirm, escape to go back." << std::endl;
		}
	}
	else
		std::cout << "You dont have enough mana for that move." << std::endl;
}

void HeroComponent::SelectTarget(EntityList heros, EntityList enemies, int targetIndex)
{
	if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
	{
		Character* possiableTarget = nullptr;
		possiableTarget = enemies[targetIndex]->getComponent<Character>();
		if (possiableTarget->GetHP() > 0.0f)
			mCurrentTarget = possiableTarget;
		else
			return;
		std::cout << "This will hit " << mCurrentTarget->GetName() << ". Press 1 to confirm, escape to go back." << std::endl;
	}
	else
	{
		Character* possiableTarget = nullptr;
		possiableTarget = heros[targetIndex]->getComponent<Character>();
		if (possiableTarget->GetHP() > 0.0f)
			mCurrentTarget = possiableTarget;
		else
			return;
		std::cout << "This will affect " << mCurrentTarget->GetName() << ". Press 1 to confirm, escape to go back." << std::endl;
	}
	mCurrentTarget->mImpactIndicator->setActive(true);
	mCurrentState = STATE::CONFIRM;
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
