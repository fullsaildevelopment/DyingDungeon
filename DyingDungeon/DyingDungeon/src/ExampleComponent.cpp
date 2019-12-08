#include "ExampleComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, ExampleComponent)

void ExampleComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable(); 
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();
}

void ExampleComponent::update(double deltaTime)
{
	if (GetAsyncKeyState('F'))
	{
		mGameObject->getComponent<Odyssey::Transform>()->addScale(-deltaTime, -deltaTime, -deltaTime);
	}
	if (GetAsyncKeyState('G'))
	{
		mGameObject->getComponent<Odyssey::Transform>()->addScale(deltaTime, deltaTime, deltaTime);
	}
}