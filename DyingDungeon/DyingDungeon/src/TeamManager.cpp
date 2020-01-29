#include "TeamManager.h"

TeamManager& TeamManager::getInstance()
{
	static TeamManager instance;
	return instance;
}

void TeamManager::AddCharacterToPlayerTeam(std::shared_ptr<Odyssey::Entity> _characterToAdd)
{
	mPlayerTeam.push_back(_characterToAdd);
}

void TeamManager::AddCharacterToEnemyTeam(std::shared_ptr<Odyssey::Entity> _characterToAdd)
{
	mEnemyTeam.push_back(_characterToAdd);
}
