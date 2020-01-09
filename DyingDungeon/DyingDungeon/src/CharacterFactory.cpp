#include "CharacterFactory.h"
#include "FileManager.h"
#include "Animator.h"
#include "Transform.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"

std::shared_ptr<Odyssey::Entity> CharacterFactory::CreateCharacter(CharacterOptions _characterToCreate, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation)
{
	std::shared_ptr<Odyssey::Entity> newCharacter = std::make_shared<Odyssey::Entity>();

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

	case Skeleton:
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

	default:
		break;
	}

	newCharacter->getComponent<Odyssey::Animator>()->setDebugEnabled(true);
	newCharacter->getComponent<Character>()->SetDead(false);
	//newCharacter->getComponent<Character>()->SetStun(false);

	return newCharacter;
}
