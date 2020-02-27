#include "CharacterFactory.h"
#include "RenderManager.h"
#include "Animator.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "HeroComponent.h"
#include "EnemyComponent.h"
#include "GameUIManager.h"
#include "PurifyMover.h"
#include "JudgementMover.h"

CharacterFactory& CharacterFactory::getInstance()
{
	static CharacterFactory instance;
	return instance;
}

void CharacterFactory::initialize(Odyssey::Application* _application)
{
	// Set application
	mApplication = _application;

	// Create VXF's prefab
	MakeVXFEffects();

	// Get the entity that the new prefab will be added to
	Odyssey::Entity* newCharacter;

	// Create the paladin prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Paladin);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Paladin] = newCharacter;

	// Create the mage prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Mage);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Mage] = newCharacter;

	// Create the bard prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Bard);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Bard] = newCharacter;

	// Create the warrior prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Warrior);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Warrior] = newCharacter;

	// Create the monk prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Monk);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Monk] = newCharacter;

	// Create the skeleton prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Skeleton);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Skeleton] = newCharacter;

	// Create the summoner prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Summoner);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Summoner] = newCharacter;
	
	// Create the melee demon prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::MeleeDemon);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::MeleeDemon] = newCharacter;
	
	// Create the caster demon prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::CasterDemon);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::CasterDemon] = newCharacter;

	// Create the skeleton prefab
	newCharacter = CreateCharacterPrefab(CharacterOptions::Ganfaul);
	// Add the new character to the prefab map
	mCharacterPrefabMap[CharacterFactory::Ganfaul] = newCharacter;

	// Create the Hero HUD
	Odyssey::Entity* heroHud = CreateHeroHudPrefab(DirectX::XMFLOAT2(10.0f, 600.0f));
	// Add the hero hud to the hud prefab map
	mHudPrefabMap[HudID::HeroLeft] = heroHud;

	// Create the Hero HUD
	heroHud = CreateHeroHudPrefab(DirectX::XMFLOAT2(470.0f, 600.0f));
	// Add the hero hud to the hud prefab map
	mHudPrefabMap[HudID::HeroMiddle] = heroHud;

	// Create the Hero HUD
	heroHud = CreateHeroHudPrefab(DirectX::XMFLOAT2(910.0f, 600.0f));
	// Add the hero hud to the hud prefab map
	mHudPrefabMap[HudID::HeroRight] = heroHud;

	// Create the Enemy HUD
	Odyssey::Entity* enemyHud = CreateEnemyHudPrefab(DirectX::XMFLOAT2(200.0f, 10.0f));
	// Add the enemy hud to the hud prefab map
	mHudPrefabMap[HudID::EnemyLeft] = enemyHud;

	// Create the Enemy HUD
	enemyHud = CreateEnemyHudPrefab(DirectX::XMFLOAT2(533.0f, 10.0f));
	// Add the enemy hud to the hud prefab map
	mHudPrefabMap[HudID::EnemyMiddle] = enemyHud;

	// Create the Enemy HUD
	enemyHud = CreateEnemyHudPrefab(DirectX::XMFLOAT2(863.0f, 10.0f));
	// Add the enemy hud to the hud prefab map
	mHudPrefabMap[HudID::EnemyRight] = enemyHud;

	// Create the Left Skill Hover Prefab
	Odyssey::Entity* skillHoverHUD = CreateSkillHoverHudPrefab(DirectX::XMFLOAT2(10.0f, 582.0f));
	// Add the skill hover hud to the skill hover prefab map
	mSkillHoverPrefabMap[SkillHoverID::LeftHUD] = skillHoverHUD;

	// Create the Middle Skill Hover Prefab
	skillHoverHUD = CreateSkillHoverHudPrefab(DirectX::XMFLOAT2(470.0f, 582.0f));
	// Add the skill hover hud to the skill hover prefab map
	mSkillHoverPrefabMap[SkillHoverID::MiddleHUD] = skillHoverHUD;

	// Create the Right Skill Hover Prefab
	skillHoverHUD = CreateSkillHoverHudPrefab(DirectX::XMFLOAT2(910.0f, 582.0f));
	// Add the skill hover hud to the skill hover prefab map
	mSkillHoverPrefabMap[SkillHoverID::RightHUD] = skillHoverHUD;

	// Create the impact indicator 
	mImpactIndicatorPrefab = CreateImpactIndicatorPrefab();

	// Create the turn indicator
	mTurnIndicatorPrefab = CreateTurnIndicatorPrefab();

	// Create the blood effect
	mBloodEffect = CreateBloodEffectPrefab();
}

