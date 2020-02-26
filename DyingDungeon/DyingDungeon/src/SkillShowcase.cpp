#include "OdysseyEngine.h"
#include "SkillShowcase.h"

CLASS_DEFINITION(Component, SkillShowcase)

void SkillShowcase::initialize()
{
	
}

void SkillShowcase::update(double deltaTime)
{
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
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		position = DirectX::XMLoadFloat3(&(bard1->getComponent<Odyssey::Transform>()->getPosition()));
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin1, &paladin1Spawn, position, rotation));
	}
}

std::shared_ptr<Odyssey::Component> SkillShowcase::clone() const
{
	return std::make_shared<SkillShowcase>(*this);
}

void SkillShowcase::onDestroy()
{
}
