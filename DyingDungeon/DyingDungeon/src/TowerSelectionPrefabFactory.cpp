#include "TowerSelectionPrefabFactory.h"

TowerSelectionPrefabFactory& TowerSelectionPrefabFactory::getInstance()
{
	static TowerSelectionPrefabFactory instance;
	return instance;
}

void TowerSelectionPrefabFactory::CreateTowerSelectionPrefab(Odyssey::Application* _application)
{
	// Create the door hover prefab for the team selection
}
