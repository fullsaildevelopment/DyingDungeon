#include "CharacterFactory.h"
#include "FileManager.h"
#include "Animator.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"
#include "GameUIManager.h"

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
		// Set up character pos
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
		// Set up its model
		Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/Paladin.dxm", true);
		// Import its animations
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
		// Add controler
		newCharacter->addComponent<HeroComponent>(HEROID::Paladin);
		// Set shadows
		newCharacter->setStatic(false);
		// Add particle effects
		HeroComponent* tempHero = newCharacter->getComponent<HeroComponent>();
		tempHero->SetPSBlood(setupBlood());
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
		tempHero->SetPSBlood(setupBlood());
		tempHero->GetSkills()[0]->SetParticleSystem(setUpFireButBetter());
		tempHero->GetSkills()[0]->SetParticleFiringTime(0.23f);
		tempHero->GetSkills()[0]->SetParticleOffset(DirectX::XMFLOAT3(-2.0f, 3.1f, 0.9f));
		tempHero->GetSkills()[1]->SetParticleSystem(setUpFireStorm());
		tempHero->GetSkills()[1]->SetParticleFiringTime(0.57f);
		tempHero->GetSkills()[2]->SetParticleSystem(setUpFireStorm());
		tempHero->GetSkills()[2]->SetParticleFiringTime(0.57f);
		tempHero->GetSkills()[3]->SetParticleSystem(setUpFireButBetter());
		tempHero->GetSkills()[3]->SetParticleFiringTime(0.23f);
		tempHero->GetSkills()[3]->SetParticleOffset(DirectX::XMFLOAT3(-2.0f, 3.1f, 0.9f));
		
		break;
	}
	case Bard:
	{
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);

		//Odyssey::FileManager::getInstance().importModel(newCharacter, "assets/models/Bard.dxm", true);

		newCharacter->addComponent<HeroComponent>(HEROID::Bard);

		newCharacter->setStatic(false);

		HeroComponent* tempHero = newCharacter->getComponent<HeroComponent>();
		tempHero->SetPSBlood(setupBlood());

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

		EnemyComponent* tempEnemy = newCharacter->getComponent<EnemyComponent>();
		tempEnemy->SetPSBlood(setupBlood());
		tempEnemy->GetSkills()[2]->SetParticleSystem(setUpFireStorm());
		tempEnemy->GetSkills()[2]->SetParticleFiringTime(0.57f);
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

		EnemyComponent* tempEnemy = newCharacter->getComponent<EnemyComponent>();
		tempEnemy->SetPSBlood(setUpFireStorm());
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

Odyssey::ParticleSystem* CharacterFactory::setUpFireButBetter()
{
	std::shared_ptr<Odyssey::Entity> gFireBall = std::make_shared<Odyssey::Entity>();
	gFireBall->addComponent<Odyssey::Transform>();
	Odyssey::ParticleSystem* fireButBetter = gFireBall->addComponent<Odyssey::ParticleSystem>(*mRenderRefrence);
	fireButBetter->setTexture(Odyssey::TextureType::Diffuse, "Fire.jpg");
	fireButBetter->setColor(DirectX::XMFLOAT3(0.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fireButBetter->setLifetime(0.5f, 1.0f);
	fireButBetter->setParticleCount(100, 150);
	fireButBetter->setRateOverTime(125);
	fireButBetter->setDuration(7.0);
	fireButBetter->setSpeed(1.0f, 1.5f);
	fireButBetter->setSize(1.0f, 1.5f);
	fireButBetter->setLooping(false);
	fireButBetter->setShape(Odyssey::SpherePS(0.0f, 0.0f, 0.0f, 0.05f));
	fireButBetter->stop();
	gFireBall->addComponent<ParticleMover>();
	gFireBall->getComponent<ParticleMover>()->SetLifeTime(0.0f);
	gFireBall->getComponent<ParticleMover>()->SetSpeed(1.0f);
	mGameScene->addEntity(gFireBall);
	return fireButBetter;
}

Odyssey::ParticleSystem* CharacterFactory::setUpFireStorm()
{
	std::shared_ptr<Odyssey::Entity> gFireStorm = std::make_shared<Odyssey::Entity>();
	gFireStorm->addComponent<Odyssey::Transform>();
	Odyssey::ParticleSystem* fireStorm = gFireStorm->addComponent<Odyssey::ParticleSystem>(*mRenderRefrence);
	fireStorm->setTexture(Odyssey::TextureType::Diffuse, "Guy.png");
	fireStorm->setColor(DirectX::XMFLOAT3(0.75f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fireStorm->setLifetime(1.5f, 2.5f);
	fireStorm->setParticleCount(25, 50);
	fireStorm->setRateOverTime(0.0f);
	fireStorm->setDuration(5.0f);
	fireStorm->setSpeed(2.5f, 5.0f);
	fireStorm->setSize(1.0f, 1.5f);
	fireStorm->setLooping(true);
	fireStorm->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.1f, 180.0f, 180.0f));
	fireStorm->stop();
	mGameScene->addEntity(gFireStorm);

	return fireStorm;
}

Odyssey::ParticleSystem* CharacterFactory::setupBlood()
{
	std::shared_ptr<Odyssey::Entity> hitEffect = std::make_shared<Odyssey::Entity>();
	hitEffect->addComponent<Odyssey::Transform>();
	Odyssey::ParticleSystem* blood = hitEffect->addComponent<Odyssey::ParticleSystem>(*mRenderRefrence);
	blood->setTexture(Odyssey::TextureType::Diffuse, "Particle.png");
	blood->setColor(DirectX::XMFLOAT3(0.75f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	blood->setLifetime(0.5f, 0.75f);
	blood->setParticleCount(150, 300);
	blood->setRateOverTime(150);
	blood->setDuration(1.25);
	blood->setSpeed(5.0f, 7.5f);
	blood->setSize(0.25f, 0.5f);
	blood->setGravity(12.5f);
	blood->setLooping(false);
	blood->setShape(Odyssey::SpherePS(0.0f, 2.5f, 0.0f, 0.2f));
	blood->stop();
	mGameScene->addEntity(hitEffect);

	return blood;
}