#pragma once
#include "ParticleMover.h"
#include "StatusEffect.h"
#include "StatusEvents.h"
#include "GameplayTypes.h"
#include <memory>
//forward declare charater class
class Character;

class Skills
{
//public and private variables
public:
	
	struct ParticleInfomation
	{
		Odyssey::ParticleSystem* mPixelEntity = nullptr;
		DirectX::XMFLOAT3 mStartingPosOffest = { 0.0f,0.0f,0.0f };
		float mFiringTime = 0.0f;
	};
protected:
	
	// Skill type
	GameplayTypes::SKILLTYPE mSkillTypeId;
	// How much mana it will cost the caster
	float mMpCost;
	// Animation time for when effects such as hit animation or particle effects should go out
	float mAnimationTime;
	// Name of skill
	std::wstring mSkillName;
	// Animation ID
	std::string mAnimationId;
	// Tell if it affects a whole team or a single target
	bool mIsAOE;
	// Status Effect Attached to skill
	std::shared_ptr<StatusEffect> mStatusEffect;
	// Particle effect if the skill has it 
	ParticleInfomation mPS;
	// Description of the skill
	std::wstring mSkillDescription;
	// The Icon sprite for the skill
	std::wstring mSkillIconName;


private:
//public and private functions
public:
	// Constructor
	Skills() = default;
	// Deconstructor
	~Skills() = default;
	// Get how much mana the skill cost
	float GetManaCost();
	// Get Skill name
	std::wstring GetSkillName();
	// Gets the Animation ID
	std::string GetAnimationId();
	// Gets the animation timing
	float GetAnimationTiming();
	// Get TypeID
	GameplayTypes::SKILLTYPE GetSkillTypeId();
	// Get isAOE
	bool IsAOE();
	StatusEffect* GetStatusEffect();
	// Set the status Effect this applies
	void SetStatusEffect(std::shared_ptr<StatusEffect> se);
	// Gets the particle effect Entity
	Odyssey::ParticleSystem* GetParticleSystem();
	// Get the particle start pos offset
	DirectX::XMFLOAT3 GetPosOffset();
	// Get the particle firing time
	float GetPSFiringTime();
	// Set the pointer to a Particle system
	void SetParticleSystem(Odyssey::ParticleSystem* newParticleSystem);
	// Set the starting position offset
	void SetParticleOffset(DirectX::XMFLOAT3 newOffset);
	// Set the firing timer for the particle effe3ct
	void SetParticleFiringTime(float newFiringTime);
	// Use the skill
	virtual void Use(Character& caster, Character& target) = 0;
	// Set Description
	void SetSkillDescription(std::wstring desc);
	// Get Description Of The Skill
	std::wstring GetSkillDescription();
	// Get Icon Name
	std::wstring GetSkillIconPath();
	// Set Icon Name	
	void SetSkillIconPath(std::wstring icon);


private:

};

