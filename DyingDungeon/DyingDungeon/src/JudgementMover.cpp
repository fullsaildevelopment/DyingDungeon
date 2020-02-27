#include "JudgementMover.h"
#include "Entity.h"

CLASS_DEFINITION(Odyssey::Component, JudgementMover)

void JudgementMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	mVelocity = { 0.0f,0.0f,10.0f };
}

std::shared_ptr<Odyssey::Component> JudgementMover::clone() const
{
	return std::make_shared<JudgementMover>(*this);
}

void JudgementMover::update(double delta)
{
	mTransform->addPosition(DirectX::XMVectorGetX(mVelocity) * delta, DirectX::XMVectorGetY(mVelocity) * delta, DirectX::XMVectorGetZ(mVelocity) * delta);
	mTransform->addRotation(1000.0f * delta, 0.0f, 0.0f);
}

void JudgementMover::onDestroy()
{
}

void JudgementMover::SetVelocity(DirectX::XMFLOAT3 newVelocity, float speed)
{
	mVelocity = DirectX::XMLoadFloat3(&newVelocity);
	mVelocity = DirectX::XMVector3Normalize(mVelocity);
	mVelocity = DirectX::XMVectorMultiply(mVelocity, {speed, speed, speed});
}