// Getters
Odyssey::Entity* CharacterFactory::GetCharacterPrefab(CharacterOptions _characterType)
{
	return mCharacterPrefabMap[_characterType];
}

Odyssey::Entity* CharacterFactory::GetHUDPrefab(HudID _hudToGet)
{
	// Default to the enemy right hud
	Odyssey::Entity* myHud = mHudPrefabMap[HudID::EnemyRight];

	// Check what hud we wanted to get
	switch (_hudToGet)
	{
		case HudID::HeroLeft:
			myHud = mHudPrefabMap[HudID::HeroLeft];
			break;
		case HudID::HeroMiddle:
			myHud = mHudPrefabMap[HudID::HeroMiddle];
			break;
		case HudID::HeroRight:
			myHud = mHudPrefabMap[HudID::HeroRight];
			break;
		case HudID::EnemyLeft:
			myHud = mHudPrefabMap[HudID::EnemyLeft];
			break;
		case HudID::EnemyMiddle:
			myHud = mHudPrefabMap[HudID::EnemyMiddle];
			break;
		case HudID::EnemyRight:
			myHud = mHudPrefabMap[HudID::EnemyRight];
			break;
		default:
			std::cout << "Didn't get the hud in Char Factory, GetHUDPrefab!" << std::endl;
			break;
	}
	
	// Return the hud I wanted;
	return myHud;
}

Odyssey::Entity* CharacterFactory::GetSkillHoverHUDPrefab(SkillHoverID _hoverHudToGet)
{
	// Default to the enemy right hud
	Odyssey::Entity* mySkillHoverHud = mSkillHoverPrefabMap[SkillHoverID::LeftHUD];

	// Check what hud we wanted to get
	switch (_hoverHudToGet)
	{
		case SkillHoverID::LeftHUD:
			mySkillHoverHud = mSkillHoverPrefabMap[SkillHoverID::LeftHUD];
			break;
		case SkillHoverID::MiddleHUD:
			mySkillHoverHud = mSkillHoverPrefabMap[SkillHoverID::MiddleHUD];
			break;
		case SkillHoverID::RightHUD:
			mySkillHoverHud = mSkillHoverPrefabMap[SkillHoverID::RightHUD];
			break;
		default:
			std::cout << "Didn't get the hud in Char Factory, GetHUDPrefab!" << std::endl;
			break;
	}

	// Return the hud I wanted;
	return mySkillHoverHud;
}

Odyssey::Entity* CharacterFactory::GetImpactIndicatorPrefab() 
{ 
	return mImpactIndicatorPrefab; 
}

Odyssey::Entity* CharacterFactory::GetTurnIndicatorPrefab() 
{ 
	return mTurnIndicatorPrefab; 
}

Odyssey::Entity* CharacterFactory::GetBloodEffectPrefab()
{
	return mBloodEffect;
}

