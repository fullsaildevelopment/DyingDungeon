#pragma once
#include "Component.h"
#include "GameObject.h"

class Character;

// enum used for readablity 
enum class STATS { NONE = 0,HP, MP, Atk, Def, Spd, Shd };

class Buffs
{
	// public and private variables
public:
private:
	float mAmountOfEffect;
	int mDuration;
	STATS mEffectedStat;
	bool mBleed;
	Character* mRecipient;
	// public and private functions
public:
	// Constructors, Deconstructors
	Buffs() = default;
	Buffs(STATS effectedStat, float effect, int duration, bool isBleed, Character* recipient);
	~Buffs();
	// Apply the buff effect functions
	void InitalEffect();
	void RevertEffect();
	void Bleed();
	//  mDuration functions
	int GetDuration();
	void SetDuration(int newDuration);
	void ReduceDuration(int deduction);
	// mBleed functions
	bool IsBleed();
	// mEffectedStat functions
	STATS GetEffectedStat();
	void SetEffectedStat(STATS statEffected);
	// mAmountOfEffect functions
	float GetAmountOfEffect();
	// mRecipient functions
	Character* GetRecipient();
	void SetRecipient(Character* recipient);
private:
};

