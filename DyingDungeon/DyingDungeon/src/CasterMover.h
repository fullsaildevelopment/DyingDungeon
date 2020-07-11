#pragma once
#include "Component.h"
#include "Transform.h"

class CasterMover : public Odyssey::Component
{
	CLASS_DECLARATION(CasterMover)
public:
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();
	void SetRadius(float _radius);
	float GetRadius();
	CasterMover(float _startingDegree);

private:
	Odyssey::Transform* mTransform;
	DirectX::XMFLOAT3 initialPosition;
	float mRadius;
	float mcurrnetDegree;
	float startingDegree;
	float direction;
};

