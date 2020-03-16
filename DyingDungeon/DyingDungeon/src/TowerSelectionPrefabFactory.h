#pragma once
#include "Entity.h"
#include "UICanvas.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Sprite2D.h"
#include "UIElement.h"
#include "Application.h"
#include "TeamManager.h"
#include <map>

class TowerSelectionPrefabFactory
{
public:

	// Singleton pattern
	/**
	 *	Get the singleton instance of the game ui manager.
	 *	@param[in] void
	 *	@return InputManager& The singleton instance of the game ui manager.
	 */
	static TowerSelectionPrefabFactory& getInstance();
	~TowerSelectionPrefabFactory() { }

private: // Singleton pattern
	TowerSelectionPrefabFactory() {}

public: // Functions
	
	// The initalize function to create all of the UI Prefabs
	void CreateTowerSelectionPrefab(Odyssey::Application* _application, std::vector<TeamManager::EnemySetups> _enemyEnums);

	// Get the info popup prefab
	Odyssey::Entity* GetInfoPrefab();

private:

	// Hold the prefab for the door when hovering over it
	Odyssey::Entity* mTowerSelectPopupPrefab;
};

