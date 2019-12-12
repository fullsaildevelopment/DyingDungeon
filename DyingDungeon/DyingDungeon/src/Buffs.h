#pragma once
#include "Component.h"

class Character;

class Buffs : public Odyssey::Component
{
	CLASS_DECLARATION(Buff)
	//public and private variables
public:
	enum STATS { HP = 0, MP };
private:
	float* mStatToAffect;
	float mAmountOfEffect;
	int mDuration;
	int mEffectedStat;
	//public and private functions
public:
	Buffs() = default;
	Buffs(int effectedStat, float effect, int duration);
	~Buffs();
	virtual void initialize(Odyssey::GameObject* parent);
private:
};

