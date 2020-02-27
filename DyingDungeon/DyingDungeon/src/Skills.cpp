#include "Skills.h"
#include "GameplayTypes.h"
#include "Character.h"

/*
 * Function:  GetManaCost()
 * --------------------
 * Gets the mana cost of the skill/ability
 *
 * returns: void
 */
float Skills::GetManaCost()
{
	return mMpCost;
}
/*
 * Function:  Use()
 * --------------------
 * Caster will attack the target with dealing damage
 * and any status effects will be applied.
 *
 * returns: void
 */
void Skills::Use(Character& caster,Character& target)
{
}

// Set the description of the skill
void Skills::SetSkillDescription(std::wstring desc)
{
    mSkillDescription = desc;
}

// Get description of the skill
std::wstring Skills::GetSkillDescription()
{
    return mSkillDescription;
}

// Return a wstring to the icon name
std::wstring Skills::GetSkillIconPath()
{
    if (mSkillIconName.size() > 0)
        return mSkillIconName;
    else
        return L"assets/images/Blank.png";
}

// Set Icon name to passed in name
void Skills::SetSkillIconPath(std::wstring icon)
{
    mSkillIconName = icon;
}

// Get skill name 
std::wstring Skills::GetSkillName()
{
	return mSkillName;
}

// Returns the skills animation id
std::string Skills::GetAnimationId()
{
	return mAnimationId;
}

std::string Skills::GetSoundEffectName()
{
	return mSoundEffectPath;
}

float Skills::GetSoundEffectTiming()
{
	return mSoundEffectTiming;
}

void Skills::SetSoundEffect(std::string newSoundEffect, float newSoundTiming)
{
	mSoundEffectPath = newSoundEffect;
	mSoundEffectTiming = newSoundTiming;
}

// Return animation timing
float Skills::GetAnimationTiming()
{
	return mAnimationTime;
}

float Skills::GetStatusChance()
{
	return mStatusEffectChance;
}

void Skills::SetStatusChance(float newPercent)
{
	mStatusEffectChance = newPercent;
}

float Skills::RandomChance()
{
	return static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
}

// Returns Type Id
GameplayTypes::SKILLTYPE Skills::GetSkillTypeId()
{
	return mSkillTypeId;
}

// Returns if the skill is an aoe move
bool Skills::IsAOE()
{
	return mIsAOE;
}

// Gets the pointer to any status effect attatched to the skill if one exists
// Returns nullptr otherwise
StatusEffect* Skills::GetStatusEffect()
{
    return mStatusEffect.get();
}

// Takes in a shared pointer for statseffects and sets it to the status effect for 
// the current skill
void Skills::SetStatusEffect(std::shared_ptr<StatusEffect> se)
{
	mStatusEffect = se;
}

// Return the particlesystems entity
Odyssey::Entity* Skills::GetParticleSystem()
{
	return mPS.mPixelEntity;
}

// Returns the position offset for the particle system attatched to the skill
DirectX::XMFLOAT3 Skills::GetPosOffset()
{
	return mPS.mStartingPosOffest;
}

// Return particle systems firing time
float Skills::GetPSFiringTime()
{
	return mPS.mFiringTime;
}

// Set the particle system entity
void Skills::SetParticleSystem(Odyssey::Entity* newParticleSystem)
{
	mPS.mPixelEntity = newParticleSystem;
}

// Set the particle system offset
void Skills::SetParticleOffset(DirectX::XMFLOAT3 newOffset)
{
	mPS.mStartingPosOffest = newOffset;
}

// Set the particle system firing time
void Skills::SetParticleFiringTime(float newFiringTime)
{
	mPS.mFiringTime = newFiringTime;
}
