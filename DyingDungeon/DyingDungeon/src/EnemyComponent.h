#pragma once
#include "Character.h"
#include "AiMoves.h"
#include "GameplayTypes.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	EnemyComponent() = default;
	EnemyComponent(GameplayTypes::ENEMYID _enemyID);
	~EnemyComponent();
	//virtual void initialize();
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
	//Death Functions
	virtual void Die();
	void BeginAttack(std::vector<std::shared_ptr<Odyssey::Entity>> targets);

	// Skills functions
	virtual std::vector<std::shared_ptr<Skills>> GetSkills();

	// Extra Mechanics for enemys 
	void (EnemyComponent::*mMechPtr)() = nullptr;
	void GanfaulPhaseMechanic();
	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:
	//std::function<float(Skills, Character*)> fScoreMove = nullptr;
	int currentSkillMoveCheck = 0;
	AIMoves mMoves;
	GameplayTypes::SKILLTYPE mMoveOverride;
};

