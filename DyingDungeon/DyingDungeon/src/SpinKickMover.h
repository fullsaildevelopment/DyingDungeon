#pragma once
#include "Component.h"
#include "Transform.h"

class SpinKickMover : public Odyssey::Component
{
	CLASS_DECLARATION(SpinKickMover)
public:
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();
	void SetRadius(float _radius);
	float GetRadius();

private:
	Odyssey::Transform* mTransform;
	float mRadius;
	float mCurrentDegree;
};

