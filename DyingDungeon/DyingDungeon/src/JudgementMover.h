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

private:
	Odyssey::Transform* mTransform;
};