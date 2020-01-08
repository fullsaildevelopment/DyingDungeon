#pragma once
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include "Skills.h"
#include "TowerManager.h"
#include "EventManager.h"
#include "StatusEvents.h"
class StatTracker
{

private:
	enum Action { Attack, Defend, Aid };
public:

	struct Turn
	{
		std::string characterName;
		std::vector<std::string> targetNames;
		std::vector<float> blockValues;
		uint32_t round;
		float value;
		STATS effect;
		StatTracker::Action actionType;
		bool isSpell = false;
		std::string actionName;
	};

	struct Level
	{
		uint32_t levelNumber;
		uint32_t turnCount;
		std::vector<StatTracker::Turn> turns;
	};

private:
	uint32_t m_currentLevel;
	std::vector<Level> m_levels;
	TowerManager* m_towerManager;
public:
	static StatTracker& Instance();
	~StatTracker();

	void StartNextTurn();
	void StartNextLevel();

	void SaveStats(std::string saveName);
	void LoadStats(std::string loadFileName);

	void LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent);
	void LogTakeDamageEvent(CharacterTakeDamage* ctdEvent);
	void LogHealingEvent(CharacterHealsCharacterEvent* chcEvent);
	void LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent);

	void LevelStartReflex(LevelStartEvent* lsEvent);

	void TurnStartReflex(TurnStartEvent* tsEvent);

private:
	StatTracker();
	float CalculateDamageDealt(std::string name);
	float CalculateDamageDone(std::string name);
	float CalculateDamageTaken(std::string name);
	unsigned int CalculateRoundsInLevel(unsigned int levelNumber);
};