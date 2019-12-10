#pragma once
#include "Component.h"

class EnemyComponent : public Odyssey::Component
{
	CLASS_DECLARATION(EnemyComponent)

public:
	EnemyComponent() = default;
public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);
	float GetHP();
	void SetHP(float m_HP);

private:
	float mHP;
};

