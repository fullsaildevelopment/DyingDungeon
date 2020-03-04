#include "StatTracker.h"
#include "EnemyComponent.h"
#include "HeroComponent.h"

#define DEBUG_STAT_TRACK
StatTracker::StatTracker()
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_currentLevel = 1;
	m_maxPlayerCount = 3;
	m_p_canvas = nullptr;
	m_portrait_width = 500;
	m_portrait_height = 500;

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

void StatTracker::SetCanvas(Odyssey::UICanvas* canvas, unsigned int path_width, unsigned int path_height)
{
	m_p_canvas = canvas;
	m_portrait_width = path_width;
	m_portrait_height = path_height;
}

void StatTracker::UpdateRewardScreen(RewardsActiveEvent* raEvent) 
{
	//std::vector<std::string> temp_nameList = GetListPlayerCharacterNames(raEvent->level);
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
	/*for (int txt = 3, i = 0; i < 3; i++) 
	{
		m_p_canvas->getElements<Odyssey::Sprite2D>()[i]->setSprite(m_levels.back().characters[i].second, m_portrait_width, m_portrait_height);
		for (int j = 0; j < 3; j++)
		{
			std::wstring rewardsText;
			if (txt < 6)
			{
				rewardsText.append(L"Attack: " + FormatToPercentageW(CalculatePercentageStat(m_levels.back().characters[j].first.second, Action::Attack, static_cast<unsigned int>(m_levels.size())),2) + L"%\n" +
								   L"Damage Dealt: "+ FormatToPercentageW(CalculateDamageDealt(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size()), true),2) + L"\n" +
								   L"Damage Success: " + FormatToPercentageW(CalculatePercentDamageSuccess(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size())),2) + L"%");
			}
			else if (txt < 9)
			{
				rewardsText.append(L"Defend: " + FormatToPercentageW(CalculatePercentageStat(m_levels.back().characters[j].first.second, Action::Defend, static_cast<unsigned int>(m_levels.size())),2) + L"%\n" +
								   L"Damage Taken: " + FormatToPercentageW(CalculateDamageTaken(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size())),2) + L"\n" +
								   L"Damage Blocked: " + FormatToPercentageW(CalculateDamageMitigatated(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size())),2) + L"%\n" +
								   L"Health Gained: " + FormatToPercentageW(CalculateHealthRecived(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size())),2));
			}
			else if (txt < 12)
			{
				rewardsText.append(L"Aid: " + FormatToPercentageW(CalculatePercentageStat(m_levels.back().characters[j].first.second, Action::Aid, static_cast<unsigned int>(m_levels.size())),2) + L"%\n" +
								   L"Heal: " + FormatToPercentageW(CalculateHealthRecived(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size())),2) + L"\n" +
								   L"Defense Buff: " + FormatToPercentageW(CalculateShieldGiven(m_levels.back().characters[j].first.second, static_cast<unsigned int>(m_levels.size())),2));
			}
			m_p_canvas->getElements<Odyssey::Text2D>()[txt]->setText(rewardsText);
			txt++;
		}

	}*/
	for (int i = 0; i < 3; i++) {
		m_p_canvas->getElements<Odyssey::Sprite2D>()[i]->setSprite(m_levels.back().characters[i].second, m_portrait_width, m_portrait_height);

		std::wstring rewardsText;

		rewardsText.append(std::to_wstring(GetStatCount(m_levels.back().characters[i].first.unique_id, raEvent->level, Action::Attack)) + L" time(s)");
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1)*10)]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculateDamageDealt(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size()), true), 2));
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 1]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculatePercentDamageSuccess(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size())), 2) + L"%");
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 2]->setText(rewardsText);
		rewardsText.clear();
		
		rewardsText.append(std::to_wstring(GetStatCount(m_levels.back().characters[i].first.unique_id, raEvent->level, Action::Defend)) + L" time(s)");
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 3]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculateDamageTaken(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size())), 2));
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 4]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculateDamageMitigatated(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size())), 2) + L"%\n");
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 5]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculateHealthRecived(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size())), 2));
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 6]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(std::to_wstring(GetStatCount(m_levels.back().characters[i].first.unique_id, raEvent->level, Action::Aid)) + L" time(s)");
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 7]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculateHealthRecived(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size())), 2));
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 8]->setText(rewardsText);
		rewardsText.clear();

		rewardsText.append(FormatToPercentageW(CalculateShieldGiven(m_levels.back().characters[i].first.unique_id, static_cast<unsigned int>(m_levels.size())), 2));
		m_p_canvas->getElements<Odyssey::Text2D>()[((i + 1) * 10) + 9]->setText(rewardsText);
		rewardsText.clear();
	}
	//OutputStatSheet();
}

