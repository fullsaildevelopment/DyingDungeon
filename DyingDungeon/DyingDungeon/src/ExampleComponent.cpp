#include "ExampleComponent.h"
#include "SceneObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, ExampleComponent)

void ExampleComponent::initialize(Odyssey::SceneObject* parent)
{
	onEnable(); 
	mParent = parent;
}

void ExampleComponent::update(double deltaTime)
{
	if (GetAsyncKeyState('F'))
	{
		mParent->getComponent<Odyssey::Transform>()->addPosition(0.0f, 10.0f * deltaTime, 0.0f);
	}
}