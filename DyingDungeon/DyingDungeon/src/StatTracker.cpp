#include "StatTracker.h"

StatTracker::StatTracker()
{

	m_currentLevel = 1;


	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogDamageDeltEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogTakeDamageEvent);

	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogHealingEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogReciveHealingEvent);

	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LevelStartReflex);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::TurnStartReflex);
}

StatTracker::~StatTracker()
{

}

StatTracker& StatTracker::Instance()
{
	static StatTracker instance_stat_tracker_manager;
	return instance_stat_tracker_manager;
}

//void StatTracker::StartNextTurn()
//{
//	Turn newTurn;
//	newTurn.round = m_towerManager->GetBattleInstance()->GetCurrentRound();
//	m_levels[m_currentLevel - 1].turns.push_back(newTurn);
//	m_levels[m_currentLevel - 1].turnCount++;
//}
//
//void StatTracker::StartNextLevel()
//{
//	StatTracker::Level newLevel;
//	newLevel.levelNumber = m_towerManager->GetCurrentLevel();
//	m_levels.push_back(newLevel);
//	m_currentLevel = newLevel.levelNumber;
//}

void StatTracker::SaveStats(std::string saveName)
{
	std::fstream file(saveName, std::ios::out | std::ios::binary | std::ios::trunc);

	if (file.is_open()) {
		file.write((const char*)&m_currentLevel, sizeof(uint32_t));

		for (unsigned int i = 0; i < m_currentLevel; i++) 
		{
			file.write((const char*)&m_levels[i].levelNumber, sizeof(uint32_t));

			uint32_t size_r = static_cast<uint32_t>(m_levels[i].turns.size());
			file.write((const char*)&size_r, sizeof(uint32_t));

			for (unsigned int j = 0; j < size_r; j++) {
				uint32_t size_c = static_cast<uint32_t>(m_levels[i].turns[j].characterName.size());
				file.write((const char*)&size_c, sizeof(uint32_t));
				file.write(m_levels[i].turns[j].characterName.c_str(), size_c);

				uint32_t size_t = static_cast<uint32_t>(m_levels[i].turns[j].targetNames.size());
				file.write((const char*)&size_t, sizeof(uint32_t));
				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = static_cast<uint32_t>(m_levels[i].turns[j].targetNames[k].size());
					file.write((const char*)&sizeName, sizeof(uint32_t));
					file.write(m_levels[i].turns[j].targetNames[k].c_str(), sizeName);
				}

				file.write((const char*)&m_levels[i].turns[j].round, sizeof(uint32_t));

				file.write((const char*)&m_levels[i].turns[j].value, sizeof(float));

				uint32_t size_b = static_cast<uint32_t>(m_levels[i].turns[j].blockValues.size());
				file.write((const char*)&size_b, sizeof(uint32_t));
				for (unsigned int l = 0; l < size_b; l++) {
					file.write((const char*)&m_levels[i].turns[j].blockValues[l], sizeof(float));
				}

				uint32_t effect = (uint32_t)m_levels[i].turns[j].effect;
				file.write((const char*)&effect, sizeof(uint32_t));

				uint32_t action = (uint32_t)m_levels[i].turns[j].actionType;
				file.write((const char*)&action, sizeof(uint32_t));

				file.write((const char*)&m_levels[i].turns[j].isSpell, sizeof(bool));

				file.write((const char*)&m_levels[i].turns[j].isPlayer, sizeof(bool));

				uint32_t size_a = static_cast<uint32_t>(m_levels[i].turns[j].actionName.size());
				file.write((const char*)&size_a, sizeof(uint32_t));
				file.write(m_levels[i].turns[j].actionName.c_str(), size_a);

			}
		}
	}

	file.close();

}

