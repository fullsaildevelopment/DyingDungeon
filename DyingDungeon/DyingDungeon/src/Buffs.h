#pragma once
#include "Component.h"
#include "GameObject.h"

class Character;

class Buffs
{
	// public and private variables
public:
	enum STATS { HP = 0, MP };
private:
	float mAmountOfEffect;
	int mDuration;
	int mEffectedStat;
	bool mBleed;
	Character* mRecipient;
	// public and private functions
public:
	// Constructors, Deconstructors
	Buffs() = default;
	Buffs(int effectedStat, float effect, int duration, bool isBleed, Character* recipient);
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
	int GetEffectedStat();
	void SetEffectedStat(int statEffected);
	// mAmountOfEffect functions
	float GetAmountOfEffect();
	// mRecipient functions
	Character* GetRecipient();
	void SetRecipient(Character* recipient);
private:
};

