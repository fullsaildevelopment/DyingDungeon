#include "OdysseyEngine.h"
#include "SkillShowcase.h"

CLASS_DEFINITION(Component, SkillShowcase)

void SkillShowcase::initialize()
{
	
}

void SkillShowcase::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::F))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(caster1->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(caster1->getComponent<Odyssey::Transform>()->getEulerRotation()));

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(caster1, &caster1Spawn, position, rotation));
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::O))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(ganfaul1->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(ganfaul1->getComponent<Odyssey::Transform>()->getEulerRotation()));

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(ganfaul1, &ganfaul1Spawn, position, rotation));
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::I))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(skeleton1->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(skeleton1->getComponent<Odyssey::Transform>()->getEulerRotation()));

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(skeleton1, &skeleton1Spawn, position, rotation));

		position = DirectX::XMLoadFloat3(&(skeleton2->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(skeleton2->getComponent<Odyssey::Transform>()->getEulerRotation()));

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(skeleton2, &skeleton2Spawn, position, rotation));
	}
	
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Y))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		position = DirectX::XMLoadFloat3(&(bard1->getComponent<Odyssey::Transform>()->getPosition()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard1, &bard1Spawn, position, rotation));

		position = DirectX::XMLoadFloat3(&(bard2->getComponent<Odyssey::Transform>()->getPosition()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard2, &bard2Spawn, position, rotation));

		position = DirectX::XMLoadFloat3(&(bard3->getComponent<Odyssey::Transform>()->getPosition()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard3, &bard3Spawn, position, rotation));

		position = DirectX::XMLoadFloat3(&(bard4->getComponent<Odyssey::Transform>()->getPosition()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard4, &bard4Spawn, position, rotation));
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::T))
	{
		// Placeholder for spawn position and rotation
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(paladin1->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(paladin1->getComponent<Odyssey::Transform>()->getEulerRotation()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin1, &paladin1Spawn, position, rotation));

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(paladin2->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(paladin2->getComponent<Odyssey::Transform>()->getEulerRotation()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin2, &paladin2Spawn, position, rotation));

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(paladin3->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(paladin3->getComponent<Odyssey::Transform>()->getEulerRotation()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin3, &paladin3Spawn, position, rotation));

		// Get the prefab's position and rotation
		position = DirectX::XMLoadFloat3(&(paladin4->getComponent<Odyssey::Transform>()->getPosition()));
		rotation = DirectX::XMLoadFloat3(&(paladin4->getComponent<Odyssey::Transform>()->getEulerRotation()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin4, &paladin4Spawn, position, rotation));
	}
}

std::shared_ptr<Odyssey::Component> SkillShowcase::clone() const
{
	return std::make_shared<SkillShowcase>(*this);
}

void SkillShowcase::onDestroy()
{
}
