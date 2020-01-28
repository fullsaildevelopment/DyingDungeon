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
		std::string characterNames[3];
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

	//Getter Functions
	/// <summary>
	/// Getter to retreves the level at the index given in the parameters
	/// </summary>
	/// <param name="index">The index of the level that is wanted same to [] operator</param>
	/// <returns>Returns a level at the index by refrence</returns>
	StatTracker::Level& GetLevel(unsigned int index);
	/// <summary>
	/// Getter to retreve the current level variable
	/// </summary>
	/// <returns>returns a the current level number by refrence</returns
	unsigned int& GetLevelCount();
	/// <summary>
	/// Getter to retreve the number of levels 
	/// </summary>
	/// <param name="level">The level to which the number of rounds is being requested from</param>
	/// <returns>The number of rounds from the requested level</returns>
	unsigned int GetRoundCount(unsigned int level);
	/// <summary>
	/// Getter for the size of the container holding the statistic data from combat levels
	/// </summary>
	/// <returns>The size of the container holding the statist data from combat</returns>
	size_t GetLevelSize();

	//Setters
	/// <summary>
	/// Setter for the number of levels intended to written to the container holding the statistic data of combat, resizing the container in preperation
	/// </summary>
	/// <param name="numLevels">unsigned int of the number of levels to be written to statistic data of combat</param>
	void SetLevels(unsigned int numLevels);
	/// <summary>
	/// Setter for the in-game rewards/end screen canvas
	/// </summary>
	/// <param name="canvas"></param>
	void SetCanvas(Odyssey::UICanvas* canvas);

	/// <summary>
	/// clears all the levels stored in the StatTrackers level container
	/// </summary>
	void ClearLevels();
	
	//Listner Functions
	/// <summary>
	/// Function is called when a Damage Delt event is brodcasted, to log information from the event
	/// </summary>
	/// <param name="cddEvent"></param>
	void LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent);
	/// <summary>
	/// Function is called when the Damage Taken event is brodcasted, to log information from the event
	/// </summary>
	/// <param name="ctdEvent"></param>
	void LogTakeDamageEvent(CharacterTakeDamage* ctdEvent);
	/// <summary>
	/// Function is called when the Heal event is brodcasted, to log information from the event
	/// </summary>
	/// <param name="chcEvent"></param>
	void LogHealingEvent(CharacterHealsEvent* chcEvent);
	/// <summary>
	/// Function is called when the Recive Health event is brodcasted, to log information from the event
	/// </summary>
	/// <param name="crhEvent"></param>
	void LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent);
	/// <summary>
	/// Function is called when the Buff event is brodcasted, to log information from the event
	/// </summary>
	/// <param name="csEvent"></param>
	void LogBuffingEvent(CharacterBuffsEvent* csEvent);
	/// <summary>
	/// Function is called when the Debuff event is brodcasted, to log information from the event
	/// </summary>
	/// <param name="cdEvent"></param>
	void LogDebuffingEvent(CharacterDebuffsEvent* cdEvent);
	/// <summary>
	/// Function is called as a reflex to the Level Stat event
	/// </summary>
	/// <param name="lsEvent"></param>
	void LevelStartReflex(LevelStartEvent* lsEvent);
	/// <summary>
	/// Function is called as a refles to the Turn Start event
	/// </summary>
	/// <param name="tsEvent"></param>
	void TurnStartReflex(TurnStartEvent* tsEvent);

	/// <summary>
	/// Function to print out a stat sheet to the stat_sheet folder in the apps file folder
	/// </summary>
	void OutputStatSheet();

	/// <summary>
	/// Calculation functions to see how much damage a character has output in a specified level and round as opposed to landed
	/// </summary>
	/// <param name="level">The level which the damage output amount should be returned from</param>
	/// <param name="round">The level which the damage output amount should be returned from</param>
	/// <returns>The amount of damage output from the specified level and round</returns>
	float CalculateDamageDealt(unsigned int level, unsigned int round);


