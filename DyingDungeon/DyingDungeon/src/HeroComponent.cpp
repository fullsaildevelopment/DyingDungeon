#include "HeroComponent.h"
#include "Entity.h"
#include "Transform.h"
#include "EnemyComponent.h"
#include "MeshRenderer.h"
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
	mDead = false;
	SetHero(true);
	mEXP = 0;
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
		mShielding = 0.0f;
		for (int i = 0; i < TOTALSKILLS; ++i)
		// Basic Attack (Add Provoke 30% chance)
		temp = std::make_shared<Provoked>(1, this, nullptr);
		mSkillList.push_back(std::make_shared<Attack>("Basic Attack", "BasicAttack", 0.40f, -5.0f, 15.0f, temp));
		// Skill 1 Judgement (deal damage and heal self)
		mSkillList.push_back(std::make_shared<Attack>("Judgement", "BigAttack", 0.25f, 15.0f, 200.0f, 25.0f));
		// Skill 2 Shield of Light (Gives the team 25 temp hp with a shield)
		temp = std::make_shared<Shields>(25.0f, 3, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>("Shield of Light", "Heal", 0.25f, 20.0f, temp, true, true));
		// Skill 3 Blessing of light (Gives the team 50% damage reduction for 2 turns)
		temp = std::make_shared<StatUp>(1.0f, 3, STATS::Def, nullptr);
		mSkillList.push_back(std::make_shared<Buffs>("Blessing of Light", "Defense", 0.25f, 15.0f,temp,true,true));
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
		//If the player is stunned manage all his effects and exit the loop.
	case STATE::STUNNED:
	{
		mCurrentState = STATE::NONE;
		ManageAllEffects();
		return true;
	}
		//If the player is not stunned enter the start of his turn, all bleed and regen dots will tick. 
		//If the players character dies he will have his state set to dead, else he will start to select his move.
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
		//Here the player will be able to select from his four options for skills
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
		//If the player character's selection is not Aoe or currently provoked they will then select thier target
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
		//Player will confirm that this is thier desired move
	case STATE::CONFIRM:
	{
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::D1))
		{
			mAnimator->playClip(mCurrentSkill->GetAnimationId());
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
		static bool trigger = false;
		if (!trigger && mAnimator->getProgress() > mCurrentSkill->GetAnimationTiming())
		{
			// If its ment for the enemies play the hit animation to time with the animation timing
			if (mCurrentSkill->GetTypeId() == SKILLTYPE::ATTACK || mCurrentSkill->GetTypeId() == SKILLTYPE::DEBUFF)
			{
				if (mCurrentSkill->IsAOE())
				{
					for (std::shared_ptr<Odyssey::Entity> c : enemies)
					{
						if(c != nullptr)
							c.get()->getComponent<Odyssey::Animator>()->playClip("Hit");
					}
				}
				else if(mCurrentTarget != nullptr)
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("Hit");
			}
			else
			{
				// If its ment for the players characters play the recived buff animation to time with the animation timing
				if (mCurrentSkill->IsAOE())
				{
					for (std::shared_ptr<Odyssey::Entity> c : heros)
					{
						if(c != nullptr && c.get()->getComponent<HeroComponent>() != this)
							c.get()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
					}
				}
				else if (mCurrentTarget != nullptr)
					mCurrentTarget->getEntity()->getComponent<Odyssey::Animator>()->playClip("GotBuffed");
			}
			// Set trigger to true to avoid looping the recipents animation
			trigger = true;
		}
		// Once the animation is nearly finished use the skill and apply the effects
		if (mAnimator->getProgress() > 0.9f)
		{
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
			trigger = false;
			mCurrentState = STATE::FINISHED;			
		}
	}
	break;
		// once here loop through remaining status effects and reset current state
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
				for(std::shared_ptr<Odyssey::Entity> e : enemies)
					e.get()->getComponent<Character>()->GetInpactIndicator()->getComponent<Odyssey::MeshRenderer>()->setActive(true);
			}
			else
			{
				std::cout << "This will affect your entire party. Press 1 to confirm, escape to go back." << std::endl;
				for (std::shared_ptr<Odyssey::Entity> h : heros)
					h.get()->getComponent<Character>()->GetInpactIndicator()->getComponent<Odyssey::MeshRenderer>()->setActive(true);
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
	mCurrentTarget->GetInpactIndicator()->getComponent<Odyssey::MeshRenderer>()->setActive(true);
	mCurrentState = STATE::CONFIRM;
}
void HeroComponent::ResetToSelection(EntityList heros, EntityList enemies)
{
	for (std::shared_ptr<Odyssey::Entity> e : enemies)
		e.get()->getComponent<Character>()->GetInpactIndicator()->getComponent<Odyssey::MeshRenderer>()->setActive(false);
	for (std::shared_ptr<Odyssey::Entity> h : heros)
		h.get()->getComponent<Character>()->GetInpactIndicator()->getComponent<Odyssey::MeshRenderer>()->setActive(false);
	mCurrentSkill = nullptr;
	mCurrentTarget = nullptr;
	mCurrentState = STATE::SELECTMOVE;
	std::cout << "Reselect A Skill..." << std::endl;
}