void StatTracker::LoadStats(std::string loadFileName)
{

	m_levels.clear();

	std::fstream file(loadFileName, std::ios::in | std::ios::binary);

	if (file.is_open()) 
	{
		uint32_t numLevels = 0;
		file.read((char*)&numLevels, sizeof(uint32_t));

		m_levels.resize(numLevels);

		for (unsigned int i = 0; i < numLevels; i++)
		{

			file.read((char*)&m_levels[i].levelNumber, sizeof(uint32_t));
			uint32_t size_r = 0;
			file.read((char*)&size_r, sizeof(uint32_t));
			m_levels[i].turns.resize(size_r);
			m_levels[i].turnCount = size_r;

			for (unsigned int j = 0; j < size_r; j++) 
			{
				uint32_t size_c = 0;
				file.read((char*)&size_c, sizeof(uint32_t));
				file.read((char*)&m_levels[i].turns[j].characterName[0], size_c);

				uint32_t size_t = 0;
				file.read((char*)&size_t, sizeof(uint32_t));
				m_levels[i].turns[j].targetNames.resize(size_t);
				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = 0;
					file.read((char*)&sizeName, sizeof(uint32_t));
					m_levels[i].turns[j].targetNames[k].resize(sizeName);
					file.read((char*)&m_levels[i].turns[j].targetNames[k][0], sizeName);
				}

				file.read((char*)&m_levels[i].turns[j].round, sizeof(uint32_t));

				file.read((char*)&m_levels[i].turns[j].value, sizeof(float));

				uint32_t size_b = 0;
				file.read((char*)&size_b, sizeof(uint32_t));
				m_levels[i].turns[j].blockValues.resize(size_b);
				for (unsigned int l = 0; l < size_b; l++) {
					file.read((char*)&m_levels[i].turns[j].blockValues[l], sizeof(uint32_t));
				}

				uint32_t effect = 0;
				file.read((char*)&effect, sizeof(uint32_t));
				m_levels[i].turns[j].effect = (STATS)effect;

				uint32_t action = 0;
				file.read((char*)&action, sizeof(uint32_t));
				m_levels[i].turns[j].actionType = (Action)action;

				file.read((char*)&m_levels[i].turns[j].isSpell, sizeof(bool));

				file.read((char*)&m_levels[i].turns[j].isPlayer, sizeof(bool));

				uint32_t size_a = 0;
				file.read((char*)&size_a, sizeof(uint32_t));
				m_levels[i].turns[j].actionName.resize(size_a);
				file.read((char*)&m_levels[i].turns[j].actionName[0], size_a);
			}
		}
	}

	file.close();
}

void StatTracker::LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].characterName = cddEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Attack;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].effect = cddEvent->actionEffect;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].isSpell = cddEvent->isSpell;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].value = cddEvent->damageAmount;

}

void StatTracker::LogTakeDamageEvent(CharacterTakeDamage* ctdEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targetNames.push_back(ctdEvent->targetName);
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].blockValues.push_back(ctdEvent->mitigationAmount);

}

void StatTracker::LogHealingEvent(CharacterHealsEvent* chcEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionName = chcEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Aid;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].isSpell = chcEvent->isSpell;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].value = chcEvent->health;

}

void StatTracker::LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent) 
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].targetNames.push_back(crhEvent->targetName);
	if (m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].characterName == crhEvent->targetName) {
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Defend;
	}
}

void StatTracker::LevelStartReflex(LevelStartEvent* lsEvent)
{
	StatTracker::Level newLevel;
	newLevel.levelNumber = lsEvent->levelNumber;
	m_levels.push_back(newLevel);
	m_currentLevel = newLevel.levelNumber;
}

void StatTracker::TurnStartReflex(TurnStartEvent* tsEvent)
{
	StatTracker::Turn newTurn;
	m_levels[m_currentLevel - 1].turnCount = tsEvent->turn;
	newTurn.characterName = tsEvent->characterName;
	newTurn.round = tsEvent->round;
	m_levels[m_currentLevel - 1].turns.push_back(newTurn);
}

unsigned int StatTracker::GetAttackCount()
{
	return 0;
}

unsigned int StatTracker::GetAttackCount(std::string name)
{
	return 0;
}

