#pragma once
#include "Component.h"
#include <vector>
#include <queue>

typedef std::vector<std::shared_ptr<Odyssey::Entity>> EntityList; //This will contain a list off shared pointer game objects

typedef std::queue<std::shared_ptr<Odyssey::Entity>> EntityQueue; //This will contain a queue off shared pointer game objects
