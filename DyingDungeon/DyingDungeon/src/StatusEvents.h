#pragma once
#include <iostream>
#include <string>
#include "Event.h"
#include "EventManager.h"
#include "Skills.h"
#include "Converter.h"

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
		attackerName = Converter::ConvertWStrToStr(attacker.c_str());
		actionName = Converter::ConvertWStrToStr(action.c_str());
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
		targetName = Converter::ConvertWStrToStr(target.c_str());
		actionName = Converter::ConvertWStrToStr(action.c_str());
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
		healerName = Converter::ConvertWStrToStr(healer.c_str());
		actionName = Converter::ConvertWStrToStr(action.c_str());
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
		targetName = Converter::ConvertWStrToStr(target.c_str());
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
		chracterName = Converter::ConvertWStrToStr(character.c_str());
		targetName = Converter::ConvertWStrToStr(target.c_str());
		actionName = Converter::ConvertWStrToStr(action.c_str());
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
		characterName = Converter::ConvertWStrToStr(character.c_str());
		actionName = Converter::ConvertWStrToStr(action.c_str());
		targetName = Converter::ConvertWStrToStr(target.c_str());
		debuffType = debuff;
		debuffValue = debuff_Value;
	}
};

class LevelStartEvent : public Odyssey::Event
{
public:
	unsigned int levelNumber;
	std::string playerCharacters[3];
	std::wstring playerPortaits[3];
	LevelStartEvent(unsigned int level, std::wstring player_character_1, std::wstring player_character_2, std::wstring player_character_3, std::wstring player_portrait_1, std::wstring player_portrait_2, std::wstring player_portrait_3)
	{
		levelNumber = level;
		playerCharacters[0] = Converter::ConvertWStrToStr(player_character_1.c_str());
		playerCharacters[1] = Converter::ConvertWStrToStr(player_character_2.c_str());
		playerCharacters[2] = Converter::ConvertWStrToStr(player_character_3.c_str());
		playerPortaits[0] = player_portrait_1;
		playerPortaits[1] = player_portrait_2;
		playerPortaits[2] = player_portrait_3;
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
		characterName = Converter::ConvertWStrToStr(character.c_str());
		turn = turnNumber;
		round = roundNumber;
		isPlayer = isAPlayer;
	}
};

class RewardsActiveEvent : public Odyssey::Event
{
public:
	unsigned int level;
	RewardsActiveEvent(unsigned int finishedLevel) 
	{
		level = finishedLevel;
	}
};