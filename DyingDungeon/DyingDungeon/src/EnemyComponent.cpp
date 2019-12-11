#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

void EnemyComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	SetHP(100);
	SetMana(100);

	skillList[0] = Skills(5, 5);
	skillList[1] = Skills(2, 2);
	skillList[2] = Skills(0, 0);
	skillList[3] = Skills(1, 1);
}

void EnemyComponent::update(double deltaTime)
{
	
}

EnemyComponent::Move EnemyComponent::findBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{
	Character* target = nullptr;
	for (std::shared_ptr<Odyssey::GameObject> t : targets)
	{
		if (target = t->getComponent<Character>())
		{
			break;
		}
	}
	
}

void EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{

	skillList[0].Use(*mGameObject->getComponent<Character>(), *targets[0]->getComponent<Character>());
}