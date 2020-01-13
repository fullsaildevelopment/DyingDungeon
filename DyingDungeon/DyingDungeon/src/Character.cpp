#include "Character.h"
#include "Entity.h"
#include "Transform.h"
#include "RedAudioManager.h"
#include "MeshRenderer.h"

CLASS_DEFINITION(Component, Character)

Character::Character()
{
	mHero = false;
	mDead = false;
	mDisplaying = false;
	mShielding = false;
	mAttack = 0.0f;
	mDefense = 0.0f;
	mBaseDefense = 0.0f;
	mSpeed = 0.15f;
	mBaseSpeed = 0.0f;
	mBaseMaxHP = 100.0f;
	mBaseMaxMana = 100.0f;
	mCurrentHP = 100.0f;
	mCurrentMana = 100.0f;
	mPrevHealth = 100.0f;
	mPrevMana = 100.0f;
	mEXP = 0.0f;
	mProvoked = nullptr;
	mAnimator = nullptr;
	pDmgText = nullptr;
	pHealthBar = nullptr;
	pManaBar = nullptr;
	pTurnNumber = nullptr;
	mCurrentState = STATE::NONE;
}

void Character::initialize()
{
	mAnimator = mEntity->getComponent<Odyssey::Animator>();
}

void Character::update(double deltaTime)
{
	if (mDisplaying)
	{
		pDmgText->addOpacity(static_cast<float>(-deltaTime) / 2.0f);
		if (pDmgText->getOpacity() == 0.0f)
		{
			mDisplaying = false;
		}
	}
}

bool Character::TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	return false;
}

/*
 * Function:  Die()
 * --------------------
 * Sets dead boolean to true
 * Sets animation to death animation
 *
 * returns: void
 */
void Character::Die()
{

}

/*
 * Function:  TakeDamage(float dmg)
 * --------------------
 * Pass in a float to deal damage to the character
 * Calculates any damage reduction (TODO)
 *
 * returns: void
 */
