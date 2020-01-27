#include "Character.h"
#include "Entity.h"
#include "Transform.h"
#include "RedAudioManager.h"
#include "MeshRenderer.h"
// Fix later
#include "StatUp.h"
#include "StatDown.h"
#include "Stun.h"

CLASS_DEFINITION(Component, Character)

// Constructor
Character::Character()
{
	// Setting default values for member variables //
	////////////////////////////////////////////////
	mCurrentState = STATE::NONE;
	mHero = false;
	mBaseMaxHP = mCurrentHP = 100.0f;
	mBaseMaxMana = mCurrentMana = 100.0f;
	mAttack = 0.0f;
	mDefense = mBaseDefense = 0.0f;
	mSpeed = mBaseSpeed = 0.0f;
	mEXP = 0.0f;
	mCurrentLevel = 0;
	mProvoked = nullptr;
	mAnimator = nullptr;
	pDmgText = nullptr;
	pHealthBar = nullptr;
	pManaBar = nullptr;
	pTurnNumber = nullptr;
	mDisplaying = false;
	mBigHpText = nullptr;
	mBloodParticleEffect = nullptr;
	mHpText = nullptr;
	mMpText = nullptr;
	////////////////////////////////////////////////
}

// Init for setting things up whenever the scene is initialized
void Character::initialize()
{
	mAnimator = mEntity->getComponent<Odyssey::Animator>();
}

// Called per frame when object is in a scene
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

// Called by battle instance whenever its time for this character to take its turn
bool Character::TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam)
{
	return false;
}

// Called whenever a charater needs to die setting the state to dead, and all other variables
void Character::Die()
{
	return;
}

// Some dumbass thing made by read, used by dumbass bryce
std::wstring Character::FormatToPercentageW(float number)
{
	if (number >= 100.0f)
	{
		return std::to_wstring(number).substr(0, 6);
	}
	else if (number >= 10.0f)
	{
		return std::to_wstring(number).substr(0, 5);
	}
	else
	{
		return std::to_wstring(number).substr(0, 4);
	}
}