void StatTracker::LogDamageDeltEvent(CharacterDealtDamageEvent* cddEvent)
{
	CHARACTER_STAT newCharacterStat;
	m_levels.back().turns.back().actionName = cddEvent->actionName;
	m_levels.back().turns.back().actionType = Action::Attack;
	m_levels.back().turns.back().effect = cddEvent->actionEffect;
	m_levels.back().turns.back().value = cddEvent->damageAmount;
	m_levels.back().turns.back().attackModifier = cddEvent->atkMod;

}

void StatTracker::LogTakeDamageEvent(CharacterTakeDamage* ctdEvent)
{
	//m_levels.back().turns.back().targetNames.push_back(ctdEvent->targetName);
	//m_levels.back().turns.back().blockValues.push_back(ctdEvent->mitigationAmount);
	CHARACTER_STAT newCharacterStat;
	if (ctdEvent->targetPointer->IsHero()) {
		newCharacterStat.characterName = ctdEvent->targetName;
		newCharacterStat.unique_id = GetUniqueID(ctdEvent->targetPointer);
	}
	else
	{
		newCharacterStat.characterName = ctdEvent->targetName;
		newCharacterStat.unique_id = (666 * 100) + static_cast<int>(static_cast<EnemyComponent*>(ctdEvent->targetPointer)->GetID());
	}
	m_levels.back().turns.back().targets.push_back(std::make_pair(newCharacterStat, ctdEvent->mitigationAmount));
}

unsigned int StatTracker::GetUniqueID(Character* character) 
{
	//srand(time(NULL));
	int i = 0;
	for (i = 0; i < 3; i++) {
		if (!m_levels.empty() && character == m_levels.back().character_pointers[i])
		{
			return m_levels.back().characters[i].first.unique_id;
		}
		else if (m_levels.empty())
		{
			break;
		}
	}
	return (rand() % (9999999 - 1000000 + 1) + 1000000);
}

void StatTracker::LogHealingEvent(CharacterHealsEvent* chcEvent)
{
	m_levels.back().turns.back().actionName = chcEvent->actionName;
	m_levels.back().turns.back().actionType = Action::Aid;
	m_levels.back().turns.back().value = chcEvent->health;

}

void StatTracker::LogReciveHealingEvent(CharacterRecivesHealingEvent* crhEvent) 
{
	CHARACTER_STAT newCharacterStat;
	if (crhEvent->targetPointer->IsHero()) {
		newCharacterStat.characterName = std::string("HEAL" + crhEvent->targetName);
		newCharacterStat.unique_id = GetUniqueID(crhEvent->targetPointer);
	}
	else
	{
		newCharacterStat.characterName = std::string("HEAL" + crhEvent->targetName); 
		newCharacterStat.unique_id = (666 * 100) + static_cast<int>(static_cast<EnemyComponent*>(crhEvent->targetPointer)->GetID());
	}

	m_levels.back().turns.back().buffedTargets.push_back(std::make_pair(newCharacterStat, crhEvent->healingAmount));
	
	if (m_levels.back().turns.back().unique_id == newCharacterStat.unique_id &&
		m_levels.back().turns.back().actionType != Action::Attack) 
	{
		m_levels.back().turns.back().actionType = Action::Defend;
	}
}

