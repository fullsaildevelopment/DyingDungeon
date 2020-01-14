#include "CharacterFactory.h"
#include "FileManager.h"
#include "Animator.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"

std::shared_ptr<Odyssey::Entity> CharacterFactory::CreateCharacter(CharacterOptions _characterToCreate, std::string _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, std::shared_ptr<Odyssey::Scene> _gameScene)
{
	// Create the new pointer for the character we are creating
	std::shared_ptr<Odyssey::Entity> newCharacter = std::make_shared<Odyssey::Entity>();
	// Set the mGameScene
	mGameScene = _gameScene;

	// Get Position values
	float xPos = DirectX::XMVectorGetX(_position);
	float yPos = DirectX::XMVectorGetY(_position);
	float zPos = DirectX::XMVectorGetZ(_position);

	// Get Rotation values
	float xRot = DirectX::XMVectorGetX(_rotation);
	float yRot = DirectX::XMVectorGetY(_rotation);
	float zRot = DirectX::XMVectorGetZ(_rotation);

	// Add the transform
	newCharacter->addComponent<Odyssey::Transform>();
	newCharacter->getComponent<Odyssey::Transform>()->setPosition(xPos, yPos, zPos);
	newCharacter->getComponent<Odyssey::Transform>()->setRotation(xRot, yRot, zRot);

	switch (_characterToCreate)
	{
	case Paladin:
	{
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
		Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/Paladin.dxm", true);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("AttackUp", "assets/animations/Paladin/Paladin_AttackUp.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("BasicAttack", "assets/animations/Paladin/Paladin_BasicAttack.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("BigAttack", "assets/animations/Paladin/Paladin_BigAttack.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Defense", "assets/animations/Paladin/Paladin_Defense.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Heal", "assets/animations/Paladin/Paladin_Heal.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Shield", "assets/animations/Paladin/Paladin_Shield.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Paladin/Paladin_Idle.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Stun", "assets/animations/Paladin/Paladin_Kick.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Dead", "assets/animations/Paladin/Paladin_Death.dxanim", false);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Hit", "assets/animations/Paladin/Paladin_Hit.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("GotBuffed", "assets/animations/Paladin/Paladin_Taunt.dxanim");
		newCharacter->setStatic(false);
		newCharacter->addComponent<HeroComponent>(HEROID::Paladin);
		break;
	}
	case Skeleton:
	{
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
		Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/Skeleton.dxm", false);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Skeleton/Skeleton_Idle.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("BasicAttackButBetter", "assets/animations/Skeleton/Skeleton_BasicAttack.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Dead", "assets/animations/Skeleton/Skeleton_Death.dxanim", false);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("FwdKick", "assets/animations/Skeleton/Skeleton_FwdKick.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Hit", "assets/animations/Skeleton/Skeleton_Hit.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("SpinKick", "assets/animations/Skeleton/Skeleton_SpinKick.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("GotBuffed", "assets/animations/Skeleton/Skeleton_Yell.dxanim");
		newCharacter->addComponent<EnemyComponent>(ENEMYID::Skeleton);
		newCharacter->setStatic(false);
		break;
	}
	case Drake:
	{
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
		Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/.dxm", true);
		break;
	}
	default:
	{
		break;
	}
	}
	newCharacter->getComponent<Odyssey::Animator>()->setDebugEnabled(true);

	// Set the character's name
	newCharacter->getComponent<Character>()->SetName(_characterName);

	// Create the impact indicator for each character
	CreateCharacterImpactIndicator(newCharacter);

	// Add the Character and Impact Indicator to the Game Scene
	mGameScene->addEntity(newCharacter);

	return newCharacter;
}

void CharacterFactory::CreateCharacterImpactIndicator(std::shared_ptr<Odyssey::Entity> _character)
{
	// Create the shared pointer for the impact indicator
	std::shared_ptr<Odyssey::Entity> impactIndicator = std::make_shared<Odyssey::Entity>();
	// Set the transform
	impactIndicator->addComponent<Odyssey::Transform>();
	// Get player's position
	DirectX::XMFLOAT3 characterPos = _character->getComponent<Odyssey::Transform>()->getPosition();
	// Position indicator over the head
	impactIndicator->getComponent<Odyssey::Transform>()->setPosition(characterPos.x, characterPos.y + 4.0f, characterPos.z);
	impactIndicator->getComponent<Odyssey::Transform>()->setRotation(180.0f, 0.0f, 0.0f);
	impactIndicator->getComponent<Odyssey::Transform>()->setScale(0.1f, 0.1, 0.1f);
	// Import Model
	Odyssey::FileManager::getInstance().importModel(impactIndicator, "assets/models/ImpactIndicator.dxm", false);
	// Set the impact indicator's color
	DirectX::XMFLOAT4 impactIndicatorColor = { 255.0f, 0.0f, 0.0f, 1.0f };
	impactIndicator->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor(impactIndicatorColor);
	impactIndicator->setStatic(false);
	// Don't display the indicators when creating them. They will be toggled when character's attack.
	impactIndicator->getComponent<Odyssey::MeshRenderer>()->setActive(false);
	// Assign the character's impact indicator
	_character->getComponent<Character>()->SetImpactIndicator(impactIndicator);

	// Add the impact indicator to the game scene
	mGameScene->addEntity(impactIndicator);
}