// Called whenever this character needs to take damage
void Character::TakeDamage(float dmg)
{
	// Play sound effect for getting hit
	RedAudioManager::Instance().PlaySFX("PaladinHit");

	// Calculate damage reduction based on character drffense
	dmg = dmg - (dmg * mDefense);

	// loop through shield vector and reduce the incoming damage by amount of temp health this character has
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

	// Reduce health by the amount of damage that made it through
	SetHP(GetHP() - dmg);

	// Pop up battle text that appears over the character whenever something happens to them
	pDmgText->setText(std::to_wstring(dmg).substr(0,5));
	pDmgText->setColor(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
	pDmgText->setOpacity(1.0f);

	// BattleLogText shit that dumbass Bryce uses
	std::cout << dmg << " damage!" << std::endl;
	std::wstring dmgText = std::wstring(FormatToPercentageW(dmg));
	dmgText.append(L" damage!");
	GameUIManager::getInstance().SetBattleLogText(dmgText, true);

	// If they run out of Health kill the character
	if (mCurrentHP <= 0.0f)
		Die();
}

// Gives the character health back 
void Character::ReceiveHealing(float healing)
{
	// Add healing to current hp
	SetHP(mCurrentHP + healing);
	
	// Pop up battle text that appears over the character whenever something happens to them
	pDmgText->setText(std::to_wstring(healing).substr(0, 5));
	pDmgText->setColor(DirectX::XMFLOAT3(0.0f, 255.0f, 0.0f));
	pDmgText->setOpacity(1.0f);
	
	// Send off a dumbass event for reds dumbass stat tracking
	//Odyssey::EventManager::getInstance().publish(new CharacterRecivesHealingEvent(mName, healing));
}

// Called whenever this character needs to reduce its current mana
void Character::DepleteMana(float manaCost)
{
	// Reduce current mana
	SetMana(GetMana() - manaCost);
}

// Returns the characters current health
float Character::GetHP()
{
	return mCurrentHP;
}

// Sets the current HP of the character
void Character::SetHP(float HP)
{
	// Set the hp to the passed in amount
	mCurrentHP = HP;

	// if health is lower that 0.0f set it to 0.0f, if its over the max set it to the max
	if (mCurrentHP < 0.0)
		mCurrentHP = 0.0;
	else if (mCurrentHP > mBaseMaxHP)
		mCurrentHP = mBaseMaxHP;

	// Update the UI
	UpdateHealthBar();
}

// Returns the max HP of the character
float Character::GetMaxHP()
{
	return mBaseMaxHP;
}

// Returns the current mana of a character
float Character::GetMana()
{
	return mCurrentMana;
}

// Sets the current mana of the character
void Character::SetMana(float Mana)
{
	// Set the mp to the passed in amount
	mCurrentMana = Mana;

	// if mana is lower that 0.0f set it to 0.0f, if its over the max set it to the max
	if (mCurrentMana < 0.0f)
		mCurrentMana = 0.0f;
	else if (mCurrentMana > mBaseMaxMana)
		mCurrentMana = mBaseMaxMana;

	// Update the UI
	UpdateManaBar();
}

// Returns the max MP of the character
float Character::GetMaxMana()
{
	return mBaseMaxMana;
}

// Returns the Attack stat of the character
float Character::GetAtk()
{
	return mAttack;
}

// Increases the Attack stat of the character
void Character::IncreaseAtk(float statIncrease)
{
	mAttack += statIncrease;
}

// Decreases the Attack stat of the character
void Character::DecreaseAtk(float statDecrease)
{
	mAttack -= statDecrease;

	// If attack stat is lower than -1.0f set it to -1.0f
	if (mAttack <= -1.0f)
		mAttack = -1.0f;
}

// Returns the Defense stat of the character
float Character::GetDef()
{
	return mDefense;
}

// Returns the base Defense stat of the character
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
std::wstring Character::GetName()
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
void Character::SetName(std::wstring newName)
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
bool Character::AddStatusEffect(std::shared_ptr<StatusEffect> newEffect)
{
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;
	switch (newEffect->GetTypeId())
	{
	case EFFECTTYPE::Bleed:
	{
		for (it = mBleeds.begin(); it != mBleeds.end();)
		{
			if ((*it)->GetAmountOfEffect() == newEffect->GetAmountOfEffect())
			{
				(*it)->SetDuration(newEffect->GetDuration());
				return false;
			}
			else
				it++;
		}
		mBleeds.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Regen:
	{
		for (it = mRegens.begin(); it != mRegens.end();)
		{
			if ((*it)->GetAmountOfEffect() == newEffect->GetAmountOfEffect())
			{
				(*it)->SetDuration(newEffect->GetDuration());
				return false;
			}
			else
				it++;
		}
		mRegens.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::StatUp:
	{
		StatUp* temp = nullptr;
		for (it = mBuffs.begin(); it != mBuffs.end();)
		{
			if ((*it)->GetAffectedStatId() == (*it)->GetAffectedStatId() && (*it)->GetAmountOfEffect() == newEffect->GetAmountOfEffect())
			{
				(*it)->SetDuration(newEffect->GetDuration());
				return false;
			}
			else
				it++;
		}
		mBuffs.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::StatDown:
	{
		for (it = mDebuffs.begin(); it != mDebuffs.end();)
		{
			if ((*it)->GetAffectedStatId() == (*it)->GetAffectedStatId() && (*it)->GetAmountOfEffect() == newEffect->GetAmountOfEffect())
			{
				(*it)->SetDuration(newEffect->GetDuration());
				return false;
			}
			else
				it++;
		}
		mDebuffs.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Stun:
	{
		for (it = mDebuffs.begin(); it != mDebuffs.end();)
		{
			if ((*it)->GetAffectedStatId() == (*it)->GetAffectedStatId() && (*it)->GetAmountOfEffect() == newEffect->GetAmountOfEffect())
			{
				(*it)->SetDuration(newEffect->GetDuration());
				return false;
			}
			else
				it++;
		}
		mDebuffs.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Shield:
	{
		for (it = mSheilds.begin(); it != mSheilds.end();)
		{
			if ((*it)->GetAmountOfEffect() == newEffect->GetAmountOfEffect())
			{
				(*it)->SetDuration(newEffect->GetDuration());
				return false;
			}
			else
				it++;
		}
		mSheilds.push_back(newEffect);
		break;
	}
	case EFFECTTYPE::Provoke:
	{
		for (it = mDebuffs.begin(); it != mDebuffs.end();)
		{
			if ((*it)->GetAffectedStatId() == (*it)->GetAffectedStatId())
			{
				(*it)->Remove();
				it = mDebuffs.erase(it);
			}
			else
				it++;
		}
		mDebuffs.push_back(newEffect);
		break;
	}
	default:
		break;
	}
	return true;
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
	mHpText->setText(std::to_wstring((int)mCurrentHP));
	if (!mHero)
		mBigHpText->setText(std::to_wstring((int)mCurrentHP));
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
	mMpText->setText(std::to_wstring((int)mCurrentMana));
	float fill = GetMana() / GetMaxMana();
	if (fill < 0.0f)
		fill = 0.0f;
	else if (fill > 1.0f)
		fill = 1.0f;

	pManaBar->setFill(fill);
}

void Character::SetPSBlood(Odyssey::ParticleSystem* newBloodEffect)
{
	mBloodParticleEffect = newBloodEffect;
}

Odyssey::ParticleSystem* Character::GetPSBlood()
{
	return mBloodParticleEffect;
}

std::wstring Character::GetPortraitPath()
{
	return mPortrait;
}
