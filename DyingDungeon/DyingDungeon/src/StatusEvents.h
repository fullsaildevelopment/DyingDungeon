#pragma once
#include <iostream>
#include <string>
#include "Event.h"
#include "Skills.h"

class CharacterDealtDamageEvent : public Odyssey::Event
{
	std::string m_characterName;
	std::string m_targetName;
	std::string m_actionName;
	float m_value;
	STATS m_effect;
	bool m_isSpell;
public:
	CharacterDealtDamageEvent(std::string attackerName, std::string targetName, std::string actionName, float damage, STATS effect, bool isSpell)
	{
		m_characterName = attackerName;
		m_targetName = targetName;
		m_actionName = actionName;
		m_value = damage;
		m_effect = effect;
		m_isSpell = isSpell;
	}
};

class CharacterTakeDamage : public Odyssey::Event
{
	std::string m_attackerName;
	std::string m_actionName;
	float m_damage;
	float m_mitigation;
public:
	CharacterTakeDamage(std::string attakerName, std::string actionName, float damage, float mitigation)
	{
		m_attackerName = attakerName;
		m_actionName = actionName;
		m_damage = damage;
		m_mitigation = mitigation;
	}
};

class CharacterHealsCharacterEvent : public Odyssey::Event
{
	std::string m_characterName;
	std::string m_targetName;
	std::string m_actionName;
	float m_value;
	STATS m_effect;
	bool m_isSpell;
public:
	CharacterHealsCharacterEvent(std::string healerName, std::string targetName, std::string actionName, float healthRestored, STATS effect, bool isSpell) {
		m_characterName = healerName;
		m_targetName = targetName;
		m_actionName = actionName;
		m_value = healthRestored;
		m_effect = effect;
		m_isSpell = isSpell;
	}
};