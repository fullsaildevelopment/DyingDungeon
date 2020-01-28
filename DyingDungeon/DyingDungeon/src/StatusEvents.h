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
	CharacterDealtDamageEvent(std::wstring attacker, std::wstring action, float damage, float attackMod, EFFECTTYPE effect)
	{
		attackerName = std::string(attacker.begin(), attacker.end());
		actionName = std::string(action.begin(), action.end());
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
	
	CharacterTakeDamage(std::wstring target, std::wstring action, float mitigation)
	{
		targetName = std::string(target.begin(), target.end());
		actionName = std::string(action.begin(), action.end());
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
	CharacterHealsEvent(std::wstring healer, std::wstring action, EFFECTTYPE effect, float healthAmount)
	{
		healerName = std::string(healer.begin(), healer.end());
		actionName = std::string(action.begin(), action.end());
		actionEffect = effect;
		health = healthAmount;
	}
};

class CharacterRecivesHealingEvent : public Odyssey::Event
{
public:
	std::string targetName;
	float healingAmount;
	CharacterRecivesHealingEvent(std::wstring target, float healing)
	{
		targetName = std::string(target.begin(), target.end());
		healingAmount = healing;
	}
};

class CharacterBuffsEvent : public Odyssey::Event 
{
public:
	std::string chracterName;
	std::string actionName;
	std::string targetName;
	EFFECTTYPE buffType;
	float buffValue;
	CharacterBuffsEvent(std::wstring character, std::wstring target, std::wstring action, EFFECTTYPE buff, float buffAmount)
	{
		chracterName = std::string(character.begin(), character.end());
		targetName = std::string(target.begin(), target.end());
		actionName = std::string(action.begin(), action.end());
		buffType = buff;
		buffValue = buffAmount;
	}
};

class CharacterDebuffsEvent : public Odyssey::Event
{
public:
	std::string characterName;
	std::string actionName;
	std::string targetName;
	EFFECTTYPE debuffType;
	float debuffValue;
	CharacterDebuffsEvent(std::wstring character, std::wstring target, std::wstring action, EFFECTTYPE debuff, float debuff_Value)
	{
		characterName = std::string(character.begin(), character.end());
		actionName = std::string(action.begin(), action.end());
		targetName = std::string(target.begin(), target.end());
		debuffType = debuff;
		debuffValue = debuff_Value;
	}
};

class LevelStartEvent : public Odyssey::Event
{
public:
	unsigned int levelNumber;
	std::string playerCharacters[3];
	LevelStartEvent(unsigned int level, std::wstring player_character_1, std::wstring player_character_2, std::wstring player_character_3)
	{
		levelNumber = level;
		playerCharacters[0] = std::string(player_character_1.begin(), player_character_1.end());
		playerCharacters[1] = std::string(player_character_2.begin(), player_character_2.end());
		playerCharacters[2] = std::string(player_character_3.begin(), player_character_3.end());
	}
};

class TurnStartEvent : public Odyssey::Event 
{
public:
	std::string characterName;
	unsigned int turn;
	unsigned int round;
	bool isPlayer;
	TurnStartEvent(std::wstring character, unsigned int turnNumber, unsigned int roundNumber, bool isAPlayer)
	{
		characterName = std::string(character.begin(), character.end());
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