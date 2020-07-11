#include "MeteorMover.h"
#include "Entity.h"

CLASS_DEFINITION(Odyssey::Component, MeteorMover)

void MeteorMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	//mMeteorTransform1 = mEntity->getChildren()[0]->getComponent<Odyssey::Transform>();
	//mMeteorTransform2 = mEntity->getChildren()[1]->getComponent<Odyssey::Transform>();
	//mMeteorTransform3 = mEntity->getChildren()[2]->getComponent<Odyssey::Transform>();
	mSpeed = 5.0f;
}

std::shared_ptr<Odyssey::Component> MeteorMover::clone() const
{
	return std::make_shared<MeteorMover>(*this);
}

void MeteorMover::update(double delta)
{
	mTransform->addPosition(0.0f, -mSpeed * delta, 0.0f);
	mTransform->addRotation(0.0f, 360.0f * delta, 0.0f);
	//mEntity->getChildren()[1]->getComponent<Odyssey::Transform>()->addRotation(360.0f * delta, 360.0f * delta, 360.0f * delta);
	//mEntity->getChildren()[2]->getComponent<Odyssey::Transform>()->addRotation(360.0f * delta, 360.0f * delta, 360.0f * delta);
}

void MeteorMover::onDestroy()
{
}