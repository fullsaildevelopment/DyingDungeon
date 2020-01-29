#pragma once
#include <iostream>
#include "StatTracker.h"
#include "Character.h"
#include <iterator>
#include <direct.h>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

class SaveLoad
{
public:
	struct Loadout 
	{
		std::string profile = "";
		std::string name = "";
		GameplayTypes::HEROID characterIDs[3] = {GameplayTypes::HEROID::Paladin, GameplayTypes::HEROID::Paladin, GameplayTypes::HEROID::Paladin};
	};
private:
	std::vector<Loadout> loadouts;
	std::string m_saveProfile;
	//unsigned int loadoutCount;
private:
	/// <summary>
	/// Creates director(s) from a given path
	/// </summary>
	/// <param name="directory_path">Path of directory(s) to be created</param>
	/// <returns>Error code determining if the directory was succesfully created</returns>
	int create_directory(std::string& directory_path);
	/// <summary>
	/// Counts files in specified directory 
	/// </summary>
	/// <param name="path">Path to directory</param>
	/// <returns>unsigned int count of files in specified directory</returns>
	unsigned int dir_file_count(std::string path);
	/// <summary>
	/// Counts files in specified directory
	/// </summary>
	/// <param name="dir">Path to directory</param>
	/// <returns>unsigned int count of files in specified directory</returns>
	unsigned int dir_file_count(std::experimental::filesystem::v1::path dir);
public:
	static SaveLoad& Instance();
	~SaveLoad();
	/// <summary>
	/// Saves the stats currently stored in the stat tracker to a binary file
	/// </summary>
	/// <param name="saveName"></param>
	/// <returns>bool of wheather the file saved properly</returns>
	bool SaveStats(std::string saveName);
	/// <summary>
	/// Loads saved out stats from a binary file
	/// </summary>
	/// <param name="loadFileName"></param>
	/// <returns>bool of wheather the flle loaded in properly</returns>
	bool LoadStats(std::string loadFileName);
	/// <summary>
	/// Calls all nessesary save functions needed to save a game state
	/// </summary>
	/// <param name="saveProfile">Profile name under which to save the state in</param>
	/// <returns>bool of wheather the file saved properly</returns>
	bool SaveGame(std::string saveProfile);
	/// <summary>
	/// Calls all nessesary load functions needed to load a game state
	/// </summary>
	/// <param name="saveProfile">Profile name under which to save the state in</param>
	/// <returns></returns>
	bool LoadGame(std::string saveProfile);
	/// <summary>
	/// Adds a three character load out to the Save/Load manager
	/// </summary>
	/// <param name="loadoutName">Name of the loadout </param>
	/// <param name="characterID_0">Hero id for the first character in the loadout</param>
	/// <param name="characterID_1">Hero id for the second character in the loadout</param>
	/// <param name="characterID_2">Hero id for the third character in the loadout</param>
	void AddLoadOut(std::string loadoutName, GameplayTypes::HEROID characterID_0, GameplayTypes::HEROID characterID_1, GameplayTypes::HEROID characterID_2);
	/// <summary>
	/// Saves all loadouts to binary files
	/// </summary>
	/// <returns>bool of wheather the file saved out properly</returns>
	bool SaveLoadOut();
	/// <summary>
	/// Loads all loadouts from binary files
	/// </summary>
	/// <returns>bool of wheather the file was loaded out properly</returns>
	bool LoadLoadOut();
	/// <summary>
	/// Loads specified loadout file
	/// </summary>
	/// <param name="loadoutName">Name of loadout to be loaded</param>
	/// <returns>bool of wheather the file was loaded in properly</returns>
	bool LoadLoadOut(std::string loadoutName);
	/// <summary>
	/// Setter for the profile instance
	/// </summary>
	/// <param name="saveProfile"></param>
	void SetSaveProfile(std::string saveProfile);
	/// <summary>
	/// Getter for the current save profile name
	/// </summary>
	/// <returns>name of current save profile</returns>
	std::string GetSaveProfile();
private:
	SaveLoad();
};