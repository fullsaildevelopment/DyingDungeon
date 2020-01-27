#include "StatTracker.h"
#define DEBUG_STAT_TRACK
StatTracker::StatTracker()
{
	m_currentLevel = 1;
	m_maxPlayerCount = 3;
	m_p_canvas = nullptr;

	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogDamageDeltEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogTakeDamageEvent);

	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogHealingEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogReciveHealingEvent);

	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogBuffingEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LogDebuffingEvent);

	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::LevelStartReflex);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::TurnStartReflex);
	Odyssey::EventManager::getInstance().subscribe(this, &StatTracker::UpdateRewardScreen);
}

StatTracker::~StatTracker()
{
	//delete m_p_rewardsScreen;
}

StatTracker& StatTracker::Instance()
{
	static StatTracker instance_stat_tracker_manager;
	return instance_stat_tracker_manager;
}

StatTracker::Level& StatTracker::GetLevel(unsigned int index)
{
	return m_levels[index];
}

unsigned int& StatTracker::GetLevelCount()
{
	return m_currentLevel;
}

size_t StatTracker::GetLevelSize()
{
	return m_levels.size();
}

void StatTracker::SetLevels(unsigned int numLevels)
{
	m_currentLevel = numLevels;
	m_levels.resize(numLevels);
}

void StatTracker::ClearLevels()
{
	m_levels.clear();
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

void StatTracker::SetCanvas(Odyssey::UICanvas* canvas)
{
	m_p_canvas = canvas;
}

void StatTracker::UpdateRewardScreen(RewardsActiveEvnet* raEvent) 
{
	std::vector<std::string> temp_nameList = GetListPlayerCharacterNames(raEvent->level);
	/*for (unsigned int i = 1; i <= temp_nameList.size(); i++) {
		std::wstring rewardsText;
		rewardsText.append(   L" - Attack: " + FormatToPercentageW(CalculatePercentageStat(temp_nameList[i - 1], Action::Attack, raEvent->level)) + L"%"
							+ L" - Defend: " + FormatToPercentageW(CalculatePercentageStat(temp_nameList[i - 1], Action::Defend, raEvent->level)) + L"%"
							+ L" - Aid: " + FormatToPercentageW(CalculatePercentageStat(temp_nameList[i - 1], Action::Aid, raEvent->level)) + L"%\n"
							+ L" - Damage Done: " + FormatToPercentageW(CalculateDamageDone(temp_nameList[i - 1], raEvent->level)) 
							+ L" - Damage Taken: " + FormatToPercentageW(CalculateDamageTaken(temp_nameList[i - 1], raEvent->level)) + L"\n"
							+ L" - Health Gained: " + FormatToPercentageW(CalculateHealthRecived(temp_nameList[i - 1], raEvent->level)));

		m_p_canvas->getElements<Odyssey::Text2D>()[i - 1]->setText(rewardsText);
		m_p_canvas->getElements<Odyssey::Sprite2D>()[i - 1]->setSprite();
	}*/
	for (int txt = 3, i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++)
		{
			std::wstring rewardsText;
			if (txt < 6)
			{
				rewardsText.append(L"Attack: " + FormatToPercentageW(CalculatePercentageStat(temp_nameList[j], Action::Attack, raEvent->level)) + L"%\n" +
								   L"Damage Dealt: "+ FormatToPercentageW(CalculateDamageDealt(temp_nameList[j], raEvent->level)) + L"\n" + 
								   L"Damage Success: " + FormatToPercentageW(CalculatePercentDamageSuccess(temp_nameList[j], raEvent->level)) + L"%");
			}
			else if (txt < 9)
			{
				rewardsText.append(L"Defend: " + FormatToPercentageW(CalculatePercentageStat(temp_nameList[j], Action::Defend, raEvent->level)) + L"%\n" + 
								   L"Damage Taken: " + FormatToPercentageW(CalculateDamageTaken(temp_nameList[j], raEvent->level)) + L"\n" + 
								   L"Damage Blocked: " + FormatToPercentageW(CalculateDamageMitigatated(temp_nameList[j], raEvent->level)) + L"%\n" + 
								   L"Health Gained: " + FormatToPercentageW(CalculateHealthRecived(temp_nameList[j], raEvent->level)));
			}
			else if (txt < 12)
			{
				rewardsText.append(L"Aid: " + FormatToPercentageW(CalculatePercentageStat(temp_nameList[j], Action::Aid, raEvent->level)) + L"%\n" + 
								   L"Heal: " + FormatToPercentageW(CalculateHealthRecived(temp_nameList[j], raEvent->level)) + L"\n" + 
								   L"Defence Buff: " + FormatToPercentageW(CalculateShieldGiven(temp_nameList[j], raEvent->level)));
			}
			m_p_canvas->getElements<Odyssey::Text2D>()[txt]->setText(rewardsText);
			txt++;
		}

	}
	//OutputStatSheet();
}

