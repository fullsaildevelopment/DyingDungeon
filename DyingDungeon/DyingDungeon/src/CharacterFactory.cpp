#include "CharacterFactory.h"
#include "FileManager.h"
#include "Animator.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"
#include "GameUIManager.h"

CharacterFactory& CharacterFactory::getInstance()
{
	static CharacterFactory instance;
	return instance;
}

std::shared_ptr<Odyssey::Entity> CharacterFactory::CreateCharacter(CharacterOptions _characterToCreate, std::wstring _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, DirectX::XMFLOAT2 _hudPosition, bool _showHUD, DirectX::XMFLOAT2 _hpPopupPosition, std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the new pointer for the character we are creating
	std::shared_ptr<Odyssey::Entity> newCharacter = std::make_shared<Odyssey::Entity>();
	// Set the mGameScene
	mCurrentScene = _sceneToAddTo;

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
	// Set position
	newCharacter->getComponent<Odyssey::Transform>()->setPosition(xPos, yPos, zPos);
	// Set rotation
	newCharacter->getComponent<Odyssey::Transform>()->setRotation(xRot, yRot, zRot);
	// Set the character's scale
	if (_characterToCreate != Warrior)
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	else
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.023f, 0.023f, 0.023f);

	switch (_characterToCreate)
	{
		case Paladin:
		{
			// Add hero component to the entity
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Paladin);

			// Set up its model
			Odyssey::FileManager::getInstance().importModel(newCharacter,tempHero->GetModel().c_str() , true);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}

			// Set up blood particle effect
			tempHero->SetPSBlood(setupBlood());

			break;
		}
		case Mage:
		{
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Mage);

			// Set up its model
			Odyssey::FileManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}

			// Set up blood particle effect
			tempHero->SetPSBlood(setupBlood());

			// Set up particle effects for skills
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
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Bard);

			// Set up its model
			Odyssey::FileManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}

			// Set up blood particle effect
			tempHero->SetPSBlood(setupBlood());

			break;
		}
		case Warrior:
		{
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Warrior);

			// Set up its model
			Odyssey::FileManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}

			// Set up blood particle effect
			tempHero->SetPSBlood(setupBlood());


			break;
		}
		case Skeleton:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::Skeleton);

			// Set up its model
			Odyssey::FileManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			// Set up blood particle effect
			tempEnemy->SetPSBlood(setupBlood());

			// Set up particle effects for skills
			tempEnemy->GetSkills()[2]->SetParticleSystem(setUpFireStorm());
			tempEnemy->GetSkills()[2]->SetParticleFiringTime(0.57f);

			break;
		}
		case Ganfaul:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::Ganfaul);

			// Set up its model
			Odyssey::FileManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			// Set up blood particle effect
			tempEnemy->SetPSBlood(setupBlood());

			break;
		}
		default:
		{
			break;
		}
	}
	// Set the character's name
	newCharacter->getComponent<Character>()->SetName(_characterName);
	// Allow the character to recieve shadows
	newCharacter->setStatic(false);
	// Have the animator be displayed in debug mode
	newCharacter->getComponent<Odyssey::Animator>()->setDebugEnabled(true);

	// Create entity to add the HUD to
	std::shared_ptr<Odyssey::Entity> hudEntity = std::make_shared<Odyssey::Entity>();
	//Create the character's HUD UI
	GameUIManager::getInstance().CreateCharacterPortrait(_hudPosition, _hpPopupPosition, newCharacter->getComponent<Character>()->GetPortraitPath(), hudEntity.get(), newCharacter->getComponent<Character>());
	// Set the character's hud index number
	// TODO: CREATE THE SETHUDINDEX() IN CHARACTER
	newCharacter->getComponent<Character>()->SetHudIndex(characterHudIndex);
	// Increase the character index
	characterHudIndex++;
	// Set the canvas to active depending on the bool passed in
	hudEntity->getComponent<Odyssey::UICanvas>()->setActive(_showHUD);
	// Add the hud to the scene
	mCurrentScene->addEntity(hudEntity);

	// Create the impact indicator for each character
	CreateCharacterImpactIndicator(newCharacter);

	// Add the character to the current scene
	mCurrentScene->addEntity(newCharacter);
	// Return the brand new charcter that was created
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
	mCurrentScene->addEntity(impactIndicator);
}

Odyssey::ParticleSystem* CharacterFactory::setUpFireButBetter()
{
	std::shared_ptr<Odyssey::Entity> gFireBall = std::make_shared<Odyssey::Entity>();
	gFireBall->addComponent<Odyssey::Transform>();
	Odyssey::Light* light = gFireBall->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.0f, 0.75f, 0.75f);
	light->setIntensity(0.5f);
	light->setRange(10.0f);
	light->setSpotAngle(0.0f);

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

	// TODO: VERIFY CHANGE
	gFireBall->setActive(false);

	gFireBall->addComponent<ParticleMover>();
	gFireBall->getComponent<ParticleMover>()->SetLifeTime(0.0f);
	gFireBall->getComponent<ParticleMover>()->SetSpeed(1.0f);
	mCurrentScene->addEntity(gFireBall);
	mCurrentScene->addLight(gFireBall);
	return fireButBetter;
}

Odyssey::ParticleSystem* CharacterFactory::setUpFireStorm()
{
	std::shared_ptr<Odyssey::Entity> gFireStorm = std::make_shared<Odyssey::Entity>();
	gFireStorm->addComponent<Odyssey::Transform>();
	Odyssey::ParticleSystem* fireStorm = gFireStorm->addComponent<Odyssey::ParticleSystem>(*mRenderRefrence);
	fireStorm->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
	fireStorm->setColor(DirectX::XMFLOAT3(0.8f, 0.5f, 0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	fireStorm->setLifetime(0.5f, 1.5f);
	fireStorm->setParticleCount(700, 1000);
	fireStorm->setRateOverTime(0.0f);
	fireStorm->setDuration(5.0f);
	fireStorm->setSpeed(2.5f, 5.0f);
	fireStorm->setSize(1.0f, 1.5f);
	fireStorm->setLooping(true);
	fireStorm->setShape(Odyssey::ConePS(0.0f,0.0f,0.0f,3.0f,75.0f,75.0f));
	fireStorm->stop();
	mCurrentScene->addEntity(gFireStorm);
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
	mCurrentScene->addEntity(hitEffect);
	return blood;
}