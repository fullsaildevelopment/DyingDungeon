#pragma once
#include <string>
// enum used for readablity of what stat is affected
enum class STATS {None = -1, Atk = 0, Def, Spd};
enum class EFFECTTYPE { Bleed = 0, Regen, StatUp, StatDown, Stun, Shield, Provoke};
class Character; 
class StatusEffect
{
	// public and private variables
public:
protected:
	EFFECTTYPE mTypeId;
	// How much stat will be affected
	float mAmountOfEffect;
	// How many turns the status effect will last
	int mDuration;
	// Who will be affected by the status effect
	Character* mRecipient;
private:
	// public and private functions
public:
	// Constructors, Deconstructors
	StatusEffect() = default;
	~StatusEffect() = default;
	// Application functions
	virtual void Apply(Character& target) = 0;
	virtual void Remove() = 0;
	virtual void Use() = 0;
	// TypeId functions
	EFFECTTYPE GetTypeId();
	//  mDuration functions
	int GetDuration();
	void SetDuration(int newDuration);
	void ReduceDuration(int deduction);
	// mAmountOfEffect functions
	float GetAmountOfEffect();
	void SetAmountOfEffect(float newAmount);
	// mRecipient functions
	Character* GetRecipient();
	void SetRecipient(Character * recipient);
};