bool StatTracker::SaveStats(std::string saveName)
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

				uint32_t size_t = static_cast<uint32_t>(m_levels[i].turns[j].targets.size());
				file.write((const char*)&size_t, sizeof(uint32_t));

				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = static_cast<uint32_t>(m_levels[i].turns[j].targets[k].first.size());
					file.write((const char*)&sizeName, sizeof(uint32_t));
					file.write(m_levels[i].turns[j].targets[k].first.c_str(), sizeName);
					file.write((const char*)&m_levels[i].turns[j].targets[k].second, sizeof(float));
				}

				file.write((const char*)&m_levels[i].turns[j].round, sizeof(uint32_t));

				file.write((const char*)&m_levels[i].turns[j].value, sizeof(float));

				file.write((const char*)&m_levels[i].turns[j].attackModifier, sizeof(float));

				/*uint32_t size_b = static_cast<uint32_t>(m_levels[i].turns[j].blockValues.size());
				file.write((const char*)&size_b, sizeof(uint32_t));
				for (unsigned int l = 0; l < size_b; l++) {
					file.write((const char*)&m_levels[i].turns[j].blockValues[l], sizeof(float));
				}*/

				uint32_t effect = (uint32_t)m_levels[i].turns[j].effect;
				file.write((const char*)&effect, sizeof(uint32_t));

				uint32_t action = (uint32_t)m_levels[i].turns[j].actionType;
				file.write((const char*)&action, sizeof(uint32_t));

				//file.write((const char*)&m_levels[i].turns[j].isSheild, sizeof(bool));

				file.write((const char*)&m_levels[i].turns[j].isPlayer, sizeof(bool));

				uint32_t size_a = static_cast<uint32_t>(m_levels[i].turns[j].actionName.size());
				file.write((const char*)&size_a, sizeof(uint32_t));
				file.write(m_levels[i].turns[j].actionName.c_str(), size_a);

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

bool StatTracker::LoadStats(std::string loadFileName)
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
				m_levels[i].turns[j].targets.resize(size_t);

				for (unsigned int k = 0; k < size_t; k++) {
					uint32_t sizeName = 0;
					file.read((char*)&sizeName, sizeof(uint32_t));
					m_levels[i].turns[j].targets[k].first.resize(sizeName);
					file.read((char*)&m_levels[i].turns[j].targets[k].first[0], sizeName);
					file.read((char*)&m_levels[i].turns[j].targets[k].second, sizeof(float));
				}

				file.read((char*)&m_levels[i].turns[j].round, sizeof(uint32_t));

				file.read((char*)&m_levels[i].turns[j].value, sizeof(float));

				file.read((char*)&m_levels[i].turns[j].attackModifier, sizeof(float));

				/*uint32_t size_b = 0;
				file.read((char*)&size_b, sizeof(uint32_t));
				m_levels[i].turns[j].blockValues.resize(size_b);
				for (unsigned int l = 0; l < size_b; l++) {
					file.read((char*)&m_levels[i].turns[j].blockValues[l], sizeof(uint32_t));
				}*/

				uint32_t effect = 0;
				file.read((char*)&effect, sizeof(uint32_t));
				m_levels[i].turns[j].effect = (EFFECTTYPE)effect;

				uint32_t action = 0;
				file.read((char*)&action, sizeof(uint32_t));
				m_levels[i].turns[j].actionType = (Action)action;

				//file.read((char*)&m_levels[i].turns[j].isSheild, sizeof(bool));

				file.read((char*)&m_levels[i].turns[j].isPlayer, sizeof(bool));

				uint32_t size_a = 0;
				file.read((char*)&size_a, sizeof(uint32_t));
				m_levels[i].turns[j].actionName.resize(size_a);
				file.read((char*)&m_levels[i].turns[j].actionName[0], size_a);
			}
		}
		file.close();
		return true;
	}
	else {
		return false;
	}
}

