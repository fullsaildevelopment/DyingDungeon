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

class CharacterHealsEvent : public Odyssey::Event
{
public:
	std::string healerName;
	std::string actionName;
	STATS actionEffect;
	bool isSpell;
	float health;
	CharacterHealsEvent(std::string healer, std::string action, STATS effect, float healthAmount, bool isASpell) 
	{
		healerName = healer;
		actionName = action;
		actionEffect = effect;
		isSpell = isASpell;
		health = healthAmount;
	}
};

class CharacterRecivesHealingEvent : public Odyssey::Event
{
public:
	std::string targetName;
	std::string actionName;
	CharacterRecivesHealingEvent(std::string target, std::string action)
	{
		targetName = target;
		actionName = action;
	}
};

class CharacterShieldsEvent : public Odyssey::Event 
{
public:
	std::string chracterName;
	std::string actionName;
	float shieldBuff;
	bool isSpell;
	CharacterShieldsEvent(std::string character, std::string action, float shield_defense, bool spell)
	{
		chracterName = character;
		actionName = action;
		shieldBuff = shield_defense;
		isSpell = spell;
	}
};

class CharacterRecivesShieldEvent : public Odyssey::Event 
{
public:
	std::string targetName;
	CharacterRecivesShieldEvent(std::string target) {
		targetName = target;
	}
};

class LevelStartEvent : public Odyssey::Event
{
public:
	unsigned int levelNumber;
	LevelStartEvent(unsigned int level)
	{
		levelNumber = level;
	}
};

class TurnStartEvent : public Odyssey::Event 
{
public:
	std::string characterName;
	unsigned int turn;
	unsigned int round;
	TurnStartEvent(std::string character, unsigned int turnNumber, unsigned int roundNumber)
	{
		characterName = character;
		turn = turnNumber;
		round = roundNumber;
	}
};