unsigned int StatTracker::GetDefendCount()
{
	return 0;
}

unsigned int StatTracker::GetDefendCount(std::string name)
{
	return 0;
}

unsigned int StatTracker::GetAidCount()
{
	return 0;
}

unsigned int StatTracker::GetAidCount(std::string name)
{
	return 0;
}

float StatTracker::CalculateDamageDealt()
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].effect == STATS::Atk && m_levels[i].turns[j].isPlayer)
			{
				total += m_levels[i].turns[j].value;
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDealt(std::string name) 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++) 
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++) 
		{
			if (name == m_levels[i].turns[j].characterName && m_levels[i].turns[j].effect == STATS::Atk) 
			{
				total += m_levels[i].turns[j].value;
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDone()
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].isPlayer && m_levels[i].turns[j].effect == STATS::Atk)
			{
				for (unsigned int k = 0; k < m_levels[i].turns[j].blockValues.size(); k++)
				{
					total += m_levels[i].turns[j].value - (m_levels[i].turns[j].blockValues[k] * m_levels[i].turns[j].value);
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDone(std::string name)
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (name == m_levels[i].turns[j].characterName && m_levels[i].turns[j].effect == STATS::Atk)
			{
				for (unsigned int k = 0; k < m_levels[i].turns[j].blockValues.size(); k++)
				{
					total += m_levels[i].turns[j].value - (m_levels[i].turns[j].blockValues[k] * m_levels[i].turns[j].value);
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageTaken()
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[i].turns[j].targetNames.size(); k++)
			{
				if (m_levels[i].turns[j].effect == STATS::Atk && m_levels[i].turns[j].isPlayer)
				{
					total += m_levels[i].turns[j].value - (m_levels[i].turns[j].blockValues[k] * m_levels[i].turns[j].value);
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageTaken(std::string name) 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[i].turns[j].targetNames.size(); k++) 
			{
				if (name == m_levels[i].turns[j].targetNames[k] && m_levels[i].turns[j].effect == STATS::Atk) 
				{
					total += m_levels[i].turns[j].value - (m_levels[i].turns[j].blockValues[k]* m_levels[i].turns[j].value);
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateAttackPercentage()
{
	float toatalTurns = 1.0f;
	float totalAttacks = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].isPlayer) 
			{
				toatalTurns++;
				if (m_levels[i].turns[j].effect == STATS::Atk)
				{
					totalAttacks++;
				}
			}
		}
	}

	return totalAttacks / toatalTurns;
}

float StatTracker::CalculateAttackPercentage(std::string name)
{

	float toatalTurns = 1.0f;
	float totalAttacks = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].characterName == name) 
			{
				toatalTurns++;
				if (m_levels[i].turns[j].effect == STATS::Atk)
				{
					totalAttacks++;
				}
			}
		}
	}

	return totalAttacks / toatalTurns;
}

float StatTracker::roundf(float num, unsigned int decimal_places) {
	return static_cast<float>(floor(num * std::powf(10.0f, decimal_places))/std::powf(10.0f, decimal_places));
}

double StatTracker::round(double num, unsigned int decimal_places) {
	return static_cast<double>(floor(num * std::pow(10.0, decimal_places)) / std::pow(10.0, decimal_places));
}

void StatTracker::OutputStatSheet()
{
	time_t currTime;
	struct tm timeinfo;
	char fileName[27];

	time(&currTime);
	localtime_s(&timeinfo, &currTime);

	strftime(fileName, 27, "stat_log_%d_%m_%y_%H%M.txt", &timeinfo);
	std::fstream file(fileName, std::ios::out);
	if (file.is_open())
	{
		std::string fileText = "----------------------------------------------------------\n                        Game Info\n----------------------------------------------------------\n\n                 Attack     Defend      Aid\n               | ";
		float attackPercentage = CalculateAttackPercentage();
		//fileText.append(std::to_string(32));
		file << fileText;
		std::cout << fileText;
	}
	file.close();
}