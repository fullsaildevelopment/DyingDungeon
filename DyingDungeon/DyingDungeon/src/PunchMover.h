#pragma once
#include "Component.h"
#include "Transform.h"
class PunchMover : public Odyssey::Component
{
	CLASS_DECLARATION(PunchMover)
public:
	PunchMover(DirectX::XMFLOAT3 _startPosition, DirectX::XMFLOAT3 _endPosition);
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();
	void setParticlePath(DirectX::XMFLOAT3 _startPosition, DirectX::XMFLOAT3 _endPosition);
private:
	Odyssey::Transform* mTransform;
	DirectX::XMFLOAT3 mStartPosition;
	DirectX::XMFLOAT3 mEndPosition;
	DirectX::XMFLOAT3 mVelocity;
	float timeInFrames;
};

