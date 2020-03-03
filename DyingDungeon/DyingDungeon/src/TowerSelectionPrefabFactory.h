#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Sprite2D.h"
#include "UIElement.h"
#include <map>

class TowerSelectionPrefabFactory
{
public: // Singleton pattern
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
	void CreateTowerSelectionPrefab(Odyssey::Application* _application);
};