void StatTracker::LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionName = cddEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Attack;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].effect = cddEvent->actionEffect;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].value = cddEvent->damageAmount;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].attackModifier = cddEvent->atkMod;

}

void StatTracker::LogTakeDamageEvent(CharacterTakeDamage* ctdEvent)
{
	//m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targetNames.push_back(ctdEvent->targetName);
	//m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].blockValues.push_back(ctdEvent->mitigationAmount);
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targets.push_back(std::make_pair(ctdEvent->targetName, ctdEvent->mitigationAmount));
}

void StatTracker::LogHealingEvent(CharacterHealsEvent* chcEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionName = chcEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Aid;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].value = chcEvent->health;

}

void StatTracker::LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent) 
{
	if (m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].characterName == crhEvent->targetName &&
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType != Action::Attack) 
	{
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targets.push_back(std::make_pair(std::string("HEAL" + crhEvent->targetName), crhEvent->healingAmount));
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Defend;
	}
}

void StatTracker::LogBuffingEvent(CharacterBuffsEvent* cbEvent) 
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionName = cbEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Aid;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].value = cbEvent->buffValue;
	//m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].isSheild = true;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].effect = cbEvent->buffType;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targets.push_back(std::make_pair(cbEvent->targetName, cbEvent->buffValue));

	if (m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].characterName == cbEvent->targetName && 
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targets.size() <= 1 &&
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType != Action::Attack) 
	{
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Defend;
	}
}

void StatTracker::LogDebuffingEvent(CharacterDebuffsEvent* cdEvent)
{
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionName = cdEvent->actionName;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Attack;
	//m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].value = cdEvent->debuffValue;
	//m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].isSheild = true;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].effect = cdEvent->debuffType;
	m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targets.push_back(std::make_pair(std::string("DEBUFF" + cdEvent->targetName), cdEvent->debuffValue));

	/*if (m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].characterName == cdEvent->targetName &&
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].targetNames.size() <= 1 &&
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType != Action::Attack)
	{
		m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].actionType = Action::Defend;
	}*/
}

void StatTracker::LevelStartReflex(LevelStartEvent* lsEvent)
{
	if (lsEvent->levelNumber != m_levels.size()) {
		if (m_levels.size() > 0) {
			m_levels[m_currentLevel - 1].rounds = m_levels[m_currentLevel - 1].turns[m_levels[m_currentLevel - 1].turnCount - 1].round;
		}
		StatTracker::Level newLevel;
		newLevel.levelNumber = lsEvent->levelNumber;
		m_levels.push_back(newLevel);
		m_currentLevel = newLevel.levelNumber;
	}
}

void StatTracker::TurnStartReflex(TurnStartEvent* tsEvent)
{
	StatTracker::Turn newTurn;
	m_levels[m_currentLevel - 1].turnCount = tsEvent->turn;
	newTurn.characterName = tsEvent->characterName;
	newTurn.round = tsEvent->round;
	newTurn.isPlayer = tsEvent->isPlayer;
	m_levels[m_currentLevel - 1].turns.push_back(newTurn);
	m_levels[m_currentLevel - 1].turnCount = static_cast<uint32_t>(m_levels[m_currentLevel - 1].turns.size());
}

unsigned int StatTracker::GetStatCount(Action stat)
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].isPlayer && m_levels[i].turns[j].actionType == stat)
			{
				count++;
			}
		}
	}

	return count;
}

unsigned int StatTracker::GetStatCount(Action stat, unsigned int level, unsigned int round)
{
	unsigned int count = 0;
	unsigned int curr_round = 0;
	unsigned int index = level - 1;

	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (curr_round != m_levels[index].turns[j].round)
		{
			curr_round = m_levels[index].turns[j].round;
			if (round < curr_round)
			{
				break;
			}
		}
		if (m_levels[index].turns[j].isPlayer && m_levels[index].turns[j].actionType == stat && m_levels[index].turns[j].round == round)
		{
			if (curr_round == round) {
				count++;
			}
		}
	}

	return count;
}

