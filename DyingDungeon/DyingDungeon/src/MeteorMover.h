#pragma once
#include "Component.h"
#include "Transform.h"

class MeteorMover : public Odyssey::Component
{
	CLASS_DECLARATION(MeteorMover)
public:
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();

private:
	Odyssey::Transform* mTransform;
	Odyssey::Transform* mMeteorTransform1;
	Odyssey::Transform* mMeteorTransform2;
	Odyssey::Transform* mMeteorTransform3;
	float mSpeed;
};