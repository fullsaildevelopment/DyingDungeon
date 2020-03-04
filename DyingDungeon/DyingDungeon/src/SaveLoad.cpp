#include "SaveLoad.h"

SaveLoad::SaveLoad()
{
	m_saveProfile = "Profile_0/";
}

SaveLoad& SaveLoad::Instance()
{
	// TODO: insert return statement here
	static SaveLoad instance_saveload;
	return instance_saveload;

}

SaveLoad::~SaveLoad()
{

}

bool SaveLoad::SaveStats(std::string saveName)
{
	std::fstream file(saveName, std::ios::out | std::ios::binary | std::ios::trunc);

	if (file.is_open()) {
		file.write((const char*)&StatTracker::Instance().GetLevelCount(), sizeof(uint32_t));

		for (unsigned int i = 0; i < StatTracker::Instance().GetLevelCount(); i++)
		{
			file.write((const char*)&StatTracker::Instance().GetLevel(i).levelNumber, sizeof(uint32_t));
			file.write((const char*)&StatTracker::Instance().GetLevel(i).turnCount, sizeof(uint32_t));
			file.write((const char*)&StatTracker::Instance().GetLevel(i).rounds, sizeof(uint32_t));

			uint32_t size_r = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns.size());
			file.write((const char*)&size_r, sizeof(uint32_t));

			for (unsigned int j = 0; j < size_r; j++) {
				uint32_t size_c = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].characterName.size());
				file.write((const char*)&size_c, sizeof(uint32_t));
				file.write(StatTracker::Instance().GetLevel(i).turns[j].characterName.c_str(), size_c);

				file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].unique_id, sizeof(unsigned int));

				uint32_t size_t = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].targets.size());
				file.write((const char*)&size_t, sizeof(uint32_t));

				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.characterName.size());
					file.write((const char*)&sizeName, sizeof(uint32_t));
					file.write(StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.characterName.c_str(), sizeName);
					file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.unique_id, sizeof(unsigned int));
					file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].targets[k].second, sizeof(float));
				}

				file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].round, sizeof(uint32_t));

				file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].value, sizeof(float));

				file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].attackModifier, sizeof(float));

				/*uint32_t size_b = static_cast<uint32_t>(m_levels[i].turns[j].blockValues.size());
				file.write((const char*)&size_b, sizeof(uint32_t));
				for (unsigned int l = 0; l < size_b; l++) {
					file.write((const char*)&m_levels[i].turns[j].blockValues[l], sizeof(float));
				}*/

				uint32_t effect = (uint32_t)StatTracker::Instance().GetLevel(i).turns[j].effect;
				file.write((const char*)&effect, sizeof(uint32_t));

				uint32_t action = (uint32_t)StatTracker::Instance().GetLevel(i).turns[j].actionType;
				file.write((const char*)&action, sizeof(uint32_t));

				//file.write((const char*)&m_levels[i].turns[j].isSheild, sizeof(bool));

				file.write((const char*)&StatTracker::Instance().GetLevel(i).turns[j].isPlayer, sizeof(bool));

				uint32_t size_a = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].actionName.size());
				file.write((const char*)&size_a, sizeof(uint32_t));
				file.write(StatTracker::Instance().GetLevel(i).turns[j].actionName.c_str(), size_a);

			}

			for  (unsigned int l = 0; l < 3; l++) {
				
				uint32_t size_cn = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).characters[l].first.characterName.size());
				file.write((const char*)&size_cn,sizeof(uint32_t));
				file.write(StatTracker::Instance().GetLevel(i).characters[l].first.characterName.c_str(), size_cn);
				file.write((const char*)&StatTracker::Instance().GetLevel(i).characters[l].first.unique_id, sizeof(unsigned int));
				
				/*uint32_t size_cp = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).characters[l].second.size()) * 2;
				file.write((const char*)&size_cp, sizeof(uint32_t));
				file.write((const char*)StatTracker::Instance().GetLevel(i).characters[l].second.c_str(), size_cp);*/
			}
			
		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool SaveLoad::LoadStats(std::string loadFileName)
{

	StatTracker::Instance().ClearLevels();

	std::fstream file(loadFileName, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		uint32_t numLevels = 0;
		file.read((char*)&numLevels, sizeof(uint32_t));

		StatTracker::Instance().SetLevels(numLevels);

		for (unsigned int i = 0; i < numLevels; i++)
		{

			file.read((char*)&StatTracker::Instance().GetLevel(i).levelNumber, sizeof(uint32_t));
			file.read((char*)&StatTracker::Instance().GetLevel(i).turnCount, sizeof(uint32_t));
			file.read((char*)&StatTracker::Instance().GetLevel(i).rounds, sizeof(uint32_t));

			uint32_t size_r = 0;
			file.read((char*)&size_r, sizeof(uint32_t));
			StatTracker::Instance().GetLevel(i).turns.resize(size_r);
			StatTracker::Instance().GetLevel(i).turnCount = size_r;

			for (unsigned int j = 0; j < size_r; j++)
			{
				uint32_t size_c = 0;
				file.read((char*)&size_c, sizeof(uint32_t));
				file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].characterName[0], size_c);

				uint32_t size_t = 0;
				file.read((char*)&size_t, sizeof(uint32_t));
				StatTracker::Instance().GetLevel(i).turns[j].targets.resize(size_t);

				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = 0;
					file.read((char*)&sizeName, sizeof(uint32_t));
					StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.characterName.resize(sizeName);
					file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.characterName[0], sizeName);
					file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.unique_id, sizeof(unsigned int));
					file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].targets[k].second, sizeof(float));
				}

				file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].round, sizeof(uint32_t));

				file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].value, sizeof(float));

				file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].attackModifier, sizeof(float));

				/*uint32_t size_b = 0;
				file.read((char*)&size_b, sizeof(uint32_t));
				m_levels[i].turns[j].blockValues.resize(size_b);
				for (unsigned int l = 0; l < size_b; l++) {
					file.read((char*)&m_levels[i].turns[j].blockValues[l], sizeof(uint32_t));
				}*/

				uint32_t effect = 0;
				file.read((char*)&effect, sizeof(uint32_t));
				StatTracker::Instance().GetLevel(i).turns[j].effect = (EFFECTTYPE)effect;

				uint32_t action = 0;
				file.read((char*)&action, sizeof(uint32_t));
				StatTracker::Instance().GetLevel(i).turns[j].actionType = (StatTracker::Action)action;

				//file.read((char*)&m_levels[i].turns[j].isSheild, sizeof(bool));

				file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].isPlayer, sizeof(bool));

				uint32_t size_a = 0;
				file.read((char*)&size_a, sizeof(uint32_t));
				StatTracker::Instance().GetLevel(i).turns[j].actionName.resize(size_a);
				file.read((char*)StatTracker::Instance().GetLevel(i).turns[j].actionName[0], size_a);
			}
			
			for (unsigned int l = 0; l < 3; l++) {

				uint32_t size_cn = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).characters[l].first.characterName.size());
				file.read((char*)&size_cn, sizeof(uint32_t));
				file.read((char*)&StatTracker::Instance().GetLevel(i).characters[l].first.characterName[0], size_cn);
				file.write((const char*)&StatTracker::Instance().GetLevel(i).characters[l].first.unique_id, sizeof(unsigned int));

				/*uint32_t size_cp = 0;
				file.read((char*)&size_cp, sizeof(uint32_t));
				file.read((char*)StatTracker::Instance().GetLevel(i).characters[l].second.c_str(), size_cp);*/
			}

		}
		file.close();
		return true;
	}
	else {
		return false;
	}
	return false;
}

