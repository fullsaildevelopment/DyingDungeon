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
		newCharacter->addComponent<HeroComponent>(HEROID::Paladin);
		newCharacter->setStatic(false);
		HeroComponent* tempHero = newCharacter->getComponent<HeroComponent>();
		break;
	}
	case Mage:
	{
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
		Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/Mage.dxm", true);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Hit", "assets/animations/Mage/Mage_Hit.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("OneHandedCast", "assets/animations/Mage/Mage_1H_Attack.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("TwoHandedCast", "assets/animations/Mage/Mage_2H_Attack.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Dead", "assets/animations/Mage/Mage_Death.dxanim", false);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Mage/Mage_Idle.dxanim");
		newCharacter->addComponent<HeroComponent>(HEROID::Mage);
		newCharacter->setStatic(false);
		HeroComponent* tempHero = newCharacter->getComponent<HeroComponent>();
		tempHero->GetSkills()[0]->SetParticleSystem(setUpFireButBetter());
		tempHero->GetSkills()[0]->SetParticleFiringTime(0.23f);
		tempHero->GetSkills()[0]->SetParticleOffset(DirectX::XMFLOAT3(-2.0f, 3.1f, 0.9f));
		tempHero->GetSkills()[2]->SetParticleSystem(setUpFireStorm());
		tempHero->GetSkills()[2]->SetParticleFiringTime(0.25f);
		tempHero->GetSkills()[2]->SetParticleOffset(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
		
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
		break;
	}
	case Ganfaul:
	{
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
		Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/Ganfaul.dxm", false);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Idle", "assets/animations/Ganfaul/Ganfaul_Idle.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Dead", "assets/animations/Ganfaul/Ganfaul_Death.dxanim", false);
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Hit", "assets/animations/Ganfaul/Ganfaul_Hit.dxanim");
		newCharacter->getComponent<Odyssey::Animator>()->importAnimation("Attack", "assets/animations/Ganfaul/Ganfaul_Attack.dxanim"); 
		newCharacter->addComponent<EnemyComponent>(ENEMYID::Ganfaul);
		break;
	}
	default:
	{
		break;
	}
	}
	newCharacter->setStatic(false);
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
	// Assign the character's impact indicator
	_character->getComponent<Character>()->SetImpactIndicator(impactIndicator);
	// Don't show impact indicator when creating it
	impactIndicator->setActive(false);

	// Add the impact indicator to the game scene
	mGameScene->addEntity(impactIndicator);
}

std::shared_ptr<Odyssey::Entity> CharacterFactory::setUpFireButBetter()
{
	std::shared_ptr<Odyssey::Entity> gFireBall = std::make_shared<Odyssey::Entity>();
	gFireBall->addComponent<Odyssey::Transform>();
	gFireBall->addComponent<Odyssey::ParticleSystem>(*mRenderRefrence);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Fire.jpg");
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setLifetime(0.75f, 1.0f);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setParticleCount(100);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(125);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setSpeed(1.0f, 1.5f);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setSize(1.0f, 1.5f);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	gFireBall->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::SpherePS(0.0f, 0.0f, 0.0f, 1.5f));
	gFireBall->addComponent<ParticleMover>();
	gFireBall->getComponent<ParticleMover>()->SetLifeTime(0.0f);
	gFireBall->getComponent<ParticleMover>()->SetSpeed(1.0f);
	gFireBall->setActive(false);
	mGameScene->addEntity(gFireBall);
	return gFireBall;
}

std::shared_ptr<Odyssey::Entity> CharacterFactory::setUpFireStorm()
{
	std::shared_ptr<Odyssey::Entity> gFireStorm = std::make_shared<Odyssey::Entity>();
	gFireStorm->addComponent<Odyssey::Transform>();
	gFireStorm->addComponent<Odyssey::ParticleSystem>(*mRenderRefrence);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setTexture(Odyssey::TextureType::Diffuse, "Guy.png");
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setColor(DirectX::XMFLOAT3(0.75f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setLifetime(1.5f, 2.5f);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setParticleCount(50);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setRateOverTime(25);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setDuration(5.0);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setSpeed(2.5f, 5.0f);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setSize(1.0f, 1.5f);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setLooping(true);
	gFireStorm->getComponent<Odyssey::ParticleSystem>()->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 100.0f, 180.0f, 180.0f));
	gFireStorm->addComponent<ParticleMover>();
	gFireStorm->getComponent<ParticleMover>()->SetLifeTime(0.0f);
	gFireStorm->getComponent<ParticleMover>()->SetSpeed(1.0f);
	gFireStorm->setActive(false);
	mGameScene->addEntity(gFireStorm);
	return gFireStorm;
}
