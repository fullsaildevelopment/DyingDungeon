#include "SaveLoad.h"

SaveLoad::SaveLoad()
{
	m_saveProfile = "Profile_0";
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

			uint32_t size_r = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns.size());
			file.write((const char*)&size_r, sizeof(uint32_t));

			for (unsigned int j = 0; j < size_r; j++) {
				uint32_t size_c = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].characterName.size());
				file.write((const char*)&size_c, sizeof(uint32_t));
				file.write(StatTracker::Instance().GetLevel(i).turns[j].characterName.c_str(), size_c);

				uint32_t size_t = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].targets.size());
				file.write((const char*)&size_t, sizeof(uint32_t));

				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = static_cast<uint32_t>(StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.size());
					file.write((const char*)&sizeName, sizeof(uint32_t));
					file.write(StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.c_str(), sizeName);
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
		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}

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
					StatTracker::Instance().GetLevel(i).turns[j].targets[k].first.resize(sizeName);
					file.read((char*)&StatTracker::Instance().GetLevel(i).turns[j].targets[k].first[0], sizeName);
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
		}
		file.close();
		return true;
	}
	else {
		return false;
	}
}

bool SaveLoad::SaveGame(std::string saveProfile)
{
	StatTracker::Instance().SaveStats(std::string("profiles/" + saveProfile));
	SaveLoadOut();
	return true;
}

bool SaveLoad::LoadGame(std::string saveProfile)
{
	StatTracker::Instance().LoadStats(std::string("profiles/" + saveProfile));
	return false;
}

void SaveLoad::AddLoadOut(std::string loadoutName, HEROID characterID_0, HEROID characterID_1, HEROID characterID_2)
{
	Loadout newLoadout;
	newLoadout.characterIDs[0] = characterID_0;
	newLoadout.characterIDs[1] = characterID_1;
	newLoadout.characterIDs[2] = characterID_2;
	newLoadout.name = loadoutName;
	newLoadout.profile = m_saveProfile;
	loadouts.push_back(newLoadout);
}

bool SaveLoad::SaveLoadOut()
{
	for (int i = 0; i < loadouts.size(); i++) 
	{
		/*std::fstream file(std::string(m_saveProfile + "_" + std::to_string(i)), std::ios::out | std::ios::trunc | std::ios::binary);
		if (file.is_open()) 
		{
			uint32_t 
			file.write((const char*)&, sizeof(size_t));
			file.write((loadouts[i].name.c_str());
			file.close();
		}*/
	}
	return false;
}

bool SaveLoad::LoadLoadOut()
{
	return false;
}

bool SaveLoad::LoadLoadOut(std::string loadoutName)
{
	return false;
}

void SaveLoad::SetSaveProfile(std::string saveProfile)
{
	m_saveProfile = saveProfile;
}

std::string SaveLoad::GetSaveProfile()
{
	return m_saveProfile;
}
