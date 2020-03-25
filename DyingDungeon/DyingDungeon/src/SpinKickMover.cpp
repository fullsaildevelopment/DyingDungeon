#include "SpinKickMover.h"
#include "Entity.h"

#define _HAS_CXX17 1

#include <iostream> 
#include <algorithm>

CLASS_DEFINITION(Odyssey::Component, SpinKickMover)

void SpinKickMover::initialize()
{
	mCurrentDegree = 0.0f;
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	mRadius = 1.2f;
}

std::shared_ptr<Odyssey::Component> SpinKickMover::clone() const
{
	return std::make_shared<SpinKickMover>(*this);
}

void SpinKickMover::update(double delta)
{
	//static float radiance = 0.0f;
	float sinRadius = sinf(mCurrentDegree) * mRadius;
	float cosRadius = cosf(mCurrentDegree) * mRadius;
	mCurrentDegree += (delta*6.0f);
	mCurrentDegree = max(0.0f, min(mCurrentDegree, 6.28f));
	if (mCurrentDegree >= 6.28) {
		mCurrentDegree = 0.0f;
	}
	mTransform->addPosition(5.0f * cosRadius * delta, 0.5f * delta, 5.0f * sinRadius * delta);
	mTransform->setRotation(0.0f, (mCurrentDegree*(180.0f /3.14f)), 0.0f);
}

void SpinKickMover::onDestroy()
{
}

void SpinKickMover::SetRadius(float _radius)
{
	mRadius = _radius;
}

float SpinKickMover::GetRadius()
{
	return mRadius;
}
