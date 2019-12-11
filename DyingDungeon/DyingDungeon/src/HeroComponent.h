#pragma once
#include "Character.h"

class HeroComponent : public Character
{
	CLASS_DECLARATION(HeroComponent)

public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);

	//Attack Functions
	void TakeTurn();

	//Rework needed
	void BasicAttack(EnemyComponent& target);


	/////Get and Set Functions/////

	

	/////End of Get and Set Functions/////

private:

};

