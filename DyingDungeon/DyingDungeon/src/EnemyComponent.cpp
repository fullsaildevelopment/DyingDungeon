#include "EnemyComponent.h"
#include "Entity.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

EnemyComponent::EnemyComponent(ENEMYID _enemyID)
{
	SetHero(false);
	mMoveOverride = SKILLTYPE::UNDEFINED;
	mCurrentState = STATE::NONE;
	mMoves = AIMoves(static_cast<int>(_enemyID), this);
	switch (_enemyID)
	{
	case ENEMYID::Skeleton:
	{
		mName = "Skeleton";
		mBaseMaxHP = mCurrentHP = 100.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.0f;
		mBaseSpeed = mSpeed = 20.0f;
		mMoveOverride = SKILLTYPE::ATTACK;
		mMechPtr = nullptr;
		break;
	}
	case ENEMYID::Ganfaul:
	{
		mName = "Ganfaul";
		mBaseMaxHP = mCurrentHP = 300.0f;
		mBaseMaxMana = mCurrentMana = 300.0f;
		mAttack = 0.0f;
		mBaseDefense = mDefense = 0.25f;
		mBaseSpeed = mSpeed = 45.0f;
		mMoveOverride = SKILLTYPE::UNDEFINED;
		mMechPtr = &EnemyComponent::GanfaulPhaseMechanic;
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
			if (mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::DEBUFF)
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
				if (mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::DEBUFF)
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
			if (mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::DEBUFF)
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
	pTurnNumber->setText(L"X");
}

void EnemyComponent::BeginAttack(std::vector<std::shared_ptr<Odyssey::Entity>> targets)
{
	mAnimator->playClip(mMoves.GetMove()->skill->GetAnimationId());
	if (mMoves.GetMove()->skill->IsAOE() && mMoves.GetMove()->skill->GetParticleSystem() != nullptr)
	{
		DirectX::XMFLOAT3 aoeSpawn(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 tempTransform(0.0f, 0.0f, 0.0f);
		if (mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::ATTACK || mMoves.GetMove()->skill->GetTypeId() == SKILLTYPE::DEBUFF)
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

