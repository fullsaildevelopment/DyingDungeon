#pragma once
#include "Component.h"

class Buffs : public Odyssey::Component
{
	CLASS_DECLARATION(Buffs)
	//public and private variables
public:

private:
	float* m_statToAffect;
	float m_amountOfEffect;
	int duration;
	//public and private functions
public:
	Buffs(float* stat, float effect, int duration);
	~Buffs();
	virtual void initialize(Odyssey::GameObject* parent, float* stat, float effect, int duration);
private:
};

