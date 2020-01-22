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

// Get skill name 
std::string Skills::GetName()
{
	return mName;
}

// Returns the skills animation id
std::string Skills::GetAnimationId()
{
	return mAnimationId;
}

float Skills::GetAnimationTiming()
{
	return mAnimationTime;
}

// Returns Type Id
SKILLTYPE Skills::GetTypeId()
{
	return mTypeId;
}

bool Skills::IsAOE()
{
	return mIsAOE;
}

StatusEffect* Skills::GetStatusEffect()
{
    return mStatusEffect.get();
}

void Skills::SetStatusEffect(std::shared_ptr<StatusEffect> se)
{
	mStatusEffect = se;
}

Odyssey::ParticleSystem* Skills::GetParticleSystem()
{
	return mPS.mPixelEntity;
}

DirectX::XMFLOAT3 Skills::GetPosOffset()
{
	return mPS.mStartingPosOffest;
}

float Skills::GetPSFiringTime()
{
	return mPS.mFiringTime;
}

void Skills::SetParticleSystem(Odyssey::ParticleSystem* newParticleSystem)
{
	mPS.mPixelEntity = newParticleSystem;
}

void Skills::SetParticleOffset(DirectX::XMFLOAT3 newOffset)
{
	mPS.mStartingPosOffest = newOffset;
}

void Skills::SetParticleFiringTime(float newFiringTime)
{
	mPS.mFiringTime = newFiringTime;
}
