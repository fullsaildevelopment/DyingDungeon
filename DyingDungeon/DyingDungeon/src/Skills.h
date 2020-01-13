#pragma once
#include "StatusEffect.h"
#include "StatusEvents.h"
#include <memory>
enum class SKILLTYPE {UNDEFINED = -1, ATTACK = 0, HEAL, BUFF, DEBUFF};
//forward declare charater class
class Character;

class Skills
{
//public and private variables
public:
protected:
	SKILLTYPE mTypeId;
	//how much mana it will cost the caster
	float mMpCost;
	// Animation time for when effects such as hit animation or particle effects should go out
	float mAnimationTime;
	// Name of skill
	std::string mName;
	// Animation ID
	std::string mAnimationId;
	// Tell if it affects a whole team or a single target
	bool mIsAOE;
	std::shared_ptr<StatusEffect> mDebuff;
private:
//public and private functions
public:
	//constructor
	Skills() = default;
	//deconstructor
	~Skills() = default;
	//get how much mana the skill cost
	float GetManaCost();
	// Get Skill name
	std::string GetName();
	// Gets the Animation ID
	std::string GetAnimationId();
	// Gets the animation timing
	float GetAnimationTiming();
	// Get TypeID
	SKILLTYPE GetTypeId();
	// Get isAOE
	bool IsAOE();
	StatusEffect* GetStatusEffect();
	// Set the status Effect this applies
	void SetStatusEffect(std::shared_ptr<StatusEffect> se);
	//use the skill
	virtual void Use(Character& caster, Character& target) = 0;
private:

};