unsigned int StatTracker::GetStatCount(std::string name, Action stat)
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].characterName == name && m_levels[i].turns[j].actionType == stat)
			{
				count++;
			}
		}
	}

	return count;
}

unsigned int StatTracker::GetRoundCount(unsigned int level)
{
	return m_levels[level - 1].rounds;
}

std::vector<std::string>& StatTracker::GetListPlayerCharacterNames()
{
	for (int i = 0; i < m_levels.size(); i++) 
	{
		for (int j = 0; j < m_levels[i].turns.size(); j++) 
		{
			if (m_levels[i].turns[j].isPlayer) 
			{
				if (characterNames.empty()) 
				{
					characterNames.push_back(m_levels[i].turns[j].characterName);
				}
				else 
				{
					bool inList = false;
					for (int k = 0; k < characterNames.size(); k++) {
						if (characterNames[k] == m_levels[i].turns[j].characterName) {
							inList = true;
							break;
						}
					}
					if (!inList) 
					{
						characterNames.push_back(m_levels[i].turns[j].characterName);
						if (characterNames.size() == m_maxPlayerCount) {
							break;
						}
					}
				}
			}
		}
		if (characterNames.size() == m_maxPlayerCount) {
			break;
		}
	}
	return characterNames;
}

std::vector<std::string> StatTracker::GetListPlayerCharacterNames(unsigned int level)
{
	std::vector<std::string> level_characterNames;
	for (int j = 0; j < m_levels[level - 1].turns.size(); j++)
	{
		if (m_levels[level - 1].turns[j].isPlayer)
		{
			if (level_characterNames.empty())
			{
				level_characterNames.push_back(m_levels[level - 1].turns[j].characterName);
			}
			else
			{
				bool inList = false;
				for (int k = 0; k < characterNames.size(); k++) {
					if (level_characterNames[k] == m_levels[level - 1].turns[j].characterName) {
						inList = true;
						break;
					}
				}
				if (!inList)
				{
					level_characterNames.push_back(m_levels[level - 1].turns[j].characterName);
					if (level_characterNames.size() == m_maxPlayerCount) {
						break;
					}
				}
			}
		}
	}
	
	return level_characterNames;
}

