#include "Skills.h"
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
void Skills::SetDescription(std::wstring desc)
{
    mDescription = desc;
}

// Get description of the skill
std::wstring Skills::GetDescription()
{
    return mDescription;
}

// Get skill name 
std::wstring Skills::GetName()
{
	return mName;
}

// Returns the skills animation id
std::string Skills::GetAnimationId()
{
	return mAnimationId;
}

// Return animation timing
float Skills::GetAnimationTiming()
{
	return mAnimationTime;
}

// Returns Type Id
SKILLTYPE Skills::GetTypeId()
{
	return mTypeId;
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
Odyssey::ParticleSystem* Skills::GetParticleSystem()
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
void Skills::SetParticleSystem(Odyssey::ParticleSystem* newParticleSystem)
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
