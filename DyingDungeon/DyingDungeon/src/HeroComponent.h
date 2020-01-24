#pragma once
#include "Character.h"
#include "InputManager.h"

typedef std::vector<std::shared_ptr<Odyssey::Entity>> EntityList;
class HeroComponent : public Character
{
	CLASS_DECLARATION(HeroComponent)

public:
	HeroComponent() = default;
	HeroComponent(HEROID id);
	~HeroComponent();
	//Attack Functions
	virtual bool TakeTurn(EntityList heros, EntityList enemies);

	//Death Check
	virtual void Die();

	// Skills functions
	virtual std::vector<std::shared_ptr<Skills>> GetSkills();

	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:
	Skills* mCurrentSkill;
	Character* mCurrentTarget;
	// State functions
	void SelctionState(EntityList heros, EntityList enemies, int moveIndex);
	bool SelectTarget(EntityList targets, int& targetIndex);
	void ResetToSelection(EntityList heros, EntityList enemies);
	void BeginAttack(EntityList targets);
};