void StatTracker::LogBuffingEvent(CharacterBuffsEvent* cbEvent) 
{
	m_levels.back().turns.back().actionName = cbEvent->actionName;
	m_levels.back().turns.back().actionType = Action::Aid;
	m_levels.back().turns.back().value = cbEvent->buffValue;
	//m_levels.back().turns.back().isSheild = true;
	m_levels.back().turns.back().effect = cbEvent->buffType;
	CHARACTER_STAT newCharacterStat;
	if (cbEvent->targetPointer->IsHero()) {
		newCharacterStat.characterName = cbEvent->targetName;
		newCharacterStat.unique_id = GetUniqueID(cbEvent->targetPointer);
	}
	else
	{
		newCharacterStat.characterName = cbEvent->targetName;
		newCharacterStat.unique_id = (666*100) + static_cast<int>(static_cast<EnemyComponent*>(cbEvent->targetPointer)->GetID());
	}
	if (m_levels.back().turns.back().unique_id == newCharacterStat.unique_id &&
		m_levels.back().turns.back().targets.size() <= 1 &&
		m_levels.back().turns.back().actionType != Action::Attack)
	{
		m_levels.back().turns.back().actionType = Action::Defend;
	}
	m_levels.back().turns.back().buffedTargets.push_back(std::make_pair(newCharacterStat, cbEvent->buffValue));
}

void StatTracker::LogDebuffingEvent(CharacterDebuffsEvent* cdEvent)
{
	m_levels.back().turns.back().actionName = cdEvent->actionName;
	m_levels.back().turns.back().actionType = Action::Attack;
	//m_levels.back().turns.back().value = cdEvent->debuffValue;
	//m_levels.back().turns.back().isSheild = true;
	m_levels.back().turns.back().effect = cdEvent->debuffType;
	CHARACTER_STAT newCharacterStat;
	if (cdEvent->targetPointer->IsHero()) {
		newCharacterStat.characterName = std::string("DEBUFF" + cdEvent->targetName);
		newCharacterStat.unique_id = GetUniqueID(cdEvent->targetPointer);
		newCharacterStat.characterClass = static_cast<HeroComponent*>(cdEvent->targetPointer)->GetID();
	}
	else
	{
		newCharacterStat.characterName = std::string("DEBUFF" + cdEvent->targetName);
		newCharacterStat.unique_id = (666*100) + static_cast<int>(static_cast<EnemyComponent*>(cdEvent->targetPointer)->GetID());
		//newCharacterStat.characterClass = static_cast<EnemyComponent*>(cdEvent->targetPointer)->GetID();
	}
	m_levels.back().turns.back().debuffedTargets.push_back(std::make_pair(newCharacterStat, cdEvent->debuffValue));

	/*if (m_levels.back().turns.back().characterName == cdEvent->targetName &&
		m_levels.back().turns.back().targetNames.size() <= 1 &&
		m_levels.back().turns.back().actionType != Action::Attack)
	{
		m_levels.back().turns.back().actionType = Action::Defend;
	}*/
}

void StatTracker::LevelStartReflex(LevelStartEvent* lsEvent)
{
	//srand(time(NULL));
	StatTracker::Level newLevel;
	newLevel.levelNumber = lsEvent->levelNumber;
	for(int i = 0; i < 3; i++)
	{
		newLevel.characters[i].first.characterName = lsEvent->playerCharacters[i];
		newLevel.characters[i].second = lsEvent->playerPortaits[i];
		newLevel.character_pointers[i] = lsEvent->playerPointers[i];
		newLevel.characters[i].first.characterClass = static_cast<HeroComponent*>(lsEvent->playerPointers[i])->GetID();
		/*if (m_levels.empty() || newLevel.character_pointers[i] != m_levels.back().character_pointers[i])
		{
			newLevel.characters[i].first.second = (rand() % (9999999 - 1000000 + 1) + 1000000);
		}
		else
		{
			newLevel.characters[i].first.second = m_levels.back().characters[i].first.second;
		}*/
		newLevel.characters[i].first.unique_id = GetUniqueID(newLevel.character_pointers[i]);
	}

	m_levels.push_back(newLevel);
	m_currentLevel = newLevel.levelNumber;
}

