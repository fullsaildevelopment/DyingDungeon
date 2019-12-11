#pragma once
#include "Component.h"
#include "Skills.h"

class HeroComponent : public Odyssey::Component
{
	CLASS_DECLARATION(HeroComponent)

public:
	HeroComponent() = default;
public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);

	//Attack Functions
	void TakeTurn();
	void BasicAttack(EnemyComponent& target);


	/////Get and Set Functions/////

	//HP Functions
	float GetHP();
	void SetHP(float m_HP);
	//Mana Functions
	float GetMana();
	void SetMana(float Mana);

	/////End of Get and Set Functions/////

private:
	float mHP;
	float mMana;
	Skills skillList[4];
};

