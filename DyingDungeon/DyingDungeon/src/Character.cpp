#include "Character.h"
#include "Entity.h"
#include "Transform.h"
#include "RedAudioManager.h"
#include "MeshRenderer.h"
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
	mAttack = mBaseAttack = 0.0f;
	mDefense = mBaseDefense = 0.0f;
	mSpeed = mBaseSpeed = 0.0f;
	mEXP = 0.0f;
	mCurrentLevel = 0;
	mHudIndex = 0;
	mProvoked = nullptr;
	mAnimator = nullptr;
	mBloodParticleEffect = nullptr;
	mImpactIndicator = nullptr;
	mName = L"";
	mSubName = L"";
	mPortrait = L"";
	mDescription = L"";
	mModel = "";
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
	/*if (mDisplaying)
	{
		pDmgText->addOpacity(static_cast<float>(-deltaTime) / 2.0f);
		if (pDmgText->getOpacity() == 0.0f)
		{
			mDisplaying = false;
		}
	}*/
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

// Called whenever this character needs to take damage
void Character::TakeDamage(float dmg)
{
	// Play sound effect for getting hit
	RedAudioManager::Instance().PlaySFX("PaladinHit");

	// Calculate damage reduction based on character drffense
	dmg = dmg - (dmg * GetDefMod());

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

	// Update the combat log
	GameUIManager::getInstance().UpdateCombatLogText(dmg);

	// If they run out of Health kill the character
	if (mCurrentHP <= 0.0f)
		Die();
}

