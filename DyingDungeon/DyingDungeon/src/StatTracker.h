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

private:
	enum class Action { None = -1, Attack, Defend, Aid};
public:

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
		std::vector<StatTracker::Turn> turns;
	};

private:
	uint32_t m_currentLevel;
	uint32_t m_maxPlayerCount;
	std::vector<Level> m_levels;
	std::vector<std::string> characterNames;
	//TowerManager* m_towerManager;
	//RewardScreen
	Odyssey::UICanvas* m_p_rewardsScreen;
public:
	static StatTracker& Instance();
	~StatTracker();

	//void StartNextTurn();
	//void StartNextLevel();

	void SetRewardsScreen(Odyssey::UICanvas* rewardScreen);

	bool SaveStats(std::string saveName);
	bool LoadStats(std::string loadFileName);

	void LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent);
	void LogTakeDamageEvent(CharacterTakeDamage* ctdEvent);
	void LogHealingEvent(CharacterHealsEvent* chcEvent);
	void LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent);
	void LogBuffingEvent(CharacterBuffsEvent* csEvent);
	void LogDebuffingEvent(CharacterDebuffsEvent* cdEvent);
	void LevelStartReflex(LevelStartEvent* lsEvent);
	void TurnStartReflex(TurnStartEvent* tsEvent);
	void OutputStatSheet();

private:
	StatTracker();
	void UpdateRewardScreen(RewardsActiveEvnet* raEvent);

	unsigned int GetStatCount(Action stat);
	unsigned int GetStatCount(std::string name, Action stat);
	std::vector<std::string>& GetListPlayerCharacterNames();
	std::vector<std::string> GetListPlayerCharacterNames(unsigned int levelNumber);
	float CalculateDamageDealt();
	float CalculateDamageDealt(std::string name);
	float CalculateDamageDone();
	float CalculateDamageDone(std::string name);
	float CalculateDamageTaken();
	float CalculateDamageTaken(std::string name);
	float CalculatePercentDamageSuccess();
	float CalculateDamageMitigatated();
	float CalculateHealthRecived();
	float CalculateHealthRecived(std::string name);
	float CalculateShieldGiven();
	float CalculateShieldGiven(std::string name);
	float CalculatePercentageStat(Action stat);
	float CalculatePercentageStat(std::string name, Action stat);
	std::string FormatToPercentage(float number);
	std::wstring FormatToPercentageW(float number);
	float roundf(float num, unsigned int decimal_places = 0);
	double round(double num, unsigned int decimal_places = 0);
	//unsigned int CalculateRoundsInLevel(unsigned int levelNumber);
};