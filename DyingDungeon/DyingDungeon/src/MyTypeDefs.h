#pragma once
#include "Component.h"
#include <vector>
#include <queue>

typedef std::vector<std::shared_ptr<Odyssey::GameObject>> GameObjectList; //This will contain a list off shared pointer game objects

typedef std::queue<std::shared_ptr<Odyssey::GameObject>> GameObjectQueue; //This will contain a queue off shared pointer game objects