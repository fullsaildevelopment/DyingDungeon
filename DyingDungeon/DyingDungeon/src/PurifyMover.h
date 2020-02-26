#pragma once
#include "Component.h"
#include "Transform.h"

class PurifyMover : public Odyssey::Component
{
	CLASS_DECLARATION(PurifyMover)
public:
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();

private:
	Odyssey::Transform* mTransform;
};