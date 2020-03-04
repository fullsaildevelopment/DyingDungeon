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

	enum TowerSelectPopupPrefabs
	{
		Door1, 
		Door2,
		Door3,
		Door4,
		Door5,
	};

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

	// Get the info popup prefab map
	Odyssey::Entity* GetInfoPrefabs(TowerSelectPopupPrefabs _prefab);

private:

	// Hold the prefabs for each door when hovering over it
	std::map<TowerSelectPopupPrefabs, Odyssey::Entity*> mTowerSelectPopupPrefabs;
};

