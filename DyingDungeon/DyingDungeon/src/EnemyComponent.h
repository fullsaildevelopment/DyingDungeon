#pragma once
#include "Character.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	EnemyComponent() = default;
	virtual void initialize();
	struct Move 
	{
		Character* target;
		Skills* skill;
	};
	//Attack Functions
	Move findBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets);
	float ScoreMove(Skills skillOption, Character* target);
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> targets);
	//Death Functions
	virtual void Die();
	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:

};