void StatTracker::TurnStartReflex(TurnStartEvent* tsEvent)
{
	StatTracker::Turn newTurn;
	/*for (int i = 0; i < 3; i++)
	{
		if (m_levels.back().character_pointers[i] == tsEvent->playerPointer)
		{*/
			newTurn.unique_id = GetUniqueID(tsEvent->playerPointer);
	/*		break;
		}
	}*/
	m_levels.back().turnCount = tsEvent->turn;
	newTurn.characterName = tsEvent->characterName;
	newTurn.round = tsEvent->round;
	newTurn.isPlayer = tsEvent->isPlayer;
	m_levels.back().turns.push_back(newTurn);
	m_levels.back().turnCount = static_cast<uint32_t>(m_levels.back().turns.size());
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

unsigned int StatTracker::GetStatCount(unsigned int id, Action stat)
{
	unsigned int count = 0;

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
		{
			if (m_levels[i].turns[j].unique_id == id && m_levels[i].turns[j].actionType == stat)
			{
				count++;
			}
		}
	}

	return count;
}

unsigned int StatTracker::GetStatCount(unsigned int id, unsigned int level, Action stat)
{
	unsigned int count = 0;
	unsigned int index = level - 1;
	/*for (unsigned int i = 0; i < m_levels.size(); i++)
	{*/
		for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
		{
			if (m_levels[index].turns[j].unique_id == id && m_levels[index].turns[j].actionType == stat)
			{
				count++;
			}
		}
	//}

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
					if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") || (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
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
					if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") || (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
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
				if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
					total += m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier);
				}
			}
		}
	}
	return total;
}

float StatTracker::CalculateDamageDealt(unsigned int id, unsigned int level, bool a)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (id == m_levels[index].turns[j].unique_id && m_levels[index].turns[j].actionType == Action::Attack)
		{
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
				if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
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
				if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF")
					&& (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL"
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
					if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
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
					if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
						total += m_levels[i].turns[j].value - (m_levels[i].turns[j].targets[k].second * m_levels[i].turns[j].value);
					}
				}
			}
		}
	}
	return 0.0f;
}