// Creates
Odyssey::Entity* CharacterFactory::CreateCharacterPrefab(CharacterOptions _characterToCreate)
{
	// Create the new pointer for the character we are creating
	Odyssey::Entity* newCharacter = nullptr;
	// Make the new character a prefab
	newCharacter = mApplication->createPrefab();

	std::wstring characterName = L"This player go no name, CharFactory.cpp";

	// Get Position values
	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;
	// Get Rotation values
	float xRot = 0.0f;
	float yRot = 0.0f;
	float zRot = 0.0f;

	// Add the transform
	newCharacter->addComponent<Odyssey::Transform>();
	// Set position
	newCharacter->getComponent<Odyssey::Transform>()->setPosition(xPos, yPos, zPos);
	// Set rotation
	newCharacter->getComponent<Odyssey::Transform>()->setRotation(xRot, yRot, zRot);
	// Set the character's scale
	if (_characterToCreate == Warrior)
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.023f, 0.023f, 0.023f);
	else if (_characterToCreate == Mage)
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.55f, 0.55f, 0.55f);
	else if (_characterToCreate == Paladin)
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.028f, 0.028f, 0.028f);
	else
		newCharacter->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);

	switch (_characterToCreate)
	{
		case Paladin:
		{
			// Add hero component to the entity
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Paladin);

			tempHero->GetSkills()[0]->SetParticleSystem(mVXFMap["PallySkill1"]);
			tempHero->GetSkills()[0]->SetParticleOffset({ 0.0f, 3.0f, 0.0f });
			tempHero->GetSkills()[0]->SetParticleFiringTime(0.1f);
			tempHero->GetSkills()[0]->SetPartilceIsProjectile(true);

			tempHero->GetSkills()[1]->SetParticleSystem(mVXFMap["PallySkill2"]);
			tempHero->GetSkills()[1]->SetParticleOffset({ 0.0f, 6.0f, 0.0f });
			tempHero->GetSkills()[1]->SetParticleFiringTime(0.1f);

			tempHero->GetSkills()[2]->SetParticleSystem(mVXFMap["PallySkill3"]);
			tempHero->GetSkills()[2]->SetParticleOffset({ 0.0f, 0.0f, 0.0f });
			tempHero->GetSkills()[2]->SetParticleFiringTime(0.1f);

			tempHero->GetSkills()[3]->SetParticleSystem(mVXFMap["PallySkill3"]);
			tempHero->GetSkills()[3]->SetParticleOffset({ 0.0f, 0.0f, 0.0f });
			tempHero->GetSkills()[3]->SetParticleFiringTime(0.1f);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);
			newCharacter->getChildren()[0]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[1]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[2]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[3]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}
			characterName = L"Paladin";
			break;
		}
		case Mage:
		{
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Mage);

			tempHero->GetSkills()[0]->SetParticleSystem(mVXFMap["MageSkill1"]);
			tempHero->GetSkills()[0]->SetParticleOffset({ 0.0f, 3.0f, 0.0f });
			tempHero->GetSkills()[0]->SetParticleFiringTime(0.1f);
			tempHero->GetSkills()[0]->SetPartilceIsProjectile(true);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}
			characterName = L"Mage";
			break;
		}
		case Bard:
		{
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Bard);

			// Set up heros VXF's
			tempHero->GetSkills()[0]->SetParticleSystem(mVXFMap["bardSkill1"]);
			tempHero->GetSkills()[0]->SetParticleOffset({0.0f, 3.0f, 0.0f});
			tempHero->GetSkills()[0]->SetParticleFiringTime(0.1f);

			tempHero->GetSkills()[1]->SetParticleSystem(mVXFMap["bardSkill2"]);
			tempHero->GetSkills()[1]->SetParticleOffset({ 0.0f, 0.0f, 0.0f });
			tempHero->GetSkills()[1]->SetParticleFiringTime(0.1f);

			tempHero->GetSkills()[2]->SetParticleSystem(mVXFMap["bardSkill3"]);
			tempHero->GetSkills()[2]->SetParticleOffset({ 0.0f, 0.0f, 0.0f });
			tempHero->GetSkills()[2]->SetParticleFiringTime(0.1f);

			tempHero->GetSkills()[3]->SetParticleSystem(mVXFMap["bardSkill4"]);
			tempHero->GetSkills()[3]->SetParticleOffset({ 0.0f, 0.0f, 0.0f });
			tempHero->GetSkills()[3]->SetParticleFiringTime(0.1f);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);

			newCharacter->getChildren()[0]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[1]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[2]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[3]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			newCharacter->getChildren()[4]->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });
			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}
			characterName = L"Bard";
			break;
		}
		case Warrior:
		{
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Warrior);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), true);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Warrior";
			break;
		}
		case Monk:
		{
			// Set up hero component
			HeroComponent* tempHero = newCharacter->addComponent<HeroComponent>(GameplayTypes::HEROID::Monk);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempHero->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempHero->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempHero->GetAnimationPaths()[i].mAnimationNickName, tempHero->GetAnimationPaths()[i].mAnimationPath.c_str(), tempHero->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Monk";
			break;
		}
		case Skeleton:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::Skeleton);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			newCharacter->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Skeleton";
			break;
		}
		case Summoner:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::Summoner);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Summoner";
			break;
		}
		case MeleeDemon:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::MeleeDemon);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Summoner";
			break;
		}
		case CasterDemon:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::CasterDemon);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Summoner";
			break;
		}
		case Ganfaul:
		{
			// Set up enemy component
			EnemyComponent* tempEnemy = newCharacter->addComponent<EnemyComponent>(GameplayTypes::ENEMYID::Ganfaul);

			// Set up its model
			Odyssey::RenderManager::getInstance().importModel(newCharacter, tempEnemy->GetModel().c_str(), false);

			// For each animation in its vector of animations path, import an animation
			for (int i = 0; i < tempEnemy->GetAnimationPaths().size(); ++i)
			{
				newCharacter->getComponent<Odyssey::Animator>()->importAnimation(tempEnemy->GetAnimationPaths()[i].mAnimationNickName, tempEnemy->GetAnimationPaths()[i].mAnimationPath.c_str(), tempEnemy->GetAnimationPaths()[i].mIsLooping);
			}

			characterName = L"Ganfaul";
			break;
		}
		default:
		{
			break;
		}
	}
	// Set the character's name
	newCharacter->getComponent<Character>()->SetName(characterName);
	// Allow the character to recieve shadows
	newCharacter->setStatic(false);

	// Return the brand new charcter that was created
	return newCharacter;
}

