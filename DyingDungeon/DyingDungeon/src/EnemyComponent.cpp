#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

void EnemyComponent::initialize()
{
	onEnable();
	mBaseMaxHP = 100.0f;
	mBaseMaxMana = 100.0f;

	SetHP(100);
	SetMana(100);
	SetHero(false);

	mSkillList[0] = Skills(5, 5);
	mSkillList[1] = Skills(2, 2);
	mSkillList[2] = Skills(0, 0);
	mSkillList[3] = Skills(1, 1);
}

EnemyComponent::Move EnemyComponent::findBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{
	Character* target = nullptr;
	for (std::shared_ptr<Odyssey::GameObject> t : targets)
	{
		if(target = t->getComponent<Character>())
		{
			if (target->IsHero() == true && target->IsDead() == false)
			{
				break;
			}
		}
	}
	Skills* skill = &mSkillList[0];
	Move bestMove;
	bestMove.skill = skill;
	bestMove.target = target;
	return bestMove;
}

float EnemyComponent::ScoreMove(Skills skillOption, Character* target)
{
	float bestScore = 0;
	return bestScore;
}

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{
	// Find my best option
	Move bestMove = findBestMove(targets);
	// Use the best move
	bestMove.skill->Use(*mGameObject->getComponent<Character>(), *bestMove.target);
	// If i have any buffs manage them 
	ManageStatusEffects();
	return true;
}

void EnemyComponent::Die()
{
	if (GetHP() <= 0)
	{
		SetDead(true);
		//TODO Uncomment for death animation
		//mGameObject->getComponent<Odyssey::Animator>()->setAnimationClip("Death");
	}
}