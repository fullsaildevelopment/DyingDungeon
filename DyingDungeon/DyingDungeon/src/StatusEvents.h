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
	EFFECTTYPE actionEffect;

	CharacterDealtDamageEvent(std::string attacker, std::string action, float damage, EFFECTTYPE effect)
	{
		attackerName = attacker;
		actionName = action;
		damageAmount = damage;
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
	CharacterShieldsEvent(std::string character, std::string action, float shield_defense)
	{
		chracterName = character;
		actionName = action;
		shieldBuff = shield_defense;
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
	RewardsActiveEvnet() {

	}
};