Odyssey::Entity* CharacterFactory::CreateHeroHudPrefab(DirectX::XMFLOAT2 _hudPosition)
{
	// Create the new pointer for the hero hud
	Odyssey::Entity* heroHud = nullptr;
	// Make the new hero hud a prefab
	heroHud = mApplication->createPrefab();

	// Create the hero hud
	GameUIManager::getInstance().CreateHeroHud(heroHud, _hudPosition);

	return heroHud;
}

Odyssey::Entity* CharacterFactory::CreateEnemyHudPrefab(DirectX::XMFLOAT2 _hudPosition)
{
	// Create the new pointer for the enemy hud
	Odyssey::Entity* enemyHud = nullptr;
	// Make the new enemy hud a prefab
	enemyHud = mApplication->createPrefab();

	// Create the enemy hud
	GameUIManager::getInstance().CreateEnemyHud(enemyHud, _hudPosition);

	return enemyHud;
}

Odyssey::Entity* CharacterFactory::CreateSkillHoverHudPrefab(DirectX::XMFLOAT2 _hudPosition)
{
	// Create the new pointer for the enemy hud
	Odyssey::Entity* skillHoverHud = nullptr;
	// Make the new enemy hud a prefab
	skillHoverHud = mApplication->createPrefab();

	// Create the enemy hud
	GameUIManager::getInstance().CreateSkillHoverHud(skillHoverHud, _hudPosition);

	return skillHoverHud;
}

Odyssey::Entity* CharacterFactory::CreateHpPopupPrefab()
{
	// Create the new pointer for the enemy hud
	Odyssey::Entity* hpPopup = nullptr;
	// Make the new enemy hud a prefab
	hpPopup = mApplication->createPrefab();

	// Create the hp popup
	GameUIManager::getInstance().CreateHpPopup(hpPopup);

	return hpPopup;
}

Odyssey::Entity* CharacterFactory::CreateImpactIndicatorPrefab()
{
	// Create the new pointer for the enemy hud
	Odyssey::Entity* impactIndicator = nullptr;
	// Make the new enemy hud a prefab
	impactIndicator = mApplication->createPrefab();

	// Set the transform
	impactIndicator->addComponent<Odyssey::Transform>();
	// Position indicator over the head
	impactIndicator->getComponent<Odyssey::Transform>()->setPosition(0.0f, 4.0f, 0.0f);
	impactIndicator->getComponent<Odyssey::Transform>()->setRotation(180.0f, 0.0f, 0.0f);
	impactIndicator->getComponent<Odyssey::Transform>()->setScale(0.5f, 0.5f, 0.5f);
	// Import Model
	Odyssey::RenderManager::getInstance().importModel(impactIndicator, "assets/models/TurnIndicator.dxm", false);
	// Set the impact indicator's color
	DirectX::XMFLOAT4 impactIndicatorColor = { 255.0f, 0.0f, 0.0f, 1.0f };
	impactIndicator->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor(impactIndicatorColor);
	impactIndicator->setStatic(false);

	return impactIndicator;
}

Odyssey::Entity* CharacterFactory::CreateTurnIndicatorPrefab()
{
	// Create the new pointer for the enemy hud
	Odyssey::Entity* turnIndicator = nullptr;
	// Make the new enemy hud a prefab
	turnIndicator = mApplication->createPrefab();

	turnIndicator->addComponent<Odyssey::Transform>();
	turnIndicator->getComponent<Odyssey::Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	turnIndicator->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);
	Odyssey::RenderManager::getInstance().importModel(turnIndicator, "assets/models/TurnIndicator.dxm", false);
	DirectX::XMFLOAT4 turnIndicatorColor = { 0.0f, 0.0f, 255.0f, 1.0f };
	turnIndicator->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor(turnIndicatorColor);
	turnIndicator->setStatic(false);

	return turnIndicator;
}

