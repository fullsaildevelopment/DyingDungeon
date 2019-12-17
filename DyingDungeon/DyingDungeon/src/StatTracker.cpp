#include "StatTracker.h"

StatTracker* StatTracker::m_p_Instance = nullptr;

StatTracker::StatTracker()
{
}

StatTracker::~StatTracker()
{
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
	//m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turns.size() - 1].push_back();
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
			if (name == m_levels[i].turns[j].targetName) {
				//total += m_levels[i].turns[j].damage;
			}
		}
	}
	return total;
}