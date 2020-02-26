#pragma once
#include "Component.h"

class SkillShowcase : public Odyssey::Component
{
public:
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void onDestroy();

public:
	Odyssey::Entity* bard1;
	Odyssey::Entity* bard2;
	Odyssey::Entity* bard3;
	Odyssey::Entity* bard4;
};