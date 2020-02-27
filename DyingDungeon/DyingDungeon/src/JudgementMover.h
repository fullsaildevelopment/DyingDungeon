#pragma once
#include "Component.h"
#include "Transform.h"

class JudgementMover : public Odyssey::Component
{
	CLASS_DECLARATION(JudgementMover)
public:
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();
	void SetVelocity(DirectX::XMFLOAT3 newVelocity, float speed);

private:
	Odyssey::Transform* mTransform;
	DirectX::XMVECTOR mVelocity;
};