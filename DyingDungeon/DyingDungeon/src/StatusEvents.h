#pragma once
#include <iostream>
#include <string>
#include "Event.h"
#include "EventManager.h"
#include "Skills.h"

std::string ConvertStrToWStr(const wchar_t* p_char) {
	const size_t word_size = wcslen(p_char) + 1;
	size_t zero = 0;
	char* output = new char[word_size];

	wcstombs_s(&zero, output, word_size, p_char, _TRUNCATE);
	return std::string(output);
}

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
		attackerName = ConvertStrToWStr(attacker.c_str());
		actionName = ConvertStrToWStr(action.c_str());
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
		targetName = ConvertStrToWStr(target.c_str());
		actionName = ConvertStrToWStr(action.c_str());
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
		healerName = ConvertStrToWStr(healer.c_str());
		actionName = ConvertStrToWStr(action.c_str());
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
		targetName = ConvertStrToWStr(target.c_str());
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
		chracterName = ConvertStrToWStr(character.c_str());
		targetName = ConvertStrToWStr(target.c_str());
		actionName = ConvertStrToWStr(action.c_str());
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
		characterName = ConvertStrToWStr(character.c_str());
		actionName = ConvertStrToWStr(action.c_str());
		targetName = ConvertStrToWStr(target.c_str());
		debuffType = debuff;
		debuffValue = debuff_Value;
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
	TurnStartEvent(std::wstring character, unsigned int turnNumber, unsigned int roundNumber, bool isAPlayer)
	{
		characterName = ConvertStrToWStr(character.c_str());
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