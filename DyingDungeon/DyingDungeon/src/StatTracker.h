#pragma once
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<ctime>
#include "Skills.h"
#include "TowerManager.h"
#include "EventManager.h"
#include "StatusEvents.h"
class StatTracker
{

private:
	enum class Action { None = -1, Attack, Defend, Aid };
public:

	struct Turn
	{
		std::string characterName = "";
		std::vector<std::string> targetNames;
		std::vector<float> blockValues;
		uint32_t round = 1;
		float value = 0.0f;
		STATS effect = STATS::None;
		StatTracker::Action actionType = StatTracker::Action::None;
		bool isSpell = false;
		bool isPlayer = false;
		std::string actionName = "";
	};

	struct Level
	{
		uint32_t levelNumber = 1;
		uint32_t turnCount = 1;
		std::vector<StatTracker::Turn> turns;
	};

private:
	uint32_t m_currentLevel = 1;
	std::vector<Level> m_levels;
	//TowerManager* m_towerManager;
public:
	static StatTracker& Instance();
	~StatTracker();

	//void StartNextTurn();
	//void StartNextLevel();

	void SaveStats(std::string saveName);
	void LoadStats(std::string loadFileName);

	void LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent);
	void LogTakeDamageEvent(CharacterTakeDamage* ctdEvent);
	void LogHealingEvent(CharacterHealsEvent* chcEvent);
	void LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent);

	void LevelStartReflex(LevelStartEvent* lsEvent);

	void TurnStartReflex(TurnStartEvent* tsEvent);
	void OutputStatSheet();

private:
	StatTracker();

	unsigned int GetAttackCount();
	unsigned int GetAttackCount(std::string name);
	unsigned int GetDefendCount();
	unsigned int GetDefendCount(std::string name);
	unsigned int GetAidCount();
	unsigned int GetAidCount(std::string name);

	float CalculateDamageDealt();
	float CalculateDamageDealt(std::string name);
	float CalculateDamageDone();
	float CalculateDamageDone(std::string name);
	float CalculateDamageTaken();
	float CalculateDamageTaken(std::string name);
	float CalculateAttackPercentage();
	float CalculateAttackPercentage(std::string name);
	float roundf(float num, unsigned int decimal_places);
	double round(double num, unsigned int decimal_places);
	//unsigned int CalculateRoundsInLevel(unsigned int levelNumber);
};