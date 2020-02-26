#include "OdysseyEngine.h"
#include "SkillShowcase.h"

void SkillShowcase::initialize()
{
}

void SkillShowcase::update(double deltaTime)
{
}

std::shared_ptr<Odyssey::Component> SkillShowcase::clone() const
{
	return std::make_shared<SkillShowcase>(*this);
}

void SkillShowcase::onDestroy()
{
}
