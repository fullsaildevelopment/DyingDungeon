#include "CombatManager.h"

CombatManager& CombatManager::getInstance()
{
	static CombatManager instance;
	return instance;
}