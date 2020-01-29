#include "TeamManager.h"

TeamManager& TeamManager::getInstance()
{
	static TeamManager instance;
	return instance;
}