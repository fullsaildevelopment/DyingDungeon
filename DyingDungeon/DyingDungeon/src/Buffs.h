#pragma once
#include "Component.h"

class Character;

class Buffs : public Odyssey::Component
{
	CLASS_DECLARATION(Buff)
	//public and private variables
public:

private:
	float* m_statToAffect;
	float m_amountOfEffect;
	int m_duration;
	//public and private functions
public:
	Buffs(float* stat, float effect, int duration);
	~Buffs();
	virtual void initialize(Odyssey::GameObject* parent);
private:
};