float StatTracker::CalculateDamageDone(std::string name = "", unsigned int level = 0, unsigned int round = 0)
{
	float total = 0.0f;
	if (level <= 0) {
		for (unsigned int i = 0; i < m_levels.size(); i++)
		{
			if (name == "")
			{
				for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
				{
					if (name == m_levels[i].turns[j].characterName && m_levels[i].turns[j].actionType == Action::Attack)
					{
						for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
						{
							if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
								total += m_levels[i].turns[j].value - (m_levels[i].turns[j].targets[k].second * m_levels[i].turns[j].value);
							}
						}
					}
				}
			}
			else
			{
				for (unsigned int j = 0; j < m_levels[i].turns.size(); j++)
				{
					if (m_levels[i].turns[j].isPlayer && m_levels[i].turns[j].actionType == Action::Attack)
					{
						for (unsigned int k = 0; k < m_levels[i].turns[j].targets.size(); k++)
						{
							if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
								total += (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)) - (m_levels[i].turns[j].targets[k].second * (m_levels[i].turns[j].value + (m_levels[i].turns[j].value * m_levels[i].turns[j].attackModifier)));
							}
						}
					}
				}
			}
		}
	}
	else if (level > 0)
	{
		unsigned int index = level - 1;
		if (name != "" && round > 0)
		{
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
				if (name == m_levels[index].turns[j].characterName && m_levels[index].turns[j].actionType == Action::Attack)
				{
					for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
					{
						if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL") && round == curr_round) {
							total += m_levels[index].turns[j].value - (m_levels[index].turns[j].targets[k].second * m_levels[index].turns[j].value);
						}
					}
				}
			}
		}
		else if (name != "")
		{
			for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
			{
				if (name == m_levels[index].turns[j].characterName && m_levels[index].turns[j].actionType == Action::Attack)
				{
					for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
					{
						if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
							total += m_levels[index].turns[j].value - (m_levels[index].turns[j].targets[k].second * m_levels[index].turns[j].value);
						}
					}
				}
			}
		}
		else if (round > 0)
		{
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
				if (m_levels[index].turns[j].isPlayer && m_levels[index].turns[j].actionType == Action::Attack && round == curr_round)
				{
					for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
					{
						if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
							total += (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)) - (m_levels[index].turns[j].targets[k].second * (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)));
						}
					}
				}
			}
		}
		else
		{
			for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
			{
				if (m_levels[index].turns[j].isPlayer && m_levels[index].turns[j].actionType == Action::Attack)
				{
					for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
					{
						if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
							total += (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)) - (m_levels[index].turns[j].targets[k].second * (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)));
						}
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
				if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) != "HEAL")) {
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
					if (name == m_levels[i].turns[j].targets[k].first.characterName)
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
				if (name == m_levels[index].turns[j].targets[k].first.characterName)
				{
					total += (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)) - (m_levels[index].turns[j].targets[k].second * (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier)));
				}
			}
		}
	}
	
	return total;
}

float StatTracker::CalculateDamageTaken(unsigned int id, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (m_levels[index].turns[j].actionType == Action::Attack) {
			for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++)
			{
				if (id == m_levels[index].turns[j].targets[k].first.unique_id && m_levels[index].turns[j].targets[k].first.characterName.substr(0,4) != "HEAL" && m_levels[index].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF")
				{
					float dmg = (m_levels[index].turns[j].value + (m_levels[index].turns[j].value * m_levels[index].turns[j].attackModifier));
					total += dmg - (m_levels[index].turns[j].targets[k].second * dmg);
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

float StatTracker::CalculatePercentDamageSuccess(unsigned int id, unsigned int level)
{
	float total = 0.0f;
	float totalDmg = 0.0f;
	unsigned int index = level - 1;
	/*for (unsigned int i = 0; i < m_levels.size(); i++)
	{*/
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (m_levels[index].turns[j].actionType == Action::Attack && m_levels[index].turns[j].unique_id == id)
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
					if ((m_levels[index].turns[j].targets[k].first.characterName == name) && (m_levels[index].turns[j].actionType == Action::Attack))
					{
						total += m_levels[index].turns[j].targets[k].second * m_levels[index].turns[j].value;
					}
				}
			}
		}
	//}
	return total;

}

float StatTracker::CalculateDamageMitigatated(unsigned int id, unsigned int level)
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
				if ((m_levels[index].turns[j].targets[k].first.unique_id == id) && (m_levels[index].turns[j].actionType == Action::Attack))
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
				if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) == "HEAL") && m_levels[i].turns[j].isPlayer && !(m_levels[i].turns[j].effect == EFFECTTYPE::Shield))
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
				if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 4) == "HEAL") && (m_levels[i].turns[j].targets[k].first.characterName.substr(4, name.size()) == name) && !(m_levels[i].turns[j].effect == EFFECTTYPE::Shield))
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
				if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) == "HEAL") && (m_levels[index].turns[j].targets[k].first.characterName.substr(4, name.size()) == name) && !(m_levels[index].turns[j].effect == EFFECTTYPE::Shield))
				{
					total += m_levels[index].turns[j].targets[k].second;
				}
			}
		}
	
	return total;
}