private:
	StatTracker();

	//Function to generate reward screen at the end of the level
	/// <summary>
	/// Function is called as a response to the Rewards Screen Active event
	/// </summary>
	/// <param name="raEvent"></param>
	void UpdateRewardScreen(RewardsActiveEvnet* raEvent);

	//Getter Functions
	/// <summary>
	/// Getter for the count of stats 
	/// </summary>
	/// <param name="stat">The stat of which the count is requested</param>
	/// <returns>The count of the specifed stat</returns>
	unsigned int GetStatCount(Action stat);
	/// <summary>
	/// Getter for the count of stats within a specified level and round
	/// </summary>
	/// <param name="stat">The stat of which the count is requested</param>
	/// <param name="level">The level of which the stat count is requested</param>
	/// <param name="round">The round of which the stat count is requested</param>
	/// <returns>The count of the specifed stat</returns>
	unsigned int GetStatCount(Action stat, unsigned int level, unsigned int round);
	/// <summary>
	/// Getter for the count of stats from a certain character, specified by name
	/// </summary>
	/// <param name="name">Name of character</param>
	/// <param name="stat">he stat of which the count is requested</param>
	/// <returns></returns>
	unsigned int GetStatCount(std::string name, Action stat);
	/// <summary>
	/// Getter for a vector of player names found in the logged statistics
	/// </summary>
	/// <returns>A vector of player character names</returns>
	std::vector<std::string>& GetListPlayerCharacterNames();
	/// <summary>
	/// Getter for a vector of player names found in the logged statistics of a level
	/// </summary>
	/// <param name="levelNumber">Level which the list of names will be retrived from</param>
	/// <returns></returns>
	std::vector<std::string> GetListPlayerCharacterNames(unsigned int levelNumber);

	/// <summary>
	/// Calculation functions to see how much damage a character has output as opposed to landed
	/// </summary>
	/// <returns>The amount of damage output by a character</returns>
	float CalculateDamageDealt();
	/// <summary>
	/// Calculation functions to see how much damage a specified character has output as opposed to landed
	/// </summary>
	/// <param name="name">The name of the character of which you want the damage output from</param>
	/// <returns>The amount of damage output by a character</returns>
	float CalculateDamageDealt(std::string name);
	/// <summary>
	/// Calculation functions to see how much damage a specified character has output as opposed to landed in a specified level
	/// </summary>
	/// <param name="name">The name of the character of which you want the damage output from</param>
	/// <param name="level">The level of which the damage output should be retreaved from</param>
	/// <returns>The amount of damage output by a character</returns>
	float CalculateDamageDealt(std::string name, unsigned int level);

	//Calculation functinos to see how much damage a character landed
	/// <summary>
	/// Calculation of damage the player has landed
	/// </summary>
	/// <returns>The amount of damage laned</returns>
	float CalculateDamageDone();
	/// <summary>
	/// Calculation of the damage a specified character has landed
	/// </summary>
	/// <param name="name">The name of the character whos amount of damage landed is requested</param>
	/// <returns>The amount of damage landed by a specified character</returns>
	float CalculateDamageDone(std::string name);
	/// <summary>
	/// Calculation of the damage of a specific character that landed in a specific level and round
	/// </summary>
	/// <param name="name">The name of the character whos amount of damage landed is requested</param>
	/// <param name="level">The level of from which the damage landed is retrived from</param>
	/// <param name="round">The round of from which the damage landed is retrived from</param>
	/// <returns>The amount of damage landed by a specified characte in a spcified level and round</returns>
	float CalculateDamageDone(std::string name, unsigned int level, unsigned int round);
	/// <summary>
	/// Calculation of the damage of a specific character that landed in a specific level
	/// </summary>
	/// <param name="name">The name of the character whos amount of damage landed is requested</param>
	/// <param name="level">The level of from which the damage landed is retrived from</param>
	/// <returns>The amount of damage landed by a specified characte in a specfied level</returns>
	float CalculateDamageDone(std::string name, unsigned int level);

	//Calculation functions to see how much damage a character has taken
	/// <summary>
	/// Calculation of the damage taken
	/// </summary>
	/// <returns>The amount of damage the player has taken</returns>
	float CalculateDamageTaken();
	/// <summary>
	/// Calculation of the damage taken by a specific character
	/// </summary>
	/// <param name="name">The name of the character of whos amount of damage taken is requested</param>
	/// <returns>the amount of damage the a spcified character has taken</returns>
	float CalculateDamageTaken(std::string name);
	/// <summary>
	/// Calculation of damage taken by a specifed player in a specified level
	/// </summary>
	/// <param name="name">The name of the character of whos amount of damage taken is requested</param>
	/// <param name="level">The level from which the damage taken is being requested</param>
	/// <returns></returns>
	float CalculateDamageTaken(std::string name, unsigned int level);

	//Calculate the percentage of damage that a character landed
	/// <summary>
	/// Calculation of percent of damage landed from the damage output
	/// </summary>
	/// <returns>The percentage of damage landed</returns>
	float CalculatePercentDamageSuccess();
	/// <summary>
	/// Calculation of percent of damage landed from a spceific character in a spcific level
	/// </summary>
	/// <param name="name">The name of specifed character which the percentage is requested</param>
	/// <param name="level">The level from which the percentage is being requested from</param>
	/// <returns>The percentage of damage landed from a specific character in a specific level</returns>
	float CalculatePercentDamageSuccess(std::string name, unsigned int level);

	//Calculate the amount of damage a character mitigated
	/// <summary>
	/// Calculation of the amount of damage mitigated by the player
	/// </summary>
	/// <returns>The amount of damage mitigated by the player</returns>
	float CalculateDamageMitigatated();
	/// <summary>
	/// Calculation of the amount of damage mitigated by a specified character in a spcified level
	/// </summary>
	/// <param name="name">The name of the specified character of whose mitigation amount is requested</param>
	/// <param name="level">The level from which the mitigation amount is requested</param>
	/// <returns>The amount of damage mitigated by the spcified character in spcified level</returns>
	float CalculateDamageMitigatated(std::string name, unsigned int level);

	//Calculate the amount of health a character has recived
	/// <summary>
	/// Calculates amount of health recived by the player
	/// </summary>
	/// <returns>The amount of health recived by the player</returns>
	float CalculateHealthRecived();
	/// <summary>
	/// Calculation of health recived by a specified character
	/// </summary>
	/// <param name="name">The name of the spcified character whose health recived amount is requested</param>
	/// <returns>The amount of health recived by a specified character</returns>
	float CalculateHealthRecived(std::string name);
	/// <summary>
	/// Calculation of health recived by a specified character in a specified level
	/// </summary>
	/// <param name="name">The name of the specifed character whose health recived amount is requested</param>
	/// <param name="level">The level in which the health recived is being requested</param>
	/// <returns>The amount of health recived by a specified character</returns>
	float CalculateHealthRecived(std::string name, unsigned int level);

	//Calculate the amount of sheilding a character gives to allies
	/// <summary>
	/// Calculation of the amount of sheilding given by the player
	/// </summary>
	/// <returns>The amount of sheilding given by the player</returns>
	float CalculateShieldGiven();
	/// <summary>
	/// Calculation of the amount of sheilding given by a specified character
	/// </summary>
	/// <param name="name">The name of the specified character whoes sheilding given amount is requested</param>
	/// <returns>The amount of sheilding given by a specified character</returns>
	float CalculateShieldGiven(std::string name);
	/// <summary>
	/// Calculation of the amount of sheilding given by a specifed character in a specified level
	/// </summary>
	/// <param name="name">The name of the specified character whoes sheilding given amount is requested</param>
	/// <param name="level">The level from which the sheilding given amount is requested</param>
	/// <returns>The amount of sheilding given by a specified character in a specifid level</returns>
	float CalculateShieldGiven(std::string name, unsigned int level);

	//Calculate the percentatge of a specific 'stat' used in combat
	/// <summary>
	/// Calculation of the percentage of the use of a specified Action 
	/// </summary>
	/// <param name="stat">The specified Action of which the use percentage is being requested</param>
	/// <returns>The percentage of the use of the specified Action</returns>
	float CalculatePercentageStat(Action stat);
	/// <summary>
	/// Calculation of the percentage of the use of a specified Action in a specified level and round
	/// </summary>
	/// <param name="stat">The specified Action of which the use percentage is being requested</param>
	/// <param name="level">The specified level from which the use percentage is being requested</param>
	/// <param name="round">The specified round from which the use percentage is being requested</param>
	/// <returns>The percentage of the use of the specified Action in a specified level and round</returns>
	float CalculatePercentageStat(Action stat, unsigned int level, unsigned int round);
	/// <summary>
	/// Calculation of the percentage of the use of a specified Action for a specified character
	/// </summary>
	/// <param name="name">Name of specified character for whome the use percentage is being requested</param>
	/// <param name="stat">The specified Action of which the use percentage is being requested</param>
	/// <returns>The percentage of the use of the specified Action in a specified character</returns>
	float CalculatePercentageStat(std::string name, Action stat);
	/// <summary>
	/// Calculation of the percentage of the use of a specific Action for a specified character in a specified level
	/// </summary>
	/// <param name="name">Name of specified character for whome the use percentage is being requested</param>
	/// <param name="stat">The specified Action of which the use percentage is being requested</param>
	/// <param name="level">The specified level from which the use percentage is being requested</param>
	/// <returns>The percentage of the use of the specified Action in a specified character in a specified level</returns>
	float CalculatePercentageStat(std::string name, Action stat, unsigned int level);
	/// <summary>
	/// Calculation of the percentage of the use of a specific Action for a specified character in a specified level and round
	/// </summary>
	/// <param name="name">Name of specified character for whome the use percentage is being requested</param>
	/// <param name="stat">The specified Action of which the use percentage is being requested</param>
	/// <param name="level">The specified level from which the use percentage is being requested</param>
	/// <param name="round">The specified round from which the use percentage is being requested</param>
	/// <returns>The percentage of the use of the specified Action in a specified character in a specified level and round</returns>
	float CalculatePercentageStat(std::string name, Action stat, unsigned int level, unsigned int round);

	//Formats a float to a string with two decimal places
	/// <summary>
	/// Function to format an float in to a string with a specified amount of decimal place
	/// </summary>
	/// <param name="number">The value to be formated</param>
	/// <param name="decimal_places">The number of decimal places wanted in the string</param>
	/// <returns>The float as a string with a specific amount of decimal places</returns>
	std::string FormatToPercentage(float number, unsigned int decimal_places = 2);
	/// <summary>
	/// Function to format an float in to a wstring with a specified amount of decimal place
	/// </summary>
	/// <param name="number">The value to be formated</param>
	/// <param name="decimal_places">The number of decimal places wanted in the wstring</param>
	/// <returns>The float as a string with a specific amount of decimal places</returns>
	std::wstring FormatToPercentageW(float number, unsigned int decimal_places = 2);

	//functions to round decimal alues to a certain decimal place
	/// <summary>
	/// Function that rounds a float to a specified amount of decimal places
	/// </summary>
	/// <param name="num">The value to be rounded</param>
	/// <param name="decimal_places">The amount of decimal places to be rounded to</param>
	/// <returns>The rounded float value</returns>
	float roundf(float num, unsigned int decimal_places = 0);
	/// <summary>
	/// Function that rounds a double to a specified amount of decmial places
	/// </summary>
	/// <param name="num">The value to be rounded</param>
	/// <param name="decimal_places">The amount of decimal places to be rounded to</param>
	/// <returns>The rounded double value</returns>
	double round(double num, unsigned int decimal_places = 0);

	//unsigned int CalculateRoundsInLevel(unsigned int levelNumber);
};