float StatTracker::CalculateDamageDealt()
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].actionType == Action::Attack && m_levels[i].turns[j].isPlayer)
			{
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++) {
					if ((m_levels[i].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") || (m_levels[i].turns[j].targets[k].first.substr(0, 4) != "HEAL")) {
						total += m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier);
					}
				}
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
			if (name == m_levels[i].turns[j].characterName && m_levels[i].turns[j].actionType == Action::Attack) 
			{
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++) {
					if ((m_levels[i].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") || (m_levels[i].turns[j].targets[k].first.substr(0, 4) != "HEAL")) {
						total += m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier);
					}
				}

			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDealt(std::string name, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (name == m_levels[index].turns[j].characterName && m_levels[index].turns[j].actionType == Action::Attack)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
				if ((m_levels[index].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.substr(0, 4) != "HEAL")) {
					total += m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier);
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDealt(unsigned int level, unsigned int round)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	unsigned int curr_round = 0;

	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (curr_round != m_levels[index].turns[j].round)
		{
			curr_round = m_levels[index].turns[j].round;
			if (round < curr_round)
			{ 
				break;
			}
		}
		if (m_levels[index].turns[j].isPlayer && m_levels[index].turns[j].actionType == Action::Attack)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
				if ((m_levels[index].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") 
					&& (m_levels[index].turns[j].targets[k].first.substr(0, 4) != "HEAL" 
					&& round == curr_round)) {

					total += m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier);

				}
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
			if (m_levels[i].turns[j].isPlayer && m_levels[i].turns[j].actionType == Action::Attack)
			{
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
				{
					if ((m_levels[i].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.substr(0, 4) != "HEAL")) {
						total += (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)) - (m_levels[i].turns[j].targets[k].second * (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)));
					}
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
			if (name == m_levels[i].turns[j].characterName && m_levels[i].turns[j].actionType == Action::Attack)
			{
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
				{
					if ((m_levels[i].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.substr(0, 4) != "HEAL")) {
						total += m_levels[i].turns[j].value - (m_levels[i].turns[j].targets[k].second * m_levels[i].turns[j].value);
					}
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDone(std::string name, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (name == m_levels[index].turns[j].characterName && m_levels[index].turns[j].actionType == Action::Attack)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
			{
				if ((m_levels[index].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.substr(0, 4) != "HEAL")) {
					total += m_levels[index].turns[j].value - (m_levels[index].turns[j].targets[k].second * m_levels[index].turns[j].value);
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
			if (!m_levels[i].turns[j].isPlayer) {
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
				{
					if (m_levels[i].turns[j].actionType == Action::Attack)
					{
						total += (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)) - (m_levels[i].turns[j].targets[k].second * (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)));
					}
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
			if (m_levels[i].turns[j].actionType == Action::Attack) {
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
				{
					if (name == m_levels[i].turns[j].targets[k].first)
					{
						total += (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)) - (m_levels[i].turns[j].targets[k].second * (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)));
					}
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageTaken(std::string name, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (m_levels[index].turns[j].actionType == Action::Attack) {
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
			{
				if (name == m_levels[index].turns[j].targets[k].first)
				{
					total += (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)) - (m_levels[index].turns[j].targets[k].second * (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)));
				}
			}
		}
	}
	
	return total;
}

float StatTracker::CalculatePercentDamageSuccess() 
{
	float total = 0.0f;
	float totalDmg = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].isPlayer && m_levels[i].turns[j].actionType == Action::Attack) {
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
				{
					totalDmg += m_levels[i].turns[j].value;
					total += m_levels[i].turns[j].value - (m_levels[i].turns[j].targets[k].second * m_levels[i].turns[j].value);
				}
			}
		}
	}
	if (totalDmg == 0.0f) 
	{
		return 0.0f;
	}
	return (total/totalDmg)*100.0f;
}

float StatTracker::CalculatePercentDamageSuccess(std::string name, unsigned int level)
{
	float total = 0.0f;
	float totalDmg = 0.0f;
	unsigned int index = level - 1;
	/*for (unsigned int i = 0; i < m_levels.size(); i++)
	{*/
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if ( m_levels[index].turns[j].actionType == Action::Attack && m_levels[index].turns[j].characterName == name)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
			{
				totalDmg += m_levels[index].turns[j].value;
				total += m_levels[index].turns[j].value - (m_levels[index].turns[j].targets[k].second * m_levels[index].turns[j].value);
			}
		}
	
	}
	//}
	if (totalDmg == 0.0f)
	{
		return 0.0f;
	}
	return (total / totalDmg) * 100.0f;
}

float StatTracker::CalculateDamageMitigatated() 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (!m_levels[i].turns[j].isPlayer) {
				for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
				{
					if (m_levels[i].turns[j].actionType == Action::Attack)
					{
						total += m_levels[i].turns[j].targets[k].second * m_levels[i].turns[j].value;
					}
				}
			}
		}
	}
	return total;

}

float StatTracker::CalculateDamageMitigatated(std::string name, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	/*for (unsigned int i = 0; i < m_levels.size(); i++)
	{*/
		for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
		{
			if (!m_levels[index].turns[j].isPlayer) 
			{
				for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
				{
					if ((m_levels[index].turns[j].targets[k].first == name) && (m_levels[index].turns[j].actionType == Action::Attack))
					{
						total += m_levels[index].turns[j].targets[k].second * m_levels[index].turns[j].value;
					}
				}
			}
		}
	//}
	return total;

}

float StatTracker::CalculateHealthRecived() 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++) {
				if ((m_levels[i].turns[j].targets[k].first.substr(0, 4) == "HEAL") && m_levels[i].turns[j].isPlayer && !(m_levels[i].turns[j].effect == EFFECTTYPE::Shield))
				{
					total += m_levels[i].turns[j].value * m_levels[i].turns[j].targets.size();
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateHealthRecived(std::string name)
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++) {
				if ((m_levels[i].turns[j].targets[k].first.substr(0, 4) == "HEAL") && (m_levels[i].turns[j].targets[k].first.substr(4, name.size()) == name) && !(m_levels[i].turns[j].effect == EFFECTTYPE::Shield))
				{
					total += m_levels[i].turns[j].targets[k].second;
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateHealthRecived(std::string name, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
		for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
				if ((m_levels[index].turns[j].targets[k].first.substr(0, 4) == "HEAL") && (m_levels[index].turns[j].targets[k].first.substr(4, name.size()) == name) && !(m_levels[index].turns[j].effect == EFFECTTYPE::Shield))
				{
					total += m_levels[index].turns[j].targets[k].second;
				}
			}
		}
	
	return total;
}

float StatTracker::CalculateShieldGiven() 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].actionType == Action::Aid && m_levels[i].turns[j].isPlayer && m_levels[i].turns[j].effect == EFFECTTYPE::Shield)
			{
				total += m_levels[i].turns[j].value * m_levels[i].turns[j].targets.size();
			}
		}
	}
	return total;
}

float StatTracker::CalculateShieldGiven(std::string name) 
{
	float total = 0.0f;
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++) {
				if (m_levels[i].turns[j].actionType == Action::Aid && m_levels[i].turns[j].targets[k].first == name && m_levels[i].turns[j].effect == EFFECTTYPE::Shield)
				{
					total += m_levels[i].turns[j].value * m_levels[i].turns[j].targets.size();
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateShieldGiven(std::string name, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;

	/*for (unsigned int i = 0; i < m_levels.size(); i++)
	{*/
		for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
				if (m_levels[index].turns[j].actionType == Action::Aid && m_levels[index].turns[j].targets[k].first == name && m_levels[index].turns[j].effect == EFFECTTYPE::Shield)
				{
					total += m_levels[index].turns[j].value * m_levels[index].turns[j].targets.size();
				}
			}
		}
	//}
	return total;
}

float StatTracker::CalculatePercentageStat(Action stat)
{
	float toatalTurns = 0.0f;
	float totalAttacks = 0.0f;
	unsigned int curr_round = 0;

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].isPlayer) 
			{
				toatalTurns++;
				if (m_levels[i].turns[j].actionType == stat)
				{
					totalAttacks++;
				}
			}
		}
	}

	if (toatalTurns == 0.0f) {
		return 0.00f;
	}

	return (totalAttacks / toatalTurns)*100.0f;
}

float StatTracker::CalculatePercentageStat(Action stat, unsigned int level, unsigned int round)
{
	float toatalTurns = 0.0f;
	float totalAttacks = 0.0f;
	unsigned int curr_round = 0;

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].isPlayer)
			{
				if (curr_round != m_levels[i].turns[j].round)
				{
					curr_round = m_levels[i].turns[j].round;
					if (round < curr_round)
					{
						break;
					}
				}
				toatalTurns++;
				if (m_levels[i].turns[j].actionType == stat)
				{
					totalAttacks++;
				}
			}
		}
		if (round < curr_round)
		{
			break;
		}
	}

	if (toatalTurns == 0.0f) {
		return 0.00f;
	}

	return (totalAttacks / toatalTurns) * 100.0f;
}

float StatTracker::CalculatePercentageStat(std::string name, Action stat)
{

	float toatalTurns = 0.0f;
	float totalStat = 0.0f;

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].characterName == name) 
			{
				toatalTurns++;
				if (m_levels[i].turns[j].actionType == stat)
				{
					totalStat++;
				}
			}
		}
	}
	if (toatalTurns == 0.0f) {
		return 0.00f;
	}
	return (totalStat / toatalTurns)*100.0f;
}

float StatTracker::CalculatePercentageStat(std::string name, unsigned int round, Action stat)
{

	float toatalTurns = 0.0f;
	float totalStat = 0.0f;
	unsigned int curr_round = 0;

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].characterName == name)
			{
				toatalTurns++;
				if (curr_round != m_levels[i].turns[j].round)
				{
					curr_round = m_levels[i].turns[j].round;
					if (round < curr_round)
					{
						break;
					}
				}
				if (m_levels[i].turns[j].actionType == stat && m_levels[i].turns[j].round)
				{
					totalStat++;
				}
			}
		}
		if (round < curr_round)
		{
			break;
		}
	}
	if (toatalTurns == 0.0f) {
		return 0.00f;
	}
	return (totalStat / toatalTurns) * 100.0f;
}

float StatTracker::CalculatePercentageStat(std::string name, Action stat, unsigned int level)
{
	float toatalTurns = 0.0f;
	float totalStat = 0.0f;
	unsigned int index = level - 1;

	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (m_levels[index].turns[j].characterName == name)
		{
			toatalTurns++;
			if (m_levels[index].turns[j].actionType == stat)
			{
				totalStat++;
			}
		}
	}

	if (toatalTurns == 0.0f) {
		return 0.00f;
	}
	return (totalStat / toatalTurns) * 100.0f;
}

std::string StatTracker::FormatToPercentage(float number)
{
	if (number >= 100.0f)
	{
		return std::to_string(number).substr(0, 6);
	}
	else if (number >= 10.0f)
	{
		return std::to_string(number).substr(0, 5);
	}
	else
	{
		return std::to_string(number).substr(0, 4);
	}
}

std::wstring StatTracker::FormatToPercentageW(float number)
{
	if (number >= 100.0f)
	{
		return std::to_wstring(number).substr(0, 6);
	}
	else if (number >= 10.0f)
	{
		return std::to_wstring(number).substr(0, 5);
	}
	else
	{
		return std::to_wstring(number).substr(0, 4);
	}
}

float StatTracker::roundf(float num, unsigned int decimal_places) {
	return static_cast<float>(ceilf(num * std::powf(10.0f, static_cast<float>(decimal_places)))/std::powf(10.0f, static_cast<float>(decimal_places)));
}

double StatTracker::round(double num, unsigned int decimal_places) {
	return static_cast<double>(ceil(num * std::pow(10.0, decimal_places)) / std::pow(10.0, decimal_places));
}

void StatTracker::OutputStatSheet()
{
	time_t currTime;
	struct tm timeinfo;
	char fileName[39];

	time(&currTime);
	localtime_s(&timeinfo, &currTime);

	strftime(fileName, 39, "stat sheets/stat_log_%d_%m_%y_%H%M.txt", &timeinfo);
	std::fstream file(fileName, std::ios::out);
	bool fileOpen = file.is_open();
	if (fileOpen)
	{
		std::string fileText = "----------------------------------------------------------\n                        Game Info\n----------------------------------------------------------\n\n                  Attack     Defend      Aid\n               | ";

		for (int i = 0; i < 3; i++) {
			float percentage = roundf(CalculatePercentageStat((Action)i), 2);

			if (percentage < 10.0f) {
				fileText.append("0");
			}

			fileText.append(FormatToPercentage(percentage));

			if (i < 2) {
				fileText.append("% | | ");
			}
			else {
				fileText.append("% |\n\n");
			}
		}
		fileText.append("         Attack   Damage Dealt   Damage Success\nAttack: |  ");

		unsigned int attackCount = GetStatCount(Action::Attack);
		if (attackCount < 10.0f) {
			fileText.append("0");
		}
		fileText.append(std::to_string(attackCount) + "  | |    ");

		float dmgDelt = CalculateDamageDealt();
		if (dmgDelt < 10.0f) {
			fileText.append("0");
		}
			
		fileText.append(FormatToPercentage(dmgDelt) + "   | |    ");

		float dmgScs = CalculatePercentDamageSuccess();
		if (dmgScs < 10.0f) {
			fileText.append("0");
		}

		fileText.append(FormatToPercentage(dmgScs) + "%    |\n\n         Defend   Damage Taken   Damage Mitigated   Health Gained\nDefend: |  ");

		unsigned int defendCount = GetStatCount(Action::Defend);
		if (defendCount < 10.0f) {
			fileText.append("0");
		}
		fileText.append(std::to_string(defendCount) + "  | |    ");

		float dmgTkn = CalculateDamageTaken();
		if (dmgTkn < 10.0f) {
			fileText.append("0");
		}

		fileText.append(FormatToPercentage(dmgTkn) + "   | |     ");


		float dmgMit = CalculateDamageMitigatated();
		if (dmgMit < 10.0f) {
			fileText.append("0");
		}

		fileText.append(FormatToPercentage(dmgMit) + "      | |    ");
		

		float hlthGand = CalculateHealthRecived();
		if (hlthGand < 10.0f) {
			fileText.append("0");
		}

		fileText.append(FormatToPercentage(hlthGand) + "    |\n\n          Aid     Heal    Deffence Buff\n   Aid: | ");
		

		unsigned int aidCount = GetStatCount(Action::Aid);
		if (aidCount < 10.0f) {
			fileText.append("0");
		}
		fileText.append(std::to_string(aidCount) + " | | ");

		float heal = CalculateHealthRecived();
		if (heal < 10.0f) {
			fileText.append("0");
		}

		fileText.append(FormatToPercentage(heal) + " | |    ");
		

		float deffBuff = CalculateShieldGiven();
		if (deffBuff < 10.0f) {
			fileText.append("0");
		}

		fileText.append(FormatToPercentage(deffBuff) + "    |");
		

		unsigned int current_round = 0;
		for (int i = 0; i < m_levels.size(); i++) {
			fileText.append("\n\n----------------------------------------------------------\n                         Level " + std::to_string(i + 1) + "\n----------------------------------------------------------\n");
			for (int j = 0; j < m_levels[i].turns.size(); j++) {
				if (current_round != m_levels[i].turns[j].round) {
					current_round = m_levels[i].turns[j].round;
					fileText.append("\nRound " + std::to_string(current_round) + "\n\n");
				}
				fileText.append("\tTurn " + std::to_string(j + 1) + ": " + m_levels[i].turns[j].characterName + "\n\n\tAction: " + m_levels[i].turns[j].actionName + "\n\n");
				for (int k = 0; k < m_levels[i].turns[j].targets.size(); k++) {
					if ((m_levels[i].turns[j].targets[k].first.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.substr(0, 6) != "HEAL")) {
						if (k == 0) {
							fileText.append("\tTarget: " + m_levels[i].turns[j].targets[k].first + "\n");
							continue;
						}
						fileText.append("\t        " + m_levels[i].turns[j].targets[k].first + "\n");
					}
				}

				fileText.append("\n");

				switch (m_levels[i].turns[j].actionType)
				{
					case StatTracker::Action::Attack: 
					{
						fileText.append("\t         Damage: " + FormatToPercentage(m_levels[i].turns[j].value) /*+ "\n\tAttack Modifier: " + FormatToPercentage(m_levels[i].turns[j].attackModifier)*/ + "\n\n");
						for (int l = 0; l < m_levels[i].turns[j].targets.size(); l++) {
							if ((m_levels[i].turns[j].targets[l].first.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[l].first.substr(0, 6) != "HEAL")) {
								fileText.append("\tTarget " + std::to_string(l + 1) + ": " + m_levels[i].turns[j].targets[l].first + "\n\tDamage Mitigated: " + FormatToPercentage(roundf(m_levels[i].turns[j].targets[l].second, 2)) + "\n\t    Danage Taken: " + FormatToPercentage((m_levels[i].turns[j].value - (m_levels[i].turns[j].value * m_levels[i].turns[j].targets[l].second))) + "\n\n");
							}
						}
					}
						break;
					case StatTracker::Action::Defend:
					{
						if (m_levels[i].turns[j].effect == EFFECTTYPE::Shield)
						{
							fileText.append("\tSheild: ");
						}
						else
						{
							fileText.append("\t  Heal: ");
						}
						fileText.append(FormatToPercentage(m_levels[i].turns[j].value));
					}
						break;
					case StatTracker::Action::Aid:
					{
						if (m_levels[i].turns[j].effect == EFFECTTYPE::Shield)
						{
							fileText.append("\tSheild: ");
						}
						else
						{
							fileText.append("\t  Heal: ");
						}
						fileText.append(FormatToPercentage(m_levels[i].turns[j].value));
					}
						break;
					default:
						break;
				}
				fileText.append("\n");
			}
		}
		file << fileText;

		#ifdef DEBUG_STAT_TRACK
			std::cout << fileText << std::endl;
		#endif // DEBUG_STAT_TRACK
	}
	file.close();
}