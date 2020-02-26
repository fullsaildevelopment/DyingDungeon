#include "PurifyMover.h"
#include "Entity.h"
#include "ParticleSystem.h"

CLASS_DEFINITION(Odyssey::Component, PurifyMover)

void PurifyMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
}

std::shared_ptr<Odyssey::Component> PurifyMover::clone() const
{
	return std::make_shared<PurifyMover>(*this);
}

void PurifyMover::update(double delta)
{
	mTransform->addRotation(0.0f, -100.0f * delta, 0.0f);
}

void PurifyMover::onDestroy()
{
}
