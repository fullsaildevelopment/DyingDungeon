#pragma once
#include <iostream>
#include <string>
#include "Event.h"
#include "Skills.h"

class CharacterDealtDamageEvent : public Odyssey::Event
{
public:
	std::string attackerName;
	std::string actionName;
	float damageAmount;
	STATS actionEffect;
	bool isSpell;

	CharacterDealtDamageEvent(std::string attacker, std::string action, float damage, STATS effect, bool isASpell)
	{
		attackerName = attacker;
		actionName = action;
		damageAmount = damage;
		actionEffect = effect;
		isSpell = isASpell;
	}
};

class CharacterTakeDamage : public Odyssey::Event
{
public:
	std::string targetName;
	std::string actionName;
	float mitigationAmount;
	
	CharacterTakeDamage(std::string target, std::string action, float mitigation)
	{
		targetName = target;
		actionName = action;
		mitigationAmount = mitigation;
	}
};

class CharacterHealsCharacterEvent : public Odyssey::Event
{
public:
	std::string healerName;
	std::string actionName;
	STATS actionEffect;
	bool m_isSpell;
	CharacterHealsCharacterEvent(std::string healer, std::string action, STATS effect, bool isSpell) 
	{
		healerName = healer;
		actionName = action;
		actionEffect = effect;
		m_isSpell = isSpell;
	}
};

class CharacterRecivesHealingEvent : public Odyssey::Event
{
public:
	std::string targetName;
	std::string actionName;
	float health;
	CharacterRecivesHealingEvent(std::string target, std::string actionName, float healthRestored)
	{
		targetName = 
		health = healthRestored;
	}
};

class CharacterDefendsEvent : public Odyssey::Event 
{

};

class LevelStartEvent : public Odyssey::Event
{
public:
	LevelStartEvent()
	{

	}
};

class TurnStartEvent : public Odyssey::Event 
{
public:
	TurnStartEvent()
	{

	}
};