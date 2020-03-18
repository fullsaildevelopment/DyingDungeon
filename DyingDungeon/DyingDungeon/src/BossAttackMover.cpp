#include "BossAttackMover.h"
#include "Entity.h"

CLASS_DEFINITION(Odyssey::Component, BossAttackMover)

void BossAttackMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	mParticle = mEntity->getComponent<Odyssey::ParticleSystem>();
	direction = -1.0f;
}

std::shared_ptr<Odyssey::Component> BossAttackMover::clone() const
{
	return std::make_shared<BossAttackMover>(*this);
}

void BossAttackMover::update(double delta)
{
	mParticle->getShape()->setRadius(DirectX::XMFLOAT3(mParticle->getShape()->mRadius.x + (0.05f* direction), mParticle->getShape()->mRadius.y + (0.05f * direction), mParticle->getShape()->mRadius.z + (0.05f * direction)));
	if (mParticle->getShape()->mRadius.x <= 0.1f && mParticle->getShape()->mRadius.y <= 0.1f && mParticle->getShape()->mRadius.z <= 0.1f && direction == -1.0f) 
	{
		direction = 1.0f;
	}
	else if (mParticle->getShape()->mRadius.x >= 0.5f && mParticle->getShape()->mRadius.y >= 0.5f && mParticle->getShape()->mRadius.z >= 0.5f && direction == 1.0f)
	{
		direction = -1.0f;
	}
}

void BossAttackMover::onDestroy()
{

}