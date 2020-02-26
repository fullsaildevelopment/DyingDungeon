#include "JudgementMover.h"
#include "Entity.h"

CLASS_DEFINITION(Odyssey::Component, JudgementMover)

void JudgementMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
}

std::shared_ptr<Odyssey::Component> JudgementMover::clone() const
{
	return std::make_shared<JudgementMover>(*this);
}

void JudgementMover::update(double delta)
{
	mTransform->addPosition(0.0f, 0.0f, 10.0f * delta);
	mTransform->addRotation(1000.0f * delta, 0.0f, 0.0f);
}

void JudgementMover::onDestroy()
{
}