// Gives the character health back 
void Character::ReceiveHealing(float healing)
{
	// Add healing to current hp
	SetHP(mCurrentHP + healing);
	
	GameUIManager::getInstance().UpdateCombatLogText(healing);

	// Send off a dumbass event for reds dumbass stat tracking
	//Odyssey::EventManager::getInstance().publish(new CharacterRecivesHealingEvent(mName, healing));

	// TODO: Update Combat Log Text Here

	// If they run out of Health kill the character
	if (mCurrentHP <= 0.0f)
		Die();
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
	// Add the health bar for animation
	GameUIManager::getInstance().AddCharacterHpBarsToUpdateList(this, mCurrentHP, HP);

	// Set the hp to the passed in amount
	float previousHealth = mCurrentHP;
	mCurrentHP = HP;

	// if health is lower that 0.0f set it to 0.0f, if its over the max set it to the max
	if (mCurrentHP < 0.0)
		mCurrentHP = 0.0;
	else if (mCurrentHP > mBaseMaxHP)
		mCurrentHP = mBaseMaxHP;

	// Check whether or not the character was healed or damaged
	// Player took damage
	if (previousHealth > mCurrentHP)
		GameUIManager::getInstance().AddHpPopupToUpdateList(this, true, abs(previousHealth - mCurrentHP));
	//Player was healed or not damaged at all
	else
		GameUIManager::getInstance().AddHpPopupToUpdateList(this, false, abs(previousHealth - mCurrentHP));
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
	// Add the health bar for animation
	GameUIManager::getInstance().AddCharacterMpBarsToUpdateList(this, mCurrentMana, Mana);

	// Set the mp to the passed in amount
	mCurrentMana = Mana;

	// if mana is lower that 0.0f set it to 0.0f, if its over the max set it to the max
	if (mCurrentMana < 0.0f)
		mCurrentMana = 0.0f;
	else if (mCurrentMana > mBaseMaxMana)
		mCurrentMana = mBaseMaxMana;
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

float Character::GetBaseAtk()
{
	return mBaseAttack;
}

float Character::GetAtkMod()
{
	return (mAttack - BASEATK) / 100.0f;
}

// Increases the Attack stat of the character
void Character::IncreaseAtk(float statIncrease)
{
	mAttack += (mBaseAttack * statIncrease);
}

// Decreases the Attack stat of the character
void Character::DecreaseAtk(float statDecrease)
{
	mAttack -= (mBaseAttack * statDecrease);
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

float Character::GetDefMod()
{
	return (mDefense - BASEDEF) / 200.0f;
}

// Increases the current Defense stat of the character
void Character::IncreaseDef(float statIncrease)
{
	mDefense += (mBaseDefense * statIncrease);
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

float Character::GetSpdMod()
{
	return (mSpeed - BASESPD) / 50.0f;
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

void Character::ResetMe()
{
	// Update the bars
	GameUIManager::getInstance().AddCharacterHpBarsToUpdateList(this, mCurrentHP, mBaseMaxHP);
	GameUIManager::getInstance().AddCharacterMpBarsToUpdateList(this, mCurrentMana, mBaseMaxMana);

	mCurrentHP = mBaseMaxHP;
	mCurrentMana = mBaseMaxMana;
	mCurrentState = STATE::NONE;
	ClearStatusEffects();
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
		if ((*it)->GetDuration() <= 0)
		{
			it = effectList.erase(it);
			continue;
		}
		else
			(*it)->Use();
		if (mCurrentState == STATE::DEAD)
			return;
		it++;
	}
}

void Character::ManageCastedEffects()
{
	// For each status effect in the list, use its effect, reduce its duration, then remove if it expires. Repeate for all other status effect vectors.
	std::vector<StatusEffect*>::iterator it;

	// Casted Effects
	for (it = mCastedEffects.begin(); it != mCastedEffects.end();)
	{
		if (static_cast<int>((*it)->GetTypeId()) < 0  || static_cast<int>((*it)->GetTypeId()) > 8)
		{
			(*it) = nullptr;
			it = mCastedEffects.erase(it);
			continue;
		}
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			(*it)->Remove();
			(*it) = nullptr;
			it = mCastedEffects.erase(it);
		}
		else
			it++;
	}
}

void Character::AddCastedEffect(StatusEffect* newCastedEffect)
{
	mCastedEffects.push_back(newCastedEffect);
}

// Manages all status effects, appling effects and removing expired ones
bool Character::ManageAllEffects()
{
	ManageStatusEffects(mRegens);
	ManageStatusEffects(mBleeds);
	if (mCurrentState == STATE::DEAD)
		return false;
	ManageStatusEffects(mBuffs);
	ManageStatusEffects(mDebuffs);
	ManageStatusEffects(mSheilds);
	return true;
}

// Clears all status effects from the Character
void Character::ClearStatusEffects()
{
	//std::vector<std::shared_ptr<StatusEffect>>::iterator it;
	mCastedEffects.clear();
	mDebuffs.clear();
	mBuffs.clear();
	mBleeds.clear();
	mRegens.clear();
	mSheilds.clear();
}

void Character::ClearBadStatusEffects()
{
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;

	for (it = mDebuffs.begin(); it != mDebuffs.end();)
	{
		if ((*it))
		{
			(*it)->Remove();
			it = mDebuffs.erase(it);
		}
		else
			it++;
	}

	for (it = mBleeds.begin(); it != mBleeds.end();)
	{
		if ((*it))
		{
			(*it)->Remove();
			it = mBleeds.erase(it);
		}
		else
			it++;
	}
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

// Turns all active particle effects to inactive
void Character::StopParticleEffects()
{
	// If blood effect is active set to false
	if (mBloodParticleEffect != nullptr)
	{
		mBloodParticleEffect->stop();
		mBloodParticleEffect->setActive(false);
	}

	// For each skill in the list
	for (std::shared_ptr<Skills> S : mSkillList)
	{
		// If valid
		if (S)
		{
			// If it has a particle effect
			if (S->GetParticleSystem() != nullptr)
			{
				// Stop and set to false
				S->GetParticleSystem()->stop();
				S->GetParticleSystem()->setActive(false);
			}
		}
	}
}

// Returns the character portrait file path
std::wstring Character::GetPortraitPath()
{
	return mPortrait;
}

// Returns the vector of strings containing the animation paths
std::vector<Character::AnimationImportData> Character::GetAnimationPaths()
{
	return mAnimations;
}

// Returns the current level of the player
unsigned int Character::GetLevel()
{
	return mCurrentLevel;
}

// Returns the sub-name of the character
std::wstring Character::GetSubName()
{
	return mSubName;
}

// Returns the description of the character
std::wstring Character::GetDescription()
{
	return mDescription;
}

std::string Character::GetModel()
{
	return mModel;
}

// Sets the description of the character
void Character::SetDescription(std::wstring newDescription)
{
	mDescription = newDescription;
}

// Set the characters Hud index
void Character::SetHudIndex(unsigned int newIndex)
{
	mHudIndex = newIndex;
}

// Returns the character hud index
unsigned int Character::GetHudIndex()
{
	return mHudIndex;
}