bool SaveLoad::SaveGame()
{
	//StatTracker::Instance().SaveStats(std::string("profiles/" + saveProfile));

	//SaveLoadOut();
	std::experimental::filesystem::v1::path dir = std::string("profiles/" + m_saveProfile);
	if (std::experimental::filesystem::exists(dir))
	{

	}
	return true;
}

bool SaveLoad::LoadGame(std::string saveProfile)
{
	//StatTracker::Instance().LoadStats(std::string("profiles/" + saveProfile));
	return false;
}

void SaveLoad::AddLoadOut(std::string loadoutName, GameplayTypes::HEROID characterID[3], unsigned int index[3])
{
	Loadout newLoadout;
	newLoadout.characterIDs[0] = characterID[0];
	newLoadout.characterIDs[1] = characterID[1];
	newLoadout.characterIDs[2] = characterID[2];
	newLoadout.index[0] = index[0];
	newLoadout.index[1] = index[1];
	newLoadout.index[2] = index[2];
	newLoadout.name = loadoutName;
	newLoadout.profile = m_saveProfile;
	loadouts[loadoutName] = newLoadout;
}

void SaveLoad::OverrideLoadOut(std::string loadoutName, GameplayTypes::HEROID characterID[3], unsigned int index[3]) {
	loadouts[loadoutName].characterIDs[0] = characterID[0];
	loadouts[loadoutName].characterIDs[1] = characterID[1];
	loadouts[loadoutName].characterIDs[2] = characterID[2];
	loadouts[loadoutName].index[0] = index[0];
	loadouts[loadoutName].index[1] = index[1];
	loadouts[loadoutName].index[2] = index[2];
	loadouts[loadoutName].name = loadoutName;
	loadouts[loadoutName].profile = m_saveProfile;
}

