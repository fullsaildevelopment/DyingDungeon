#pragma once
#include "Component.h"

class ExampleComponent : public Odyssey::Component
{
	CLASS_DECLARATION(ExampleComponent)
public:
	ExampleComponent() = default;
public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);
};