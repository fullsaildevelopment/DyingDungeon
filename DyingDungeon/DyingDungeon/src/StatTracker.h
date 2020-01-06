#pragma once
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include "Skills.h"
#include "TowerManager.h"
#include "EventManager.h"
class StatTracker
{

private:
	enum Action { Attack, Defend, Aid };
public:
	static StatTracker* m_p_Instance;

	struct Turn
	{
		std::string characterName;
		std::string targetName;
		uint32_t round;
		float value;
		float blockValue;
		STATS effect;
		StatTracker::Action actionType;
		bool isSpell = false;
		std::string actionName;
	};

	struct Level 
	{
		uint32_t levelNumber;
		uint32_t rounds;
		std::vector<StatTracker::Turn> turns;
	};

private:
	uint32_t m_currentLevel;
	std::vector<Level> m_levels;
	TowerManager* m_towerManager;
public:
	static StatTracker* Instance();
	void StartNextTurn();
	void StartNextLevel();
	void SaveStats(std::string saveName);
	void LoadStats(std::string loadFileName);
	~StatTracker();

private:
	StatTracker();
	float CalculatDamageDone(std::string name);
	float CalculateDamageTaken(std::string name);
};