Odyssey::Entity* CharacterFactory::CreateBloodEffectPrefab()
{
	Odyssey::Entity* bloodEffect = mApplication->createPrefab();
	bloodEffect->addComponent<Odyssey::Transform>();
	Odyssey::ParticleSystem* blood = bloodEffect->addComponent<Odyssey::ParticleSystem>();
	blood->setTexture(Odyssey::TextureType::Diffuse, "Particle.png");
	blood->setColor(DirectX::XMFLOAT3(0.75f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	blood->setLifetime(0.5f, 0.75f);
	blood->setParticleCount(150, 300);
	blood->setEmissionOverLifetime(150);
	blood->setDuration(1.25);
	blood->setSpeed(5.0f, 7.5f);
	blood->setSize(0.25f, 0.5f);
	blood->setGravity(12.5f);
	blood->setLooping(false);
	blood->setShape(Odyssey::SpherePS(0.0f, 2.5f, 0.0f, 0.2f));
	return bloodEffect;
}

void CharacterFactory::MakeVXFEffects()
{
	// Bard Effects
	BardSkill1Prefab();
	BardSkill2Prefab();
	BardSkill3Prefab();
	BardSkill4Prefab();
	// Pally Effects
	PallySkill1Prefab();
	PallySkill2Prefab();
	PallySkill3Prefab();
	// Mage
	MageSkill1Prefab();
}

// Bard VXF
void CharacterFactory::BardSkill1Prefab()
{
	// Create the skill 1 prefab
	Odyssey::Entity* mBardSkill = mApplication->createPrefab();
	mBardSkill->addComponent<Odyssey::Transform>();
	mBardSkill->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 5.0f);

	// Add a light to the prefab
	Odyssey::Light* light = mBardSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.5f, 0.3f, 0.6f);
	light->setRange(10.0f);
	light->setIntensity(2.0f);

	// Setup the starfire arrow vfx
	Odyssey::ParticleSystem* skillVFX = mBardSkill->addComponent<Odyssey::ParticleSystem>();
	mBardSkill->setStatic(false);
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Star2.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.5f, 0.3f, 0.6f), DirectX::XMFLOAT3(0.2f, 0.4f, 0.5f));
	skillVFX->setLifetime(0.5f, 1.0f);
	skillVFX->setParticleCount(100, 100);
	skillVFX->setEmissionOverLifetime(0);
	skillVFX->setDuration(0.75f);
	skillVFX->setSpeed(2.5f, 5.0f);
	skillVFX->setSize(0.25f, 0.5f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(4.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::SpherePS(0.0f, 0.0f, 0.0f, 0.5f));

	mVXFMap["bardSkill1"] = mBardSkill;
}
void CharacterFactory::BardSkill2Prefab()
{
	// Create the skill 2 prefab
	Odyssey::Entity* mBardSkill = mApplication->createPrefab();
	mBardSkill->addComponent<Odyssey::Transform>();
	mBardSkill->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 10.0f);

	// Add a light to the prefab
	Odyssey::Light* light = mBardSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.8f, 0.1f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup skill 2 vfx
	Odyssey::ParticleSystem* skillVFX = mBardSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Bard_VFX/Music.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.8f, 0.1f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(1.0f, 1.5f);
	skillVFX->setParticleCount(0, 15);
	skillVFX->setEmissionOverLifetime(15);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.5f, 2.25f);
	skillVFX->setSize(0.75f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(1.5f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::BoxPS(0.0f, 1.0f, 0.0f, 1.5f, 2.5f, 1.5f));

	mVXFMap["bardSkill2"] = mBardSkill;
}
void CharacterFactory::BardSkill3Prefab()
{
	// Create the skill 3 prefab
	Odyssey::Entity* mBardSkill = mApplication->createPrefab();
	mBardSkill->addComponent<Odyssey::Transform>();
	mBardSkill->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 15.0f);

	// Add a light to the prefab
	Odyssey::Light*  light = mBardSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.1f, 0.8f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Skill 3 prefab
	Odyssey::ParticleSystem* skillVFX = mBardSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Bard_VFX/Music.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.1f, 0.8f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(1.0f, 1.5f);
	skillVFX->setParticleCount(0, 15);
	skillVFX->setEmissionOverLifetime(15);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(1.5f, 2.25f);
	skillVFX->setSize(0.75f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(1.5f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::BoxPS(0.0f, 1.0f, 0.0f, 1.5f, 2.5f, 1.5f));

	mVXFMap["bardSkill3"] = mBardSkill;
}
void CharacterFactory::BardSkill4Prefab()
{
	// Create the skill 4 prefab
	Odyssey::Entity* mBardSkill = mApplication->createPrefab();
	mBardSkill->addComponent<Odyssey::Transform>();
	mBardSkill->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 15.0f);

	// Add a light to the prefab
	Odyssey::Light* light = mBardSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.12f, 1.0f, 0.29f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup the skill 4 vfx
	mBardSkill->addComponent<PurifyMover>();
	mBardSkill->getComponent<Odyssey::Transform>()->setScale(0.02f, 0.02f, 0.02f);
	Odyssey::RenderManager::getInstance().importModel(mBardSkill, "assets/models/Magic_Rune.dxm", false);
	mBardSkill->getComponent<Odyssey::MeshRenderer>()->setShadowCaster(false);
	mBardSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setAlphaBlend(true);
	mBardSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setReceiveShadow(false);
	mBardSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor({ 0.12f, 1.0f, 0.29f, 1.0f });
	mBardSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setShader("../OdysseyEngine/shaders/UnlitPixelShader.cso");
	Odyssey::ParticleSystem* skillVFX = mBardSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Purify.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.12f, 1.0f, 0.29f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.75f, 1.5f);
	skillVFX->setParticleCount(0, 90);
	skillVFX->setEmissionOverLifetime(90);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(3.0f, 4.5f);
	skillVFX->setSize(0.25f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 1.0f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, true));

	mVXFMap["bardSkill4"] = mBardSkill;
}

