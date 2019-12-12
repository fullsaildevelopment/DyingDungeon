#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

void EnemyComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	mBaseMaxHP = 100.0f;
	mBaseMaxMana = 100.0f;

	SetHP(100);
	SetMana(100);
	SetHero(false);

	skillList[0] = Skills(5, 5);
	//Buffs bleed;
	skillList[1] = Skills(2, 2);
	skillList[2] = Skills(0, 0);
	skillList[3] = Skills(1, 1);
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
	Skills* skill = &skillList[0];
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
	for (Buffs* b : mGameObject->getComponents<Buffs>())
	{
		//reduce its duration by 1
		b->ReduceDuration(1);
		// Check if my buff is over, else see if it need to bleed its effect
		if (b->GetDuration() == 0)
		{
			mGameObject->removeComponent(b);
		}
		else if(b->IsBleed())
			b->Bleed();
	}
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