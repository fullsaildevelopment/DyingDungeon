#pragma once
#include "Character.h"
#include "InputManager.h"

typedef std::vector<std::shared_ptr<Odyssey::GameObject>> GameObjectList;
class HeroComponent : public Character
{
	CLASS_DECLARATION(HeroComponent)

public:
	HeroComponent() = default;
	HeroComponent(HEROID id);
	~HeroComponent();
	//Attack Functions
	virtual bool TakeTurn(GameObjectList heros, GameObjectList enemies);

	//Death Check
	virtual void Die();
	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:
	Skills* mCurrentSkill;
	Character* mCurrentTarget;
};

