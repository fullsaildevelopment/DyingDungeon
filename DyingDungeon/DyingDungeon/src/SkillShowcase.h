#pragma once
#include "Component.h"

class SkillShowcase : public Odyssey::Component
{
	CLASS_DECLARATION(SkillShowcase)
public:
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void onDestroy();

public:
	Odyssey::Entity* bard1;
	Odyssey::Entity* bard1Spawn;
	Odyssey::Entity* bard2;
	Odyssey::Entity* bard2Spawn;
	Odyssey::Entity* bard3;
	Odyssey::Entity* bard3Spawn;
	Odyssey::Entity* bard4;
	Odyssey::Entity* bard4Spawn;

	Odyssey::Entity* paladin1;
	Odyssey::Entity* paladin1Spawn;
	Odyssey::Entity* paladin2;
	Odyssey::Entity* paladin2Spawn;
	Odyssey::Entity* paladin3;
	Odyssey::Entity* paladin3Spawn;
	Odyssey::Entity* paladin4;
	Odyssey::Entity* paladin4Spawn;

	Odyssey::Entity* skeleton1;
	Odyssey::Entity* skeleton1Spawn;
	Odyssey::Entity* skeleton2;
	Odyssey::Entity* skeleton2Spawn;

	Odyssey::Entity* ganfaul1;
	Odyssey::Entity* ganfaul1Spawn;
	
	Odyssey::Entity* caster1;
	Odyssey::Entity* caster1Spawn;
};