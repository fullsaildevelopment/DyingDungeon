#include "JudgementMover.h"
#include "Entity.h"

CLASS_DEFINITION(Odyssey::Component, JudgementMover)

void JudgementMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	mVelocity = { 0.0f,0.0f,10.0f };
	mSpinOnX = true;
}

std::shared_ptr<Odyssey::Component> JudgementMover::clone() const
{
	return std::make_shared<JudgementMover>(*this);
}

void JudgementMover::update(double delta)
{
	mTransform->addPosition(DirectX::XMVectorGetX(mVelocity) * delta, DirectX::XMVectorGetY(mVelocity) * delta, DirectX::XMVectorGetZ(mVelocity) * delta);
	if(mSpinOnX)
		mTransform->addRotation(1000.0f * delta, 0.0f, 0.0f);
	else
		mTransform->addRotation(0.0f, 0.0f, 360.0f * delta);
}

void JudgementMover::onDestroy()
{
}

void JudgementMover::SetVelocity(DirectX::XMFLOAT3 newVelocity, float speed)
{
	mVelocity = DirectX::XMLoadFloat3(&newVelocity);
	mVelocity = DirectX::XMVector3Normalize(mVelocity);
	mVelocity = DirectX::XMVectorMultiply(mVelocity, {speed, speed, speed});

	// Get ready for some math to rotate the meteor to face the velocity direction

	// Get the forward direction
	DirectX::XMFLOAT3 fwd = mEntity->getComponent<Odyssey::Transform>()->getForward();

	// Dot the forward and the velocity
	float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&fwd), mVelocity));

	// Multiply the magnitude of the forward and the velocity
	float mag = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&fwd))) * DirectX::XMVectorGetX(DirectX::XMVector3Length(mVelocity));
	
	// Get the angle between the two vectors and convert it to degrees
	float theta = DirectX::XMConvertToDegrees(std::acosf((dot / mag)));

	// Determine the direction of the rotation
	theta *= (mVelocity.m128_f32[0] > 0) ? 1 : -1;

	// Apply the rotation in the y direction
	mEntity->getComponent<Odyssey::Transform>()->setRotation(0.0f, theta, 0.0f);
}

void JudgementMover::SetSpinOnX(bool spinOnX)
{
	mSpinOnX = spinOnX;
}