// Pally VXF
void CharacterFactory::PallySkill1Prefab()
{
	// Create the skill 1 prefab
	Odyssey::Entity* mSkill = mApplication->createPrefab();
	// Import the hammer model
	Odyssey::RenderManager::getInstance().importModel(mSkill, "assets/models/Hammer.dxm", false);
	mSkill->getComponent<Odyssey::Transform>()->setScale(0.05f, 0.05f, 0.05f);
	// Add the skill mover to the prefab
	mSkill->addComponent<JudgementMover>();
	mSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.15f, 0.15f, 0.15f, 1.0f });

	// Add a light to the prefab
	Odyssey::Light* light = mSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.45f, 0.65f, 0.35f);
	light->setRange(10.0f);
	light->setIntensity(1.0f);

	// Setup the skill 3 vfx
	Odyssey::ParticleSystem* skillVFX = mSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Electric.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.25f, 0.75f);
	skillVFX->setParticleCount(0, 50);
	skillVFX->setEmissionOverLifetime(75);
	skillVFX->setDuration(4.0f);
	skillVFX->setSpeed(1.25f, 1.75f);
	skillVFX->setSize(0.25f, 0.75f);
	skillVFX->setSizeOverLifetime(0.0f, 0.5f);
	skillVFX->setGravity(2.5f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::SpherePS(0.0f, 0.25f, 0.0f, 0.5f));

	mVXFMap["PallySkill1"] = mSkill;
}
void CharacterFactory::PallySkill2Prefab()
{
	// Create the skill 2 prefab
	Odyssey::Entity* mSkill = mApplication->createPrefab();
	mSkill->addComponent<Odyssey::Transform>();
	mSkill->getComponent<Odyssey::Transform>()->setPosition(20.0f, 7.5f, 10.0f);
	mSkill->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 180.0f);

	// Add a light to the prefab
	Odyssey::Light* light = mSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.75f, 0.65f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);
	// Setup the skill 2 vfx
	mSkill->getComponent<Odyssey::Transform>()->setScale(0.025f, 0.025f, 0.025f);
	Odyssey::ParticleSystem* skillVFX = mSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Lightning.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.4f, 0.2f), DirectX::XMFLOAT3(0.75f, 0.1f, 0.1f));
	skillVFX->setLifetime(2.0f, 2.0f);
	skillVFX->setParticleCount(0, 15);
	skillVFX->setEmissionOverLifetime(30);
	skillVFX->setDuration(2.0f);
	skillVFX->setSpeed(8.5f, 9.0f);
	skillVFX->setSize(1.0f, 1.25f);
	skillVFX->setSizeOverLifetime(0.5f, 0.75f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.001f, 0.001f, 0.001f));
	skillVFX = mSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Electric.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(2.0f, 2.0f);
	skillVFX->setParticleCount(0, 75);
	skillVFX->setEmissionOverLifetime(150);
	skillVFX->setDuration(2.0f);
	skillVFX->setSpeed(8.5f, 9.0f);
	skillVFX->setSize(0.5f, 0.75f);
	skillVFX->setSizeOverLifetime(0.25f, 0.5f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::ConePS(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f));

	mVXFMap["PallySkill2"] = mSkill;
}
void CharacterFactory::PallySkill3Prefab()
{
	// Create the skill 4 prefab
	Odyssey::Entity* mSkill = mApplication->createPrefab();
	mSkill->addComponent<Odyssey::Transform>();
	mSkill->getComponent<Odyssey::Transform>()->setPosition(20.0f, 2.5f, 15.0f);

	// Add a light to the prefab
	Odyssey::Light* light = mSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(1.0f, 0.8f, 0.1f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	// Setup the skill 4 vfx
	mSkill->addComponent<PurifyMover>();
	mSkill->getComponent<Odyssey::Transform>()->setScale(0.02f, 0.02f, 0.02f);
	Odyssey::RenderManager::getInstance().importModel(mSkill, "assets/models/Magic_Rune.dxm", false);
	mSkill->getComponent<Odyssey::MeshRenderer>()->setShadowCaster(false);
	mSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setAlphaBlend(true);
	mSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setReceiveShadow(false);
	mSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setDiffuseColor({ 0.75f, 0.65f, 0.1f, 1.0f });
	mSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setShader("../OdysseyEngine/shaders/UnlitPixelShader.cso");

	Odyssey::ParticleSystem* skillVFX = mSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Purify.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.65f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.75f, 1.5f);
	skillVFX->setParticleCount(0, 90);
	skillVFX->setEmissionOverLifetime(90);
	skillVFX->setDuration(3.0f);
	skillVFX->setSpeed(3.0f, 4.5f);
	skillVFX->setSize(0.25f, 1.0f);
	skillVFX->setSizeOverLifetime(0.25f, 1.0f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::CirclePS(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, true));

	mVXFMap["PallySkill3"] = mSkill;
}

