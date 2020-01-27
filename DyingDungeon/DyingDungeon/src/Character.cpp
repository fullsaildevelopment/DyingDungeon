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

// Increases the current Defense stat of the character
void Character::IncreaseDef(float statIncrease)
{
	mDefense += (mBaseDefense * statIncrease);

	// If defense stat is greater than 1.0f set it to 1.0f
	if (mDefense > 1.0f)
		mDefense = 1.0f;
}

// Decreases the current Defense stat of the character
void Character::DecreaseDef(float statDecrease)
{
	mDefense -= (mBaseDefense * statDecrease);
}

// Returns the current Speed stat of the character
float Character::GetSpeed()
{
	return mSpeed;
}

// Returns the base speed stat of the character
float Character::GetBaseSpeed()
{
	return mBaseSpeed;
}

// Increases the current Speed stat of the character
void Character::IncreaseSpd(float statIncrease)
{
	mSpeed += (mBaseSpeed * statIncrease);
}

// Decreases the current Speed stat of the character
void Character::DecreaseSpd(float statDecrease)
{
	mSpeed -= (mBaseSpeed * statDecrease);
}

// Adds Exp to the character
void Character::AddExp(float exp)
{
	mEXP += exp;
}

// Returns the exp of the character
float Character::GetExp()
{
	return mEXP;
}

// Returns the pointer that points to the character that provoked this character
Character* Character::GetProvoked()
{
	return mProvoked;
}

// Sets the characters provoked pointer to the character passed in
void Character::SetProvoked(Character* provoker)
{
	mProvoked = provoker;
}

// Returns the characters currennt state
STATE Character::GetState()
{
	return mCurrentState;
}

// Sets the characters current state
void Character::SetState(STATE newState)
{
	mCurrentState = newState;
}

// Gets the Hero staus of the character
bool Character::IsHero()
{
	return mHero;
}

// Sets the Hero staus of the character
void Character::SetHero(bool heroStat)
{
	mHero = heroStat;
}

// Returns the characters name
std::wstring Character::GetName()
{
	return mName;
}

// Sets the character's name
void Character::SetName(std::wstring newName)
{
	mName = newName;
}

// Returns the characters skill list
std::vector<std::shared_ptr<Skills>> Character::GetSkills()
{
	return mSkillList;
}

// Adds a status effect to the character and sorts it putting it in the correct vector
bool Character::AddStatusEffect(std::shared_ptr<StatusEffect> newEffect)
{
	// Make an iterator to check the characters list to see if it already has the passed in effect
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;

	// Switch statment for detirming what vector the effect needs to go into
	switch (newEffect->GetTypeId())
	{
	case EFFECTTYPE::Bleed:
	{
		// Loop through if i find the effect return false, else add to the vector of efffect
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

// Manages status effects of passed in vector of effects, appling effects and removing expired ones
void Character::ManageStatusEffects(std::vector<std::shared_ptr<StatusEffect>>& effectList)
{
	// For each status effect in the list, use its effect, reduce its duration, then remove if it expires.
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

// Manages all status effects, appling effects and removing expired ones
bool Character::ManageAllEffects()
{
	// For each status effect in the list, use its effect, reduce its duration, then remove if it expires. Repeate for all other status effect vectors.
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;

	//Regens
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

	// Bleeds
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

	// Buffs
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

	// Debuffs
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
	 
	// Shields
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

// Sets the fill of the health bar for the character
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

// Sets the fill of the mana bar for the character
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

// Sets the Particle system pointer to a "Hit effect"
void Character::SetPSBlood(Odyssey::ParticleSystem* newBloodEffect)
{
	mBloodParticleEffect = newBloodEffect;
}

// Returns the Particle system pointer to a "Hit effect"
Odyssey::ParticleSystem* Character::GetPSBlood()
{
	return mBloodParticleEffect;
}

// Returns the character portrait file path
std::wstring Character::GetPortraitPath()
{
	return mPortrait;
}

// Returns the vector of strings containing the animation paths
std::vector<std::string> Character::GetAnimationPaths()
{
	return mAnimations;
}
