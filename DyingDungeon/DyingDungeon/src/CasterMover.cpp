#include "CasterMover.h"
#include "Entity.h"

#define _HAS_CXX17 1

#include <iostream> 
#include <algorithm>

CLASS_DEFINITION(Odyssey::Component, CasterMover)

CasterMover::CasterMover(float _startingDegree) 
{
	startingDegree = _startingDegree;
}

void CasterMover::initialize()
{
	mcurrnetDegree = startingDegree;
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	mRadius = 1.2f;
	direction = -1.0f;
	initialPosition = DirectX::XMFLOAT3(mTransform->getPosition().x, mTransform->getPosition().y, mTransform->getPosition().z);
}

std::shared_ptr<Odyssey::Component> CasterMover::clone() const
{
	return std::make_shared<CasterMover>(*this);
}

void CasterMover::update(double delta)
{
	//static float radiance = 0.0f;
	float sinRadius = sinf(mcurrnetDegree) * mRadius;
	float cosRadius = cosf(mcurrnetDegree) * mRadius;
	mcurrnetDegree += (delta*6.0f);
	mcurrnetDegree = max(0.0f, min(mcurrnetDegree, 6.28f));
	if (mcurrnetDegree >= 6.28) {
		mRadius += 0.005f * direction;
		mcurrnetDegree = 0.0f;
		if (mRadius <= 0.01f)
		{
			direction = 1.0f;
		}
		else if (mRadius >= 1.2f)
		{
			direction = -1.0f;
		}
	}
	mTransform->setPosition(cosRadius + initialPosition.x, initialPosition.y + sinRadius, initialPosition.z);
	mTransform->setRotation(0.0f, 0.0f, (mcurrnetDegree * (180.0f / 3.14f)));
}

void CasterMover::onDestroy()
{
}

void CasterMover::SetRadius(float _radius)
{
	mRadius = _radius;
}

float CasterMover::GetRadius()
{
	return mRadius;
}