float StatTracker::CalculateHealthRecived(unsigned int id, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
			if ((m_levels[index].turns[j].targets[k].first.characterName.substr(0, 4) == "HEAL") && (m_levels[index].turns[j].targets[k].first.unique_id == id) && !(m_levels[index].turns[j].effect == EFFECTTYPE::Shield))
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
				if (m_levels[i].turns[j].actionType == Action::Aid && m_levels[i].turns[j].targets[k].first.characterName == name && m_levels[i].turns[j].effect == EFFECTTYPE::Shield)
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
				if (m_levels[index].turns[j].actionType == Action::Aid && m_levels[index].turns[j].targets[k].first.characterName == name && m_levels[index].turns[j].effect == EFFECTTYPE::Shield)
				{
					total += m_levels[index].turns[j].value * m_levels[index].turns[j].targets.size();
				}
			}
		}
	//}
	return total;
}

float StatTracker::CalculateShieldGiven(unsigned int id, unsigned int level)
{
	float total = 0.0f;
	unsigned int index = level - 1;

	/*for (unsigned int i = 0; i < m_levels.size(); i++)
	{*/
	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		for (unsigned int k = 0; k < m_levels[index].turns[j].targets.size(); k++) {
			if (m_levels[index].turns[j].actionType == Action::Aid && m_levels[index].turns[j].targets[k].first.unique_id == id && m_levels[index].turns[j].effect == EFFECTTYPE::Shield)
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

float StatTracker::CalculatePercentageStat(std::string name, Action stat, unsigned int level, unsigned int round)
{

	float toatalTurns = 0.0f;
	float totalStat = 0.0f;
	unsigned int curr_round = 0;
	unsigned int index = level - 1;

	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (m_levels[index].turns[j].characterName == name)
		{
			toatalTurns++;
			if (curr_round != m_levels[index].turns[j].round)
			{
				curr_round = m_levels[index].turns[j].round;
				if (round < curr_round)
				{
					break;
				}
			}
			if (m_levels[index].turns[j].actionType == stat && curr_round == round)
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

float StatTracker::CalculatePercentageStat(unsigned int id, Action stat, unsigned int level)
{
	float toatalTurns = 0.0f;
	float totalStat = 0.0f;
	unsigned int index = level - 1;

	for (unsigned int j = 0; j < m_levels[index].turns.size(); j++)
	{
		if (m_levels[index].turns[j].unique_id == id)
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

std::string StatTracker::GetCharacterName(unsigned int level, unsigned int turn) 
{
	return m_levels[level - 1].turns[turn - 1].characterName;
}

std::wstring StatTracker::GetCharacterPortrait(unsigned int level, unsigned int unique_id)
{
	for (int i = 0; i < 3; i++) {
		if (m_levels[level - 1].characters[i].first.unique_id == unique_id)
		{
			return m_levels[level - 1].characters[i].second;
		}
	}
	return L"assets/images/Dying_Dungeon_Logo_sharp.png";
}

std::vector<float> StatTracker::GetTargetDebuffBuffValues(unsigned int level, unsigned int turn)
{
	std::vector<float> _temp;
	for (int i = 0; i < m_levels[level - 1].turns[turn - 1].targets.size(); i++)
	{
		if (m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 6) == "DEBUFF" || m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 4) == "BUFF") {
			_temp.push_back(m_levels[level - 1].turns[turn - 1].targets[i].second);
		}
	}
	return _temp;
}

std::vector<float> StatTracker::GetTargetDebuffBuffValues(unsigned int level, unsigned int turn, unsigned int unique_id)
{
	std::vector<float> _temp;
	for (int i = 0; i < m_levels[level - 1].turns[turn - 1].targets.size(); i++)
	{
		if ((m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 6) == "DEBUFF" || m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 4) == "BUFF") && (m_levels[level - 1].turns[turn - 1].targets[i].first.unique_id == unique_id)) {
			_temp.push_back(m_levels[level - 1].turns[turn - 1].targets[i].second);
		}
	}
	return _temp;
}

float StatTracker::GetTargetDebuffBuffValue(unsigned int level, unsigned int turn, unsigned int unique_id)
{
	for (int i = 0; i < m_levels[level - 1].turns[turn - 1].targets.size(); i++)
	{
		if (m_levels[level - 1].turns[turn - 1].targets[i].first.unique_id == unique_id && (m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 4) == "BUFF") && (m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 6) == "DEBUFF")) 
		{
			return m_levels[level - 1].turns[turn - 1].targets[i].second;
		}
	}
	return 0.0f;
}

