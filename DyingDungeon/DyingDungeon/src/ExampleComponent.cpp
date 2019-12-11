#include "ExampleComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"

CLASS_DEFINITION(Component, ExampleComponent)

void ExampleComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable(); 
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();
}

void ExampleComponent::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyDown('F'))
	{
		mGameObject->getComponent<Odyssey::Transform>()->addScale(-deltaTime, -deltaTime, -deltaTime);
	}

	if (Odyssey::InputManager::getInstance().getKeyDown('G'))
	{
		mGameObject->getComponent<Odyssey::Transform>()->addScale(deltaTime, deltaTime, deltaTime);
	}
}