#pragma once
#include "Component.h"
#include "Skills.h"

class Character : public Odyssey::Component
{
	CLASS_DECLARATION(Character)

public:
	Character() = default;
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
	//Dead Status Functions
	bool IsDead();
	void SetDead(bool deadStatus);

	/////End of Get and Set Functions/////

protected:
	bool mDead;
	float mHP;
	float mMana;
	Skills skillList[4];


private:

};

