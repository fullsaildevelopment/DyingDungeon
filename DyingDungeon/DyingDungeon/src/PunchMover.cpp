#include "PunchMover.h"
#include "Entity.h"

CLASS_DEFINITION(Odyssey::Component, PunchMover)

PunchMover::PunchMover(DirectX::XMFLOAT3 _startPosition, DirectX::XMFLOAT3 _endPosition)
{
	mStartPosition = _startPosition;
	mEndPosition = _endPosition;
}

void PunchMover::initialize()
{
	mTransform = mEntity->getComponent<Odyssey::Transform>();
	timeInFrames = 100.0f;
	mTransform->setPosition(mStartPosition.x, mStartPosition.y, mStartPosition.z);
	mTransform->setRotation(atanf((mEndPosition.y - mStartPosition.y) / (mEndPosition.z - mStartPosition.z)) * (180.00000000f / 3.14159265f), 0.0f,atanf((mEndPosition.y - mStartPosition.y) / (mEndPosition.x - mStartPosition.x)) * (180.00000000f / 3.14159265f) + 90.00000000f);
	mVelocity = DirectX::XMFLOAT3((mEndPosition.x - mStartPosition.x)/timeInFrames, (mEndPosition.y - mStartPosition.y) / timeInFrames, (mEndPosition.z - mStartPosition.z) / timeInFrames);
}

std::shared_ptr<Odyssey::Component> PunchMover::clone() const
{
	return std::make_shared<PunchMover>(*this);
}

void PunchMover::update(double delta)
{

		mTransform->addPosition(mVelocity.x, mVelocity.y, mVelocity.z);

}

void PunchMover::onDestroy()
{
}

void PunchMover::setParticlePath(DirectX::XMFLOAT3 _startPosition, DirectX::XMFLOAT3 _endPosition)
{
	mStartPosition = _startPosition;
	mEndPosition = _endPosition;
	mTransform->setPosition(mStartPosition.x, mStartPosition.y, mStartPosition.z);
	mTransform->setRotation(atanf((mEndPosition.y - mStartPosition.y) / (mEndPosition.z - mStartPosition.z)) * (180.00000000f / 3.14159265f), 0.0f, atanf((mEndPosition.y - mStartPosition.y) / (mEndPosition.x - mStartPosition.x)) * (180.00000000f / 3.14159265f) + 90.00000000f);
	mVelocity = DirectX::XMFLOAT3((mEndPosition.x - mStartPosition.x) / timeInFrames, (mEndPosition.y - mStartPosition.y) / timeInFrames, (mEndPosition.z - mStartPosition.z) / timeInFrames);
}
