#pragma once
#include "Component.h"
#include "GameObject.h"

class Character;

class Buffs : public Odyssey::Component
{
	CLASS_DECLARATION(Buff)
	// public and private variables
public:
	enum STATS { HP = 0, MP };
private:
	float mAmountOfEffect;
	int mDuration;
	int mEffectedStat;
	bool mBleed;
	// public and private functions
public:
	// Constructors, Deconstructors, Componet functions
	Buffs() = default;
	Buffs(int effectedStat, float effect, int duration, bool isBleed);
	~Buffs();
	virtual void initialize(Odyssey::GameObject* parent);
	// The function that reapplies the effect of the buff, used for bleeds and regens
	void Bleed();
	// Getter and setter for the mDuration
	int GetDuration();
	void SetDuration(int newDuration);
	void ReduceDuration(int deduction);
	//Getter and setter for mBleed
	bool IsBleed();
	// Getter and setter for stat to be affected
	int GetEffectedStat();
	void SetEffectedStat(int statEffected);
	// Getter and setter for amount to effect by
	float GetAmountOfEffect();
private:
};

