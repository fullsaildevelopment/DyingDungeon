#pragma once
#include "Component.h"
#include <vector>
#include <queue>

typedef std::vector<Odyssey::Entity*> EntityList; //This will contain a list off shared pointer game objects

typedef std::queue<Odyssey::Entity*> EntityQueue; //This will contain a queue off shared pointer game objects
