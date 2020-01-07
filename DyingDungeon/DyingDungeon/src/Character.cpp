#include "Character.h"
#include "Entity.h"
#include "Transform.h"
#include "RedAudioManager.h"

CLASS_DEFINITION(Component, Character)

void Character::initialize()
{
	mAnimator = mEntity->getComponent<Odyssey::Animator>();
}

void Character::update(double deltaTime)
{

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
	std::vector<StatusEffect*>::iterator it;
	for (it = mSheilds.begin(); it != mSheilds.end();)
	{
		dmg -= (*it)->GetAmountOfEffect();
		if (dmg < 0.0f)
		{
			(*it)->SetAmountOfEffect(dmg * -1.0f);
			dmg = 0.0f;

			std::cout << 0 << " damage!" << std::endl;
			return;
		}
		else
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
			it = mSheilds.erase(it);
		}
	}
	//Take Damage
	SetHP(GetHP() - dmg);
	std::cout << dmg << " damage!" << std::endl;
}

// Gives the character health back 
void Character::ReceiveHealing(float healing)
{
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
}

// Returns the Defense mod
float Character::GetDef()
{
	return mDefense;
}

// Increases the Defense stat
void Character::IncreaseDef(float statIncrease)
{
	mDefense += statIncrease;
}

// Decreases the Defense stat
void Character::DecreaseDef(float statDecrease)
{
	mDefense -= statDecrease;
}

// Returns the Speed stat
float Character::GetSpeed()
{
	return mSpeed;
}

// Increases the Speed stat
void Character::IncreaseSpd(float statIncrease)
{
	mSpeed += statIncrease;
}

// Decreases the Speed stat
void Character::DecreaseSpd(float statDecrease)
{
	mSpeed -= statDecrease;
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

//Set if the character is stunned
void Character::SetStun(bool stun)
{
	mStunned = stun;
}

//Get if the character is stunned
bool Character::GetStun()
{
	return mStunned;
}

Character* Character::GetProvoked()
{
	return mProvoked;
}

void Character::SetProvoked(Character* provoker)
{
	mProvoked = provoker;
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
Skills** Character::GetSkills()
{
	return mSkillList;
}

// Adds a new status effect to the list of status effects
void Character::AddStatusEffect(StatusEffect* newEffect)
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
	default:
		break;
	}
}

// Called at the end of the charaters turn, will call Bleed() if IsBleed(), will also remove buffs if they have expired reverting stats back to normal
void Character::ManageStatusEffects(std::vector<StatusEffect*>& effectList)
{
	std::vector<StatusEffect*>::iterator it;
	for (it = effectList.begin(); it != effectList.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
			it = effectList.erase(it);
		}
		else
		{
			(*it)->Use();
			it++;
		}
	}
}

bool Character::ManageAllEffects()
{
	std::vector<StatusEffect*>::iterator it;
	for (it = mBleeds.begin(); it != mBleeds.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
			it = mBleeds.erase(it);
		}
		else
			it++;
	}
	for (it = mRegens.begin(); it != mRegens.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
			it = mRegens.erase(it);
		}
		else
			it++;
	}
	if (mCurrentHP <= 0.0f)
	{
		mCurrentState = STATE::DEAD;
		return false;
	}
	for (it = mBuffs.begin(); it != mBuffs.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
			it = mBuffs.erase(it);
		}
		else
			it++;
	}
	for (it = mDebuffs.begin(); it != mDebuffs.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
			it = mDebuffs.erase(it);
		}
		else
			it++;
	}
	for (it = mSheilds.begin(); it != mSheilds.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			delete((*it));
			(*it) = nullptr;
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
	for (StatusEffect* se : mDebuffs)
	{
		se->Remove();
		delete(se);
		se = nullptr;
	}
	mDebuffs.clear();
	for (StatusEffect* se : mBuffs)
	{
		se->Remove();
		delete(se);
		se = nullptr;
	}
	mBuffs.clear();
	for (StatusEffect* se : mBleeds)
	{
		se->Remove();
		delete(se);
		se = nullptr;
	}
	mBleeds.clear();
	for (StatusEffect* se : mRegens)
	{
		se->Remove();
		delete(se);
		se = nullptr;
	}
	mRegens.clear();
	for (StatusEffect* se : mSheilds)
	{
		se->Remove();
		delete(se);
		se = nullptr;
	}
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
