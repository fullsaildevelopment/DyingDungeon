#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Skills.h"
#include "TowerManager.h"
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
		unsigned int round;
		float value;
		STATS effect;
		StatTracker::Action actionType;
		bool isSpell = false;
		std::string actionName;
	};

	struct Level 
	{
		unsigned int levelNumber;
		unsigned int rounds;
		std::vector<StatTracker::Turn> turns;
	};

private:
	unsigned int m_currentLevel;
	std::vector<Level> m_levels;
	TowerManager* m_towerManager;
public:
	static StatTracker* Instance();
	void StartNextTurn();
	void StartNextLevel();
	~StatTracker();

private:
	StatTracker();
	float CalculatDamageDone(std::string name);
	float CalculateDamageTaken(std::string name);
};