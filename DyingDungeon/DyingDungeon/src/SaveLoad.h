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
		HEROID characterIDs[3] = {HEROID::Paladin, HEROID::Paladin, HEROID::Paladin};
	};
private:
	std::vector<Loadout> loadouts;
	std::string m_saveProfile;
	//unsigned int loadoutCount;
public:
	static SaveLoad& Instance();
	~SaveLoad();
	bool SaveStats(std::string saveName);
	bool LoadStats(std::string loadFileName);
	bool SaveGame(std::string saveProfile);
	bool LoadGame(std::string saveProfile);
	void AddLoadOut(std::string loadoutName, HEROID characterID_0, HEROID characterID_1, HEROID characterID_2);
	bool SaveLoadOut();
	bool LoadLoadOut();
	bool LoadLoadOut(std::string loadoutName);

	unsigned int dir_file_count(std::string path);
	unsigned int dir_file_count(std::experimental::filesystem::v1::path dir);

	void SetSaveProfile(std::string saveProfile);
	std::string GetSaveProfile();
private:
	SaveLoad();
};