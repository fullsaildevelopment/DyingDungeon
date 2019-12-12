#include "TowerManager.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, TowerManager)

void TowerManager::initialize(Odyssey::GameObject* _parent)
{
	onEnable();
	mGameObject = _parent;
	mGameObject->addComponent<Odyssey::Transform>();
}

void TowerManager::update(double deltaTime)
{
}