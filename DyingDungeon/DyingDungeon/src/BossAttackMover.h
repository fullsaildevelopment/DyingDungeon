#pragma once
#include "Component.h"
#include "Transform.h"
#include "ParticleSystem.h"

class BossAttackMover : public Odyssey::Component
{
	CLASS_DECLARATION(BossAttackMover)
public:
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double delta);
	virtual void onDestroy();
private:
	Odyssey::Transform* mTransform;
	Odyssey::ParticleSystem* mParticle;
	float direction;
};

