#pragma once
#include "Character.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	EnemyComponent() = default;
	EnemyComponent(ENEMYID _enemyID);
	//virtual void initialize();
	struct Move 
	{
		Character* target;
		Skills* skill;
		float score = -100;
	};
	//Attack Functions
	bool FindBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets);
	float ScoreMove(Skills skillOption, Character* target);
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> enemyTeam);
	//Death Functions
	virtual void Die();
	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:
	//std::function<float(Skills, Character*)> fScoreMove = nullptr;
	Move bestMove;
	float currentSkillMoveCheck = 0;
	
};

