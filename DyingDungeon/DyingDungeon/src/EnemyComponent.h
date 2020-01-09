#pragma once
#include "Character.h"
#include "AiMoves.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	EnemyComponent() = default;
	EnemyComponent(ENEMYID _enemyID);
	~EnemyComponent();
	//virtual void initialize();
	//Attack Functions
	bool FindBestMove(std::vector<std::shared_ptr<Odyssey::Entity>> targets);
	float ScoreMove(Skills* skillOption, Character* target);
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
	//Death Functions
	virtual void Die();
	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:
	//std::function<float(Skills, Character*)> fScoreMove = nullptr;
	int currentSkillMoveCheck = 0;
	AIMoves mMoves;
	SKILLTYPE mMoveOverride;
};

