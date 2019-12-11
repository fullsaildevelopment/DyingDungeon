#pragma once
#include "Character.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);

	//Attack Functions
	void TakeTurn();
	

	
	/////Get and Set Functions/////
	

	/////End of Get and Set Functions/////

private:

};

