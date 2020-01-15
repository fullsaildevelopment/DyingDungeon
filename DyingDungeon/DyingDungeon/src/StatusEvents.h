#pragma once
#include <iostream>
#include <string>
#include "Event.h"
#include "EventManager.h"
#include "Skills.h"

class CharacterDealtDamageEvent : public Odyssey::Event
{
public:
	std::string attackerName;
	std::string actionName;
	float damageAmount;
	float atkMod;
	EFFECTTYPE actionEffect;
	CharacterDealtDamageEvent(std::string attacker, std::string action, float damage, float attackMod, EFFECTTYPE effect)
	{
		attackerName = attacker;
		actionName = action;
		damageAmount = damage;
		atkMod = attackMod;
		actionEffect = effect;
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
	EFFECTTYPE actionEffect;
	float health;
	CharacterHealsEvent(std::string healer, std::string action, EFFECTTYPE effect, float healthAmount) 
	{
		healerName = healer;
		actionName = action;
		actionEffect = effect;
		health = healthAmount;
	}
};

class CharacterRecivesHealingEvent : public Odyssey::Event
{
public:
	std::string targetName;
	float healingAmount;
	CharacterRecivesHealingEvent(std::string target, float healing)
	{
		targetName = target;
		healingAmount = healing;
	}
};

class CharacterShieldsEvent : public Odyssey::Event 
{
public:
	std::string chracterName;
	std::string actionName;
	std::string targetName;
	float shieldBuff;
	CharacterShieldsEvent(std::string character, std::string target, std::string action, float shield_defense)
	{
		chracterName = character;
		targetName = target;
		actionName = action;
		shieldBuff = shield_defense;
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
	bool isPlayer;
	TurnStartEvent(std::string character, unsigned int turnNumber, unsigned int roundNumber, bool isAPlayer)
	{
		characterName = character;
		turn = turnNumber;
		round = roundNumber;
		isPlayer = isAPlayer;
	}
};

class RewardsActiveEvnet : public Odyssey::Event
{
public:
	unsigned int level;
	RewardsActiveEvnet(unsigned int finishedLevel) 
	{
		level = finishedLevel;
	}
};