void Character::TakeDamage(float dmg)
{
	RedAudioManager::Instance().PlaySFX("PaladinHit");

	//TODO calculate damage reduction here
	dmg = dmg - (dmg * mDefense);
	// Shielding algorithim
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;
	for (it = mSheilds.begin(); it != mSheilds.end();)
	{
		dmg -= (*it)->GetAmountOfEffect();
		if (dmg < 0.0f)
		{
			(*it)->SetAmountOfEffect(dmg * -1.0f);
			dmg = 0.0f;
			++it;
		}
		else
		{
			(*it)->Remove();
			it = mSheilds.erase(it);
		}
	}
	//Take Damage
	SetHP(GetHP() - dmg);
	// TODO: FOR BUILD ONLY FIX LATER
	pDmgText->setText(std::to_wstring(dmg).substr(0,5));
	pDmgText->setColor(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	pDmgText->setOpacity(1.0f);
	mDisplaying = true;
	/////////////////////////////////
	std::cout << dmg << " damage!" << std::endl;
	if (mCurrentHP <= 0.0f)
		Die();
}

// Gives the character health back 
void Character::ReceiveHealing(float healing)
{
	// TODO: FOR BUILD ONLY FIX LATER
	pDmgText->setText(std::to_wstring(healing).substr(0, 5));
	pDmgText->setColor(DirectX::XMFLOAT3(0.0f, 255.0f, 0.0f));
	pDmgText->setOpacity(1.0f);
	mDisplaying = true;
	/////////////////////////////////
	SetHP(mCurrentHP + healing);
}

/*
 * Function:  DepleteMana()
 * --------------------
 * Reduces the current mana by the mana cost
 *
 * returns: void
 */
void Character::DepleteMana(float manaCost)
{
	//Special Conditions here

	//Reduce current mana
	SetMana(GetMana() - manaCost);
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the HP of the character
 *
 * returns: A float representing current HP
 */
float Character::GetHP()
{
	return mCurrentHP;
}

/*
 * Function:  SetHP(float HP)
 * --------------------
 * Sets the HP of the character to the passed in float
 *
 * returns: nothing
 */
void Character::SetHP(float HP)
{
	mPrevHealth = mCurrentHP;
	mCurrentHP = HP;
	if (mCurrentHP < 0)
		mCurrentHP = 0;
	else if (mCurrentHP > mBaseMaxHP)
		mCurrentHP = mBaseMaxHP;

	UpdateHealthBar();
}

float Character::GetMaxHP()
{
	return mBaseMaxHP;
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the mana of the character
 *
 * returns: A float representing mana
 */
float Character::GetMana()
{
	return mCurrentMana;
}

/*
 * Function:  SetMana(float Mana)
 * --------------------
 * Sets the Mana of the character to the passed in float
 *
 * returns: void
 */
void Character::SetMana(float Mana)
{
	mPrevMana = mCurrentMana;
	mCurrentMana = Mana;
	if (mCurrentMana < 0)
		mCurrentMana = 0;
	else if (mCurrentMana > mBaseMaxMana)
		mCurrentMana = mBaseMaxMana;

	UpdateManaBar();
}

float Character::GetMaxMana()
{
	return mBaseMaxMana;
}

// Returns the Attack mod
float Character::GetAtk()
{
	return mAttack;
}

// Increases the Attack stat
void Character::IncreaseAtk(float statIncrease)
{
	mAttack += statIncrease;
}

// Decreases the Attack stat
void Character::DecreaseAtk(float statDecrease)
{
	mAttack -= statDecrease;
	if (mAttack <= -1.0f)
		mAttack = -1.0f;
}

// Returns the Defense mod
float Character::GetDef()
{
	return mDefense;
}

float Character::GetBaseDef()
{
	return mBaseDefense;
}

// Increases the Defense stat
void Character::IncreaseDef(float statIncrease)
{
	mDefense += (mBaseDefense * statIncrease);
	if (mDefense > 1.0f)
		mDefense = 1.0f;
}

// Decreases the Defense stat
void Character::DecreaseDef(float statDecrease)
{
	mDefense -= (mBaseDefense * statDecrease);
}

// Returns the Speed stat
float Character::GetSpeed()
{
	return mSpeed;
}

float Character::GetBaseSpeed()
{
	return mBaseSpeed;
}

// Increases the Speed stat
void Character::IncreaseSpd(float statIncrease)
{
	mSpeed += (mBaseSpeed * statIncrease);
}

// Decreases the Speed stat
void Character::DecreaseSpd(float statDecrease)
{
	mSpeed -= (mBaseSpeed * statDecrease);
}


// Gets the shields
float Character::GetShielding()
{
	return mShielding;
}

// Add to the shield
void Character::SetShielding(float shield)
{
	mShielding = shield;
}

/*
 * Function:  IsDead()
 * --------------------
 * Check to see if the character is dead
 *
 * returns: bool
 */
bool Character::IsDead()
{
	return mDead;
}

/*
 * Function:  SetMana(float Mana)
 * --------------------
 * Set the dead staus of the character
 *
 * returns: void
 */
void Character::SetDead(bool deadStatus)
{
	mDead = deadStatus;
	if (deadStatus == true)
	{
		mCurrentState = STATE::DEAD;
	}
	else
	{
		mCurrentState = STATE::NONE;
	}
}

// Adds Exp to the charater
void Character::AddExp(float exp)
{
	mEXP += exp;
}

//Get the exp of the character
float Character::GetExp()
{
	return mEXP;
}

Character* Character::GetProvoked()
{
	return mProvoked;
}

void Character::SetProvoked(Character* provoker)
{
	mProvoked = provoker;
}

STATE Character::GetState()
{
	return mCurrentState;
}

void Character::SetState(STATE newState)
{
	mCurrentState = newState;
}

/*
 * Function:  IsHero()
 * --------------------
 * Gets the Hero staus of the character
 *
 * returns: bool
 */
bool Character::IsHero()
{
	return mHero;
}

/*
 * Function:  SetHero(bool heroStat)
 * --------------------
 * Set the Hero staus of the character
 *
 * returns: void
 */
void Character::SetHero(bool heroStat)
{
	mHero = heroStat;
}

/*
 * Function:  GetName()
 * --------------------
 * Gets the characters name
 *
 * returns: string
 */
std::string Character::GetName()
{
	return mName;
}

/*
 * Function:  SetName(string newName)
 * --------------------
 * Set the character's name
 *
 * returns: void
 */
void Character::SetName(std::string newName)
{
	mName = newName;
}

/*
*Function:  GetSkills()
* --------------------
* Gets the characters skill list.
*
*returns : Skills*
*/
std::vector<std::shared_ptr<Skills>> Character::GetSkills()
{
	return mSkillList;
}

// Adds a new status effect to the list of status effects
void Character::AddStatusEffect(std::shared_ptr<StatusEffect> newEffect)
{
	switch (newEffect->GetTypeId())
	{
	case EFFECTTYPE::Bleed:
	{
		mBleeds.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Regen:
	{
		mRegens.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::StatUp:
	{
		mBuffs.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::StatDown:
	{
		mDebuffs.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Stun:
	{
		mDebuffs.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Shield:
	{
		mSheilds.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Provoke:
	{
		mDebuffs.push_back(newEffect);
		break;
	}
	default:
		break;
	}
}

void Character::ManageStatusEffects(std::vector<std::shared_ptr<StatusEffect>>& effectList)
{
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;
	for (it = effectList.begin(); it != effectList.end();)
	{
		(*it)->Use();
		if (mCurrentState == STATE::DEAD)
			return;
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			it = effectList.erase(it);
		}
		else
			it++;
	}
}

bool Character::ManageAllEffects()
{
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;
	for (it = mRegens.begin(); it != mRegens.end();)
	{
		(*it)->Use();
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			it = mRegens.erase(it);
		}
		else
			it++;
	}
	for (it = mBleeds.begin(); it != mBleeds.end();)
	{
		(*it)->Use();
		if (mCurrentState == STATE::DEAD)
			return false;
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			it = mBleeds.erase(it);
		}
		else
			it++;
	}
	for (it = mBuffs.begin(); it != mBuffs.end();)
	{
		(*it)->Use();
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			it = mBuffs.erase(it);
		}
		else
			it++;
	}
	for (it = mDebuffs.begin(); it != mDebuffs.end();)
	{
		(*it)->Use();
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			it = mDebuffs.erase(it);
		}
		else
			it++;
	}
	for (it = mSheilds.begin(); it != mSheilds.end();)
	{
		(*it)->Use();
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			it = mSheilds.erase(it);
		}
		else
			it++;
	}
	return true;
}

// Clears all status effects from the Character
void Character::ClearStatusEffects()
{
	mDebuffs.clear();
	mBuffs.clear();
	mBleeds.clear();
	mRegens.clear();
	mSheilds.clear();
}


/*
*Function:  UpdateHealthBar()
* --------------------
* Sets the fill of the health bar for the character
*
*returns : void
*/
void Character::UpdateHealthBar()
{
	float fill = GetHP() / GetMaxHP();
	if (fill < 0.0f)
		fill = 0.0f;
	else if (fill > 1.0f)
		fill = 1.0f;

	pHealthBar->setFill(fill);
}

/*
*Function:  UpdateManaBar()
* --------------------
* Sets the fill of the mana bar for the character
*
*returns : void
*/
void Character::UpdateManaBar()
{
	float fill = GetMana() / GetMaxMana();
	if (fill < 0.0f)
		fill = 0.0f;
	else if (fill > 1.0f)
		fill = 1.0f;

	pManaBar->setFill(fill);
}
