#include "SpinKickMover.h"
#include "Entity.h"

#define _HAS_CXX17 1

#include <iostream> 
#include <algorithm>

CLASS_DEFINITION(Odyssey::Component, SpinKickMover)

void SpinKickMover::initialize()
{
	mcurrnetDegree = 0.0f;
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	mRadius = 1.2f;
	initialPosition = DirectX::XMFLOAT3(mTransform->getPosition().x, mTransform->getPosition().y, mTransform->getPosition().z);
}

std::shared_ptr<Odyssey::Component> SpinKickMover::clone() const
{
	return std::make_shared<SpinKickMover>(*this);
}

void SpinKickMover::update(double delta)
{
	//static float radiance = 0.0f;
	float sinRadius = sinf(mcurrnetDegree) * mRadius;
	float cosRadius = cosf(mcurrnetDegree) * mRadius;
	mcurrnetDegree += (delta*6.0f);
	mcurrnetDegree = max(0.0f, min(mcurrnetDegree, 6.28f));
	if (mcurrnetDegree >= 6.28) {
		mcurrnetDegree = 0.0f;
	}
	mTransform->setPosition(cosRadius + initialPosition.x, mTransform->getPosition().y + 0.00025f, initialPosition.z + sinRadius);
	mTransform->setRotation(0.0f, (mcurrnetDegree*(180.0f /3.14f)), 0.0f);
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
