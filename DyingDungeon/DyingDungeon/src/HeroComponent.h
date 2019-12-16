#pragma once
#include "Character.h"
#include "InputManager.h"

class HeroComponent : public Character
{
	CLASS_DECLARATION(HeroComponent)

public:
	HeroComponent() = default;
	HeroComponent(HEROID id);

	//Attack Functions
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> characters);
	void BasicAttack(Character* target);

	//Death Check
	virtual void Die();


	/////Get and Set Functions/////

	

	/////End of Get and Set Functions/////

private:

};

