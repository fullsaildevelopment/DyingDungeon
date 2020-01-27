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
#include "UICanvas.h"
#include "Rectangle2D.h"
class StatTracker
{
public:
	enum class Action { None = -1, Attack, Defend, Aid };
	struct Turn
	{
		std::string characterName = "";
		float attackModifier = 0.0f;
		//std::vector<std::string> targetNames;
		std::vector <std::pair<std::string, float>> targets;
		uint32_t round = 1;
		float value = 0.0f;
		EFFECTTYPE effect = EFFECTTYPE::None;
		StatTracker::Action actionType = StatTracker::Action::None;
		//bool isSheild = false;
		bool isPlayer = false;
		std::string actionName = "";
	};

	struct Level
	{
		uint32_t levelNumber = 1;
		uint32_t turnCount = 1;
		uint32_t rounds = 1;
		std::vector<StatTracker::Turn> turns;
	};

private:
	uint32_t m_currentLevel;
	uint32_t m_maxPlayerCount;
	std::vector<Level> m_levels;
	std::vector<std::string> characterNames;
	//TowerManager* m_towerManager;
	//RewardScreen
	Odyssey::UICanvas* m_p_canvas;
public:
	static StatTracker& Instance();
	~StatTracker();

	//void StartNextTurn();
	//void StartNextLevel();

	//Getters
	StatTracker::Level& GetLevel(unsigned int index);
	unsigned int& GetLevelCount();
	unsigned int GetRoundCount(unsigned int level);
	size_t GetLevelSize();

	//Setters
	void SetLevels(unsigned int numLevels);
	void SetCanvas(Odyssey::UICanvas* canvas);

	//clears all the levels stored in the StatTrackers level container
	void ClearLevels()

	//File IO functions
	bool SaveStats(std::string saveName);
	bool LoadStats(std::string loadFileName);

	//Listner Functions
	void LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent);
	void LogTakeDamageEvent(CharacterTakeDamage* ctdEvent);
	void LogHealingEvent(CharacterHealsEvent* chcEvent);
	void LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent);
	void LogBuffingEvent(CharacterBuffsEvent* csEvent);
	void LogDebuffingEvent(CharacterDebuffsEvent* cdEvent);
	void LevelStartReflex(LevelStartEvent* lsEvent);
	void TurnStartReflex(TurnStartEvent* tsEvent);

	//Function to print out a stat sheet to the stat_sheet folder in the apps file folder
	void OutputStatSheet();

	//Calculation functions to see how much damage a character has output as opposed to landed
	float CalculateDamageDealt(unsigned int level, unsigned int round);


private:
	StatTracker();
	void UpdateRewardScreen(RewardsActiveEvnet* raEvent);
	unsigned int GetStatCount(Action stat);
	unsigned int GetStatCount(Action stat, unsigned int level, unsigned int round);
	unsigned int GetStatCount(std::string name, Action stat);
	std::vector<std::string>& GetListPlayerCharacterNames();
	std::vector<std::string> GetListPlayerCharacterNames(unsigned int levelNumber);

	//Calculation functions to see how much damage a character has output as opposed to landed
	float CalculateDamageDealt();
	float CalculateDamageDealt(std::string name);
	float CalculateDamageDealt(std::string name, unsigned int level);

	//Calculation functinos to see how much damage a character landed
	float CalculateDamageDone();
	float CalculateDamageDone(std::string name);
	float CalculateDamageDone(std::string name, unsigned int level, unsigned int round);
	float CalculateDamageDone(std::string name, unsigned int level);

	//Calculation functions to see how much damage a character has taken
	float CalculateDamageTaken();
	float CalculateDamageTaken(std::string name);
	float CalculateDamageTaken(std::string name, unsigned int level);

	//Calculate the percentage of damage that a character landed
	float CalculatePercentDamageSuccess();
	float CalculatePercentDamageSuccess(std::string name, unsigned int level);

	//Calculate the amount of damage a character mitigated
	float CalculateDamageMitigatated();
	float CalculateDamageMitigatated(std::string name, unsigned int level);

	//Calculate the amount of health a character has recived
	float CalculateHealthRecived();
	float CalculateHealthRecived(std::string name);
	float CalculateHealthRecived(std::string name, unsigned int level);

	//Calculate the amount of sheilding a character gives to allies
	float CalculateShieldGiven();
	float CalculateShieldGiven(std::string name);
	float CalculateShieldGiven(std::string name, unsigned int level);

	//Calculate the percentatge of a specific 'stat' used in combat
	float CalculatePercentageStat(Action stat);
	float CalculatePercentageStat(Action stat, unsigned int level, unsigned int round);
	float CalculatePercentageStat(std::string name, Action stat);
	float CalculatePercentageStat(std::string name, Action stat, unsigned int level);
	float CalculatePercentageStat(std::string name, unsigned int round, Action stat);

	//Formats a float to a string with two decimal places
	std::string FormatToPercentage(float number, unsigned int decimal_places = 2);
	std::wstring FormatToPercentageW(float number);

	//functions to round decimal alues to a certain decimal place
	float roundf(float num, unsigned int decimal_places = 0);
	double round(double num, unsigned int decimal_places = 0);

	//unsigned int CalculateRoundsInLevel(unsigned int levelNumber);
};