void CharacterFactory::MageSkill1Prefab()
{
	Odyssey::Entity* mSkill = mApplication->createPrefab();
	mSkill->addComponent<Odyssey::Transform>();
	mSkill->getComponent<Odyssey::Transform>()->setRotation(0.0f, 0.0f, 0.0f);

	// Add a light to the prefab
	Odyssey::Light* light = mSkill->addComponent<Odyssey::Light>();
	light->setLightType(Odyssey::LightType::Point);
	light->setColor(0.75f, 0.25f, 0.0f);
	light->setRange(10.0f);
	light->setIntensity(0.5f);

	mSkill->addComponent<JudgementMover>();
	mSkill->getComponent<Odyssey::Transform>()->setScale(0.015f, 0.015f, 0.015f);
	Odyssey::RenderManager::getInstance().importModel(mSkill, "assets/models/Shard.dxm", false);
	mSkill->getComponent<Odyssey::MeshRenderer>()->getMaterial()->setGlobalAmbient({ 0.75f, 0.75f, 0.75f, 1.0f });
	Odyssey::ParticleSystem* skillVFX = mSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Flak1.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.6f, 0.1f, 0.0f), DirectX::XMFLOAT3(0.2f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.5f, 1.0f);
	skillVFX->setParticleCount(0, 100);
	skillVFX->setEmissionOverLifetime(150);
	skillVFX->setDuration(2.0f);
	skillVFX->setSpeed(1.5f, 2.0f);
	skillVFX->setSize(0.075f, 0.15f);
	skillVFX->setSizeOverLifetime(0.0f, 0.25f);
	skillVFX->setGravity(1.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::SpherePS(0.0f, 0.0f, 0.0f, 0.75f));
	skillVFX = mSkill->addComponent<Odyssey::ParticleSystem>();
	skillVFX->setTexture(Odyssey::TextureType::Diffuse, "Trail4.png");
	skillVFX->setColor(DirectX::XMFLOAT3(0.75f, 0.25f, 0.0f), DirectX::XMFLOAT3(0.2f, 0.0f, 0.0f));
	skillVFX->setLifetime(0.5f, 1.0f);
	skillVFX->setParticleCount(0, 100);
	skillVFX->setEmissionOverLifetime(150);
	skillVFX->setDuration(2.0f);
	skillVFX->setSpeed(1.5f, 2.0f);
	skillVFX->setSize(0.3f, 0.5f);
	skillVFX->setSizeOverLifetime(0.0f, 0.25f);
	skillVFX->setGravity(0.0f);
	skillVFX->setLooping(false);
	skillVFX->setShape(Odyssey::BoxPS(0.0f, 0.0f, 0.0f, 0.4f, 0.4f, 1.75f));

	mVXFMap["MageSkill1"] = mSkill;
}




