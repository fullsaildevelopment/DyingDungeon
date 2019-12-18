#include "Character.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, Character)

void Character::initialize()
{
	onEnable();
    mAnimator = mGameObject->getComponent<Odyssey::Animator>();
}

void Character::update(double deltaTime)
{
    
}

bool Character::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> enemyTeam)
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
    //TODO calculate damage reduction here
	if (dmg > 0)
	{
		dmg = dmg - (dmg * mDefense);
		if (mShielding > 0)
		{
			dmg = dmg - mShielding;
			if (dmg <= 0)
			{
				mShielding = fabsf(dmg);
				dmg = 0;
			}
			else
				mShielding = 0;
		}
	}
    //Take Damage
    SetHP(GetHP() - dmg);
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
*Function:  SetSkills(string newWSkillList)
* --------------------
* Set the character's skill list
*
*returns : void
*/
void Character::SetSkills(Skills newSkillList)
{
	mSkillList[0] = newSkillList;
}

/*
*Function:  GetSkills()
* --------------------
* Gets the characters skill list.
*
*returns : Skills*
*/
Skills* Character::GetSkills()
{
	return mSkillList;
}

// Adds a new status effect to the list of status effects
void Character::AddStatusEffect(Buffs* newEffect)
{
	mStatusEffects.push_back(newEffect);
}

// Called at the end of the charaters turn, will call Bleed() if IsBleed(), will also remove buffs if they have expired reverting stats back to normal
void Character::ManageStatusEffects()
{
	std::vector<Buffs*>::iterator it;
	for (it = mStatusEffects.begin(); it != mStatusEffects.end();)
	{
		if((*it)->IsBleed())
			(*it)->Bleed();
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			if (!(*it)->IsBleed() && (*it)->GetEffectedStat() != STATS::Shd)
				(*it)->RevertEffect();
			else
			{
				bool dontLoseShield = false;
				for (int i = 0; i < mStatusEffects.size(); ++i)
				{
					if (mStatusEffects[i]->GetEffectedStat() == STATS::Shd && mStatusEffects[i] != (*it))
					{
						dontLoseShield = true;
						break;
					}
				}
				if (dontLoseShield == false)
					(*it)->RevertEffect();
			}
			delete((*it));
			(*it) = nullptr;
			it = mStatusEffects.erase(it);
		}
		else
			it++;
	}
}

// Clears all status effects from the Character
void Character::ClearStatusEffects()
{
	for (Buffs* b : mStatusEffects)
	{
		b->RevertEffect();
		delete(b);
		b = nullptr;
	}
	mStatusEffects.clear();
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