bool SaveLoad::SaveLoadOut()
{
	bool perperExport = true;
	
	
	for (std::map<std::string, Loadout>::iterator it = loadouts.begin(); it != loadouts.end(); it++)
	{
		std::fstream file(std::string("profiles/" + m_saveProfile + "loadouts/" + it->second.name), std::ios::out | std::ios::trunc | std::ios::binary);
		if (file.is_open()) 
		{
			uint32_t nameSize = static_cast<uint32_t>(it->second.name.size());
			file.write((const char*)&nameSize, sizeof(uint32_t));
			file.write(it->second.name.c_str(), nameSize);

			uint32_t profileNameSize = static_cast<uint32_t>(it->second.profile.size());
			file.write((const char*)&profileNameSize, sizeof(uint32_t));
			file.write(it->second.profile.c_str(), profileNameSize);
			for (int j = 0; j < 3/*ARRAYSIZE(loadouts[i].characterIDs)*/; j++)
			{
				unsigned int character = static_cast<unsigned int>(it->second.characterIDs[j]);
				file.write((const char*)&character, sizeof(unsigned int));
				file.write((const char*)&it->second.index[j], sizeof(int));
			}
			file.close();
		}
		else
		{
			perperExport = false;
		}
	}
	return perperExport;
}

bool SaveLoad::LoadLoadOut()
{
	bool perperExport = true;

	std::string loadout_path = "profiles/";
	loadout_path.append(m_saveProfile);
	loadout_path.append("loadouts/");

	std::experimental::filesystem::v1::path dir = loadout_path;
	
	//create_directory();

	unsigned int file_count = dir_file_count(loadout_path);
	std::experimental::filesystem::directory_iterator end_itr;
	for (std::experimental::filesystem::directory_iterator it(dir); it != end_itr; it++)
	{
		std::string file;
		std::string path = Converter::ConvertWStrToStr(it->path().c_str());
		for (int i = 0; i < path.size(); i++)
		{
			file.append(std::string(1, path[i]));
			if (path[i] == '\\')
			{
				file.clear();
			}
		}
		perperExport = LoadLoadOut(file);
		if (!perperExport)
		{
			break;
		}
	}
	return perperExport;
}

bool SaveLoad::LoadLoadOut(std::string loadoutName)
{
	if (loadouts.find(loadoutName) == loadouts.end())
	{
		Loadout loadedLoadOut;
		std::string path = std::string("profiles/" + m_saveProfile + "loadouts/" + loadoutName);
		std::fstream file(path, std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			uint32_t nameSize = 0;
			file.read((char*)&nameSize, sizeof(uint32_t));
			loadedLoadOut.name.resize(nameSize);
			file.read((char*)&loadedLoadOut.name[0], nameSize);

			uint32_t profileNameSize = 0;
			file.read((char*)&profileNameSize, sizeof(uint32_t));
			loadedLoadOut.profile.resize(profileNameSize);
			file.read((char*)&loadedLoadOut.profile[0], profileNameSize);
			for (int j = 0; j < 3/*ARRAYSIZE(loadouts[i].characterIDs)*/; j++)
			{
				unsigned int character = 0;
				file.read((char*)&character, sizeof(unsigned int));
				loadedLoadOut.characterIDs[j] = (GameplayTypes::HEROID)character;
				file.read((char*)&loadedLoadOut.index[j], sizeof(int));
			}
			file.close();
		}
		else
		{
			return false;
		}
		loadouts[loadoutName] = loadedLoadOut;
	}
	return true;
}

unsigned int SaveLoad::dir_file_count(std::string path)
{
	std::experimental::filesystem::v1::path dir = path;
	using std::experimental::filesystem::directory_iterator;
	return static_cast<unsigned int>(std::distance(directory_iterator(path), directory_iterator{}));
}

unsigned int SaveLoad::dir_file_count(std::experimental::filesystem::v1::path dir)
{
	using std::experimental::filesystem::directory_iterator;
	return static_cast<unsigned int>(std::distance(directory_iterator(dir), directory_iterator{}));
}

int SaveLoad::create_directory(std::string& directory_path)
{
	int result = 0;
	std::experimental::filesystem::v1::path dir = directory_path;
	if (!std::experimental::filesystem::exists(dir))
	{
		std::vector<std::string> directories;
		directories.push_back(std::string());
		for (int i = 0; i < directory_path.size(); i++)
		{
			directories[directories.size() - 1].append(std::string(1, directory_path[i]));
			if (directory_path[i] == '/' && (i + static_cast<int>(1)) != directory_path.size())
			{
				directories.push_back(std::string());
			}
		}
		std::string currentPath;
		for (int j = 0; j < directories.size(); j++)
		{
			currentPath.append(directories[j]);
			std::experimental::filesystem::v1::path curr_dir = currentPath;
			if (!std::experimental::filesystem::exists(curr_dir)) {
				int error = _mkdir(currentPath.c_str());
			}
		}
	}
	else 
	{
		result = -2;
	}
	return result;
}

void SaveLoad::SetSaveProfile(std::string saveProfile)
{
	m_saveProfile = saveProfile;
}

void SaveLoad::CreateProfileDirectory()
{
	std::string path = "profiles/" + m_saveProfile;
	create_directory(path);
	std::string loadout_path =  path + "loadouts/";
	create_directory(loadout_path);
}

std::string SaveLoad::GetSaveProfile()
{
	return m_saveProfile;
}

SaveLoad::Loadout SaveLoad::GetLoadOut(std::string loadout_name)
{
	return loadouts[loadout_name];
}