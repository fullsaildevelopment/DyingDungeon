#include "StatTracker.h"

StatTracker::StatTracker()
{
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

void StatTracker::StartNextTurn()
{
	Turn newTurn;
	newTurn.round = m_towerManager->GetBattleInstance()->GetCurrentRound();
	m_levels[m_currentLevel - 1].turns.push_back(newTurn);
	m_levels[m_currentLevel - 1].turnCount++;
}

void StatTracker::StartNextLevel()
{
	StatTracker::Level newLevel;
	newLevel.levelNumber = m_towerManager->GetCurrentLevel();
	m_levels.push_back(newLevel);
	m_currentLevel = newLevel.levelNumber;
}

void StatTracker::SaveStats(std::string saveName)
{
	std::fstream file(saveName, std::ios::out | std::ios::binary | std::ios::trunc);

	if (file.is_open()) {
		file.write((const char*)&m_currentLevel, sizeof(uint32_t));

		for (int i = 0; i < m_currentLevel; i++) 
		{
			file.write((const char*)&m_levels[i].levelNumber, sizeof(uint32_t));

			uint32_t size_r = m_levels[i].turns.size();
			file.write((const char*)&size_r, sizeof(uint32_t));

			for (int j = 0; j < size_r; j++) {
				uint32_t size_c = m_levels[i].turns[j].characterName.size();
				file.write((const char*)&size_c, sizeof(uint32_t));
				file.write(m_levels[i].turns[j].characterName.c_str(), size_c);

				uint32_t size_t = m_levels[i].turns[j].targetNames.size();
				file.write((const char*)&size_t, sizeof(uint32_t));
				for (int k = 0; k < size_t; k++) {
					uint32_t sizeName = m_levels[i].turns[j].targetNames[k].size();
					file.write((const char*)&sizeName, sizeof(uint32_t));
					file.write(m_levels[i].turns[j].targetNames[k].c_str(), sizeName);
				}

				file.write((const char*)&m_levels[i].turns[j].round, sizeof(uint32_t));

				file.write((const char*)&m_levels[i].turns[j].value, sizeof(float));

				uint32_t size_b = m_levels[i].turns[j].blockValues.size();
				file.write((const char*)&size_b, sizeof(uint32_t));
				for (int l = 0; l < size_b; l++) {
					file.write((const char*)&m_levels[i].turns[j].blockValues[l], sizeof(float));
				}

				uint32_t effect = (uint32_t)m_levels[i].turns[j].effect;
				file.write((const char*)&effect, sizeof(uint32_t));

				uint32_t action = (uint32_t)m_levels[i].turns[j].actionType;
				file.write((const char*)&action, sizeof(uint32_t));

				file.write((const char*)&m_levels[i].turns[j].isSpell, sizeof(bool));

				uint32_t size_a = m_levels[i].turns[j].actionName.size();
				file.write((const char*)&size_a, sizeof(uint32_t));
				file.write(m_levels[i].turns[j].actionName.c_str(), size_a);

			}
		}
	}
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

		for (int i = 0; i < numLevels; i++)
		{

			file.read((char*)&m_levels[i].levelNumber, sizeof(uint32_t));
			uint32_t size_r = 0;
			file.read((char*)size_r, sizeof(uint32_t));
			m_levels[i].turns.resize(size_r);
			m_levels[i].turnCount = size_r;

			for (int j = 0; j < size_r; j++) 
			{
				uint32_t size_c = 0;
				file.read((char*)&size_c, sizeof(uint32_t));
				file.read((char*)&m_levels[i].turns[j].characterName[0], size_c);

				uint32_t size_t = 0;
				file.read((char*)&size_t, sizeof(uint32_t));
				m_levels[i].turns[j].targetNames.resize(size_t);
				for (int k = 0; k < size_t; k++) {
					uint32_t sizeName = 0;
					file.read((char*)&sizeName, sizeof(uint32_t));
					m_levels[i].turns[j].targetNames[k].resize(sizeName);
					file.read((char*)&m_levels[i].turns[j].targetNames[k][0], sizeName);
				}

				file.read((char*)&m_levels[i].turns[j].round, sizeof(uint32_t));

				file.read((char*)&m_levels[i].turns[j].value, sizeof(float));

				//file.read((char*)&m_levels[i].turns[j].blockValue, sizeof(float));

				uint32_t size_b = 0;
				file.read((char*)&size_b, sizeof(uint32_t));
				m_levels[i].turns[j].blockValues.resize(size_b);
				for (int l = 0; l < size_b; l++) {
					file.read((char*)&m_levels[i].turns[j].blockValues[l], sizeof(uint32_t));
				}

				uint32_t effect = 0;
				file.read((char*)&effect, sizeof(uint32_t));
				m_levels[i].turns[j].effect = (STATS)effect;

				uint32_t action = 0;
				file.read((char*)&action, sizeof(uint32_t));
				m_levels[i].turns[j].actionType = (Action)action;

				file.read((char*)&m_levels[i].turns[j].isSpell, sizeof(bool));

				uint32_t size_a = 0;
				file.read((char*)&size_a, sizeof(uint32_t));
				m_levels[i].turns[j].actionName.resize(size_a);
				file.read((char*)&m_levels[i].turns[j].actionName[0], size_a);
			}
		}
	}
}

void StatTracker::LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].characterName = cddEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].actionType = Action::Attack;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].characterName = cddEvent->attackerName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].effect = cddEvent->actionEffect;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].isSpell = cddEvent->isSpell;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].value = cddEvent->damageAmount;

}

void StatTracker::LogTakeDamageEvent(CharacterTakeDamage* ctdEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].targetNames.push_back(ctdEvent->targetName);
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].blockValues.push_back(ctdEvent->mitigationAmount);

}

void StatTracker::LogHealingEvent(CharacterHealsCharacterEvent* chcEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].characterName = chcEvent->healerName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].actionName = chcEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].actionType = Action::Aid;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].isSpell = chcEvent->isSpell;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].value = chcEvent->health;

}

void StatTracker::LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent) 
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount].targetNames.push_back(crhEvent->targetName);
}

void StatTracker::LevelStartReflex(LevelStartEvent* lsEvent)
{
	StartNextLevel();
}

void StatTracker::TurnStartReflex(TurnStartEvent* tsEvent)
{
	StartNextTurn();
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

unsigned int StatTracker::CalculateRoundsInLevel(unsigned int levelNumber) 
{
	return static_cast<unsigned int>(m_levels[levelNumber - 1].turnCount / 8);
}