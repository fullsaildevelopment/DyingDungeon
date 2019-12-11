#pragma once
#include "Component.h"
#include "Skills.h"

class EnemyComponent : public Odyssey::Component
{
	CLASS_DECLARATION(EnemyComponent)

public:
	EnemyComponent() = default;
public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);
	
	//Attack Functions
	void TakeTurn();
	

	
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