std::vector<float> StatTracker::GetTargetHeal(unsigned int level, unsigned int turn)
{
	std::vector<float> _temp;
	for (int i = 0; i < m_levels[level - 1].turns[turn - 1].targets.size(); i++)
	{
		if (m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 4) == "HEAL") {
			_temp.push_back(m_levels[level - 1].turns[turn - 1].targets[i].second);
		}
	}
	return _temp;
}

Odyssey::UICanvas* StatTracker::GetCanvas()
{
	return m_p_canvas;
}

std::vector<std::string> StatTracker::GetTargetList(unsigned int level, unsigned int turn)
{
	std::vector<std::string> temp_name;
	for (int i = 0; i < m_levels[level - 1].turns[turn - 1].targets.size(); i++)
	{
		if ((m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 4) != "HEAL") && (m_levels[level - 1].turns[turn - 1].targets[i].first.characterName.substr(0, 6) != "DEBUFF"))
		{
			temp_name.push_back(m_levels[level - 1].turns[turn - 1].targets[i].first.characterName);
		}
	}
	return temp_name;
}

unsigned int StatTracker::GetFirstTurn(unsigned int level, unsigned int round)
{
	unsigned int turn = 1;
	for (int i = 0; i < m_levels[level - 1].turns.size(); i++)
	{
		if (m_levels[level - 1].turns[i].round == round)
		{
			turn += i;
			break;
		}
	}
	return turn;
}

std::string StatTracker::FormatToPercentage(float number, unsigned int decimal_places)
{
	unsigned int count = 1 + decimal_places;
	for (float i = number; i > 0; i--, count++)
	{
		i /= 10.0f;
	}

	return std::to_string(number).substr(0, count);
}

std::wstring StatTracker::FormatToPercentageW(float number, unsigned int decimal_places)
{
	unsigned int count = 1 + decimal_places;
	for (float i = number; i > 0; i--, count++)
	{
		i /= 10.0f;
	}

	return std::to_wstring(number).substr(0, count);
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
					if ((m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[k].first.characterName.substr(0, 6) != "HEAL")) {
						if (k == 0) {
							fileText.append("\tTarget: " + m_levels[i].turns[j].targets[k].first.characterName + "\n");
							continue;
						}
						fileText.append("\t        " + m_levels[i].turns[j].targets[k].first.characterName + "\n");
					}
				}

				fileText.append("\n");

				switch (m_levels[i].turns[j].actionType)
				{
					case StatTracker::Action::Attack: 
					{
						fileText.append("\t         Damage: " + FormatToPercentage(m_levels[i].turns[j].value) /*+ "\n\tAttack Modifier: " + FormatToPercentage(m_levels[i].turns[j].attackModifier)*/ + "\n\n");
						for (int l = 0; l < m_levels[i].turns[j].targets.size(); l++) {
							if ((m_levels[i].turns[j].targets[l].first.characterName.substr(0, 6) != "DEBUFF") && (m_levels[i].turns[j].targets[l].first.characterName.substr(0, 6) != "HEAL")) {
								fileText.append("\tTarget " + std::to_string(l + 1) + ": " + m_levels[i].turns[j].targets[l].first.characterName + "\n\tDamage Mitigated: " + FormatToPercentage(roundf(m_levels[i].turns[j].targets[l].second, 2)) + "\n\t    Danage Taken: " + FormatToPercentage((m_levels[i].turns[j].value - (m_levels[i].turns[j].value * m_levels[i].turns[j].targets[l].second))) + "\n\n");
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