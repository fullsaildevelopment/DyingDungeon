#include "StatTracker.h"

StatTracker* StatTracker::m_p_Instance = nullptr;

StatTracker::StatTracker()
{
}

StatTracker::~StatTracker()
{
	delete m_p_Instance;
}

StatTracker* StatTracker::Instance()
{
	if (!m_p_Instance) {
		m_p_Instance = new StatTracker;
	}
	return m_p_Instance;
}

void StatTracker::StartNextTurn()
{
	StatTracker::Turn newTurn;
	newTurn.round = m_towerManager->GetBattleInstance()->GetCurrentRound();
	m_levels[m_currentLevel - 1].turns.push_back(newTurn);
}

void StatTracker::StartNextLevel()
{
	StatTracker::Level newLevel;
	newLevel.levelNumber = m_towerManager->GetCurrentLevel();
	m_levels.push_back(newLevel);
}

float StatTracker::CalculatDamageDone(std::string name) 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++) 
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++) 
		{
			if (name == m_levels[i].turns[j].characterName && m_levels[i].turns[j].effect == STATS::Atk) {
				total += m_levels[i].turns[j].value;
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
			if (name == m_levels[i].turns[j].targetName && m_levels[i].turns[j].effect == STATS::Atk) {
				total += m_levels[i].turns[j].value;
			}
		}
	}
	return total;
}