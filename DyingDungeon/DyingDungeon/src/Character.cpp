#include "Character.h"
#include "Entity.h"
#include "Transform.h"
#include "RedAudioManager.h"
#include "MeshRenderer.h"
#include "StatUp.h"
#include "StatDown.h"
#include "Stun.h"
#include "CharacterHUDElements.h"
#include "SkillHoverComponent.h"

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
	mBloodEffectPrefab = nullptr;
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
bool Character::TakeTurn(std::vector<Odyssey::Entity*> playerTeam, std::vector<Odyssey::Entity*> enemyTeam)
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
	// Calculate damage reduction based on character drffense
	dmg = dmg - (dmg * GetDefMod());

	// If i have a shield reduce the incoming damage
	if (mShielding > 0.0f)
	{
		mShielding -= dmg;
		if (mShielding <= 0.0f)
		{
			dmg = mShielding * -1.0f;
			mShielding = 0.0f;
			mShieldTimer = 0;
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetShieldBuff()->setVisible(false);
		}
		else
			dmg = 0.0f;
	}

	// Reduce health by the amount of damage that made it through
	SetHP(GetHP() - dmg);

	// If they run out of Health kill the character
	if (mCurrentHP <= 0.0f)
		Die();
}

// Gives the character health back 
void Character::ReceiveHealing(float healing)
{
	// Add healing to current hp
	SetHP(mCurrentHP + healing);

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

	//// Check whether or not the character was healed or damaged
	//// Player took damage
	//if (previousHealth > mCurrentHP)
	//	GameUIManager::getInstance().AddHpPopupToUpdateList(this, true, abs(previousHealth - mCurrentHP));
	////Player was healed or not damaged at all
	//else
	//	GameUIManager::getInstance().AddHpPopupToUpdateList(this, false, abs(previousHealth - mCurrentHP));
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

	// Update the hero's skill sprites if they'll be able to use them or not
	if (this->IsHero())
	{
		// Get sprite vector
		std::vector<Odyssey::Sprite2D*> skillImages = GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<SkillHoverComponent>()->GetSkillSprites();
		
		// Check each skill to see if the character has enoung mana to perform skill
		for (int i = 0; i < skillImages.size(); i++)
		{
			// If the skill cost more mana than what the character has, make the sprite a lower opacity
			if (mSkillList[i]->GetManaCost() > mCurrentMana)
				skillImages[i]->setOpacity(0.3f);
			else
				skillImages[i]->setOpacity(1.0f);
		}
	}
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

// Returns the Base Attack stat of the character
float Character::GetBaseAtk()
{
	return mBaseAttack;
}

// Returns the Attack mod of the character
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

// Returns the defense mod of the character
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

// Returns the Speed mod of the character
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

DirectX::XMFLOAT3 Character::GetThemeColor()
{
	return mThemeColor;
}

// Resets the character back to its base
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
bool Character::AddStatusEffect(std::shared_ptr<StatusEffect> newEffect, Character* caster)
{
	// Make an iterator to check the characters list to see if it already has the passed in effect
	std::vector<StatusEffect*>::iterator it;

	// Temp pointer to the target
	Character* affectedTarget = newEffect->GetRecipient();

	// Switch statment for detirming what vector the effect needs to go into
	switch (newEffect->GetTypeId())
	{
	case EFFECTTYPE::Bleed:
	{
		affectedTarget->mIsBleeding = true;
		affectedTarget->mBleedTimer = newEffect->GetDuration();
		GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetBleedBuff()->setVisible(true);
		break;
	}
	case EFFECTTYPE::Regen:
	{
		affectedTarget->mIsRegenerating = true;
		affectedTarget->mRegenTimer = newEffect->GetDuration();
		GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetRegenBuff()->setVisible(true);
		break;
	}
	case EFFECTTYPE::Stun:
	{
		affectedTarget->SetState(STATE::STUNNED);
		mStunTimer = newEffect->GetDuration();
		GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetStunBuff()->setVisible(true);
		break;
	}
	case EFFECTTYPE::Provoke:
	{
		affectedTarget->SetProvoked(caster);
		mProvokedTimer = newEffect->GetDuration();
		GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetProvokeBuff()->setVisible(true);
		break;
	}
	case EFFECTTYPE::Shield:
	{
		affectedTarget->mShielding = newEffect->GetAmountOfEffect();
		affectedTarget->mShieldTimer = newEffect->GetDuration();
		GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetShieldBuff()->setVisible(true);
		break;
	}
	case EFFECTTYPE::StatUp:
	{
		std::vector<StatusEffect*>::iterator it;
		for (it = mSE.begin(); it != mSE.end();)
		{
			if ((*it)->GetTypeId() == newEffect->GetTypeId() && (*it)->GetAffectedStatId() == newEffect->GetAffectedStatId())
			{
				(*it)->Remove();
				it = mSE.erase(it);
			}
			else
				it++;
		}
		mSE.push_back(newEffect.get());
		switch (newEffect->GetAffectedStatId())
		{
		case STATS::Atk:
		{
			GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetAttackUpBuff()->setVisible(true);
			affectedTarget->IncreaseAtk(newEffect->GetAmountOfEffect());
			break;
		}
		case STATS::Def:
		{
			GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetDefenseUpBuff()->setVisible(true);
			affectedTarget->IncreaseDef(newEffect->GetAmountOfEffect());	
			break;
		}
		case STATS::Spd:
		{
			GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSpeedUpBuff()->setVisible(true);
			affectedTarget->IncreaseSpd(newEffect->GetAmountOfEffect());
			break;
		}
		default:
			break;
		}
		break;
	}
	case EFFECTTYPE::StatDown:
	{
		std::vector<StatusEffect*>::iterator it;
		for (it = mSE.begin(); it != mSE.end();)
		{
			if ((*it)->GetTypeId() == newEffect->GetTypeId() && (*it)->GetAffectedStatId() == newEffect->GetAffectedStatId())
			{
				(*it)->Remove();
				it = mSE.erase(it);
			}
			else
				it++;
		}
		mSE.push_back(newEffect.get());
		switch (newEffect->GetAffectedStatId())
		{
		case STATS::Atk:
		{
			GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetAttackDownBuff()->setVisible(true);
			affectedTarget->DecreaseAtk(newEffect->GetAmountOfEffect());
			break;
		}
		case STATS::Def:
		{
			GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetDefenseDownBuff()->setVisible(true);
			affectedTarget->DecreaseDef(newEffect->GetAmountOfEffect());
			break;
		}
		case STATS::Spd:
		{
			GameUIManager::getInstance().GetCharacterHuds()[affectedTarget->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetSpeedDownBuff()->setVisible(true);
			affectedTarget->DecreaseSpd(newEffect->GetAmountOfEffect());
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	return true;
}

// Manages status effects of passed in vector of effects, appling effects
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

// Manages all status effect casted by this character
void Character::ManageCastedEffects()
{
	// For each status effect in the list, use its effect, reduce its duration, then remove if it expires. Repeate for all other status effect vectors.
	std::vector<std::shared_ptr<StatusEffect>>::iterator it;

	// Casted Effects
	for (it = mCastedEffects.begin(); it != mCastedEffects.end();)
	{
		(*it)->ReduceDuration(1);
		if ((*it)->GetDuration() <= 0)
		{
			if (!(*it)->RemoveMe())
				(*it)->Remove();
			it = mCastedEffects.erase(it);
		}
		else
			it++;	
	}
}

// Add a effect to my casted vector
void Character::AddCastedEffect(std::shared_ptr<StatusEffect> newCastedEffect)
{
	mCastedEffects.push_back(newCastedEffect);
}

// Manages all status effects, appling effects and removing expired ones
bool Character::ManageTOREffects()
{
	if (mIsRegenerating)
	{
		ReceiveHealing(0.10f * mBaseMaxHP);
		mRegenTimer--;
		if (mRegenTimer <= 0)
		{
			mIsRegenerating = false;
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetRegenBuff()->setVisible(false);
		}
	}
	if (mIsBleeding)
	{
		TakeDamage(0.10f * mBaseMaxHP);
		mBleedTimer--;
		if (mBleedTimer <= 0)
		{
			mIsBleeding = false;
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetBleedBuff()->setVisible(false);
		}
	}
	if (mCurrentState == STATE::DEAD)
		return false;
	if (mShielding > 0)
	{
		mShieldTimer--;
		if (mShieldTimer <= 0)
		{
			mShielding = 0.0f;
			GameUIManager::getInstance().GetCharacterHuds()[this->GetHudIndex()]->getComponent<CharacterHUDElements>()->GetShieldBuff()->setVisible(false);
		}
	}
	return true;
}

// Clears all status effects from the Character
void Character::ClearStatusEffects()
{
	mCastedEffects.clear();
}

// Clears all harmful status effects
void Character::ClearBadStatusEffects()
{
	//mDebuffs.clear();
	//mBleeds.clear();
}

// Sets the Particle system pointer to a "Hit effect"
void Character::SetBloodPrefab(Odyssey::Entity* newBloodEffectPrefab)
{
	mBloodEffectPrefab = newBloodEffectPrefab;
}

// Returns the Particle system pointer to a "Hit effect"
Odyssey::Entity* Character::GetBloodPrefab()
{
	return mBloodEffectPrefab;
}

void Character::SpawnBloodEffect()
{
	Odyssey::Transform* tempTransform = mEntity->getComponent<Odyssey::Transform>();
	Odyssey::Entity* bloodEffect = nullptr;
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&tempTransform->getPosition());
	DirectX::XMVECTOR rotation = DirectX::XMLoadFloat3(&tempTransform->getEulerRotation());

	Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(mBloodEffectPrefab, &bloodEffect, position, rotation));
}

// Turns all active particle effects to inactive
//void Character::StopParticleEffects()
//{
//	// If blood effect is active set to false
//	if (mBloodParticleEffectPrefab != nullptr)
//	{
//		mBloodParticleEffect->stop();
//		mBloodParticleEffect->setActive(false);
//	}
//
//	// For each skill in the list
//	for (std::shared_ptr<Skills> S : mSkillList)
//	{
//		// If valid
//		if (S)
//		{
//			// If it has a particle effect
//			if (S->GetParticleSystem() != nullptr)
//			{
//				// Stop and set to false
//				S->GetParticleSystem()->stop();
//				S->GetParticleSystem()->setActive(false);
//			}
//		}
//	}
//}

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

// Retuyrns the path to the characers model
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

void Character::AddSoundClip(std::string soundKey, std::string newSoundName)
{
	mSoundClips[soundKey] = newSoundName;
}

std::string Character::GetSoundClipName(std::string soundKey)
{
	return mSoundClips[soundKey];
}