// TODO: THIS IS OTHER STUFF WE WILL MAKE INTO PREFABS
//Odyssey::ParticleSystem* CharacterFactory::setUpFireButBetter(Odyssey::Scene* _sceneToAddTo)
//{
//	Odyssey::Entity* gFireBall = _sceneToAddTo->createEntity();
//	gFireBall->addComponent<Odyssey::Transform>();
//	Odyssey::Light* light = gFireBall->addComponent<Odyssey::Light>();
//	light->setLightType(Odyssey::LightType::Point);
//	light->setColor(0.0f, 0.75f, 0.75f);
//	light->setIntensity(0.5f);
//	light->setRange(10.0f);
//	light->setSpotAngle(0.0f);
//
//	Odyssey::ParticleSystem* fireButBetter = gFireBall->addComponent<Odyssey::ParticleSystem>();
//	fireButBetter->setTexture(Odyssey::TextureType::Diffuse, "Fire.jpg");
//	fireButBetter->setColor(DirectX::XMFLOAT3(0.0f, 0.75f, 0.75f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
//	fireButBetter->setLifetime(0.5f, 1.0f);
//	fireButBetter->setParticleCount(100, 150);
//	fireButBetter->setEmissionOverLifetime(125);
//	fireButBetter->setDuration(7.0);
//	fireButBetter->setSpeed(1.0f, 1.5f);
//	fireButBetter->setSize(1.0f, 1.5f);
//	fireButBetter->setLooping(false);
//	fireButBetter->setShape(Odyssey::SpherePS(0.0f, 0.0f, 0.0f, 0.05f));
//
//	// TODO: VERIFY CHANGE
//	gFireBall->setActive(false);
//
//	gFireBall->addComponent<ParticleMover>();
//	gFireBall->getComponent<ParticleMover>()->SetLifeTime(0.0f);
//	gFireBall->getComponent<ParticleMover>()->SetSpeed(1.0f);
//	return fireButBetter;
//}
//
//Odyssey::ParticleSystem* CharacterFactory::setUpFireStorm(Odyssey::Scene* _sceneToAddTo)
//{
//	Odyssey::Entity* gFireStorm = _sceneToAddTo->createEntity();
//	gFireStorm->addComponent<Odyssey::Transform>();
//	Odyssey::ParticleSystem* fireStorm = gFireStorm->addComponent<Odyssey::ParticleSystem>();
//	fireStorm->setTexture(Odyssey::TextureType::Diffuse, "Fire4.jpg");
//	fireStorm->setColor(DirectX::XMFLOAT3(0.8f, 0.5f, 0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
//	fireStorm->setLifetime(1.0f, 1.5f);
//	fireStorm->setParticleCount(100, 350);
//	fireStorm->setEmissionOverLifetime(150);
//	fireStorm->setDuration(2.0f);
//	fireStorm->setSpeed(2.5f, 3.5f);
//	fireStorm->setSize(4.0f, 4.0f);
//	fireStorm->setLooping(false);
//	fireStorm->setGravity(5.0f);
//	fireStorm->setShape(Odyssey::BoxPS(-1.0f, 0.5f, 0.0f, 13.5f, 1.0f, 5.0f));
//	return fireStorm;
//}
//
//Odyssey::ParticleSystem* CharacterFactory::setupBlood(Odyssey::Scene* _sceneToAddTo)
//{
//	Odyssey::Entity* hitEffect = _sceneToAddTo->createEntity();
//	hitEffect->addComponent<Odyssey::Transform>();
//	Odyssey::ParticleSystem* blood = hitEffect->addComponent<Odyssey::ParticleSystem>();
//	blood->setTexture(Odyssey::TextureType::Diffuse, "Particle.png");
//	blood->setColor(DirectX::XMFLOAT3(0.75f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
//	blood->setLifetime(0.5f, 0.75f);
//	blood->setParticleCount(150, 300);
//	blood->setEmissionOverLifetime(150);
//	blood->setDuration(1.25);
//	blood->setSpeed(5.0f, 7.5f);
//	blood->setSize(0.25f, 0.5f);
//	blood->setGravity(12.5f);
//	blood->setLooping(false);
//	blood->setShape(Odyssey::SpherePS(0.0f, 2.5f, 0.0f, 0.2f));
//	return blood;
//}