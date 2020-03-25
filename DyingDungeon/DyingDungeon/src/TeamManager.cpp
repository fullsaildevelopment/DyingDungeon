#include "TeamManager.h"
#include "TowerManager.h"
#include "CharacterFactory.h"
#include "Character.h"
#include "CharacterHUDElements.h"

TeamManager& TeamManager::getInstance()
{
	static TeamManager instance;
	return instance;
}

std::vector<Odyssey::Entity*> TeamManager::CreateEnemyTeam(int _index)
{
	// Clear the enemy team
	mEnemyTeam.clear();

	for (int i = 0; i < GameUIManager::getInstance().GetClickableUIElements().size(); i++)
	{
		Odyssey::Entity* clickObj = GameUIManager::getInstance().GetClickableUIElements()[i];
		clickObj->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>()->unregisterCallback("onMouseClick");
		clickObj->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>()->unregisterCallback("onMouseEnter");
		clickObj->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>()->unregisterCallback("onMouseExit");

		// Remove the enemy clickable rectangles because we will be recreating them
		if (i >= 3)
		{
			GameUIManager::getInstance().RemoveClickableCharacterObj(i);
			i--;
		}
	}

	// Destory the previous spot lights
	for (int i = 0; i < mEnemySpotLights.size(); i++)
	{
		//Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemySpotLights[i]));
	}
	// Clear spot light prefabs
	mEnemySpotLights.clear();

	// Clear the bars to animate before adding in the new enemies
	GameUIManager::getInstance().ClearBarsToUpdateList();
	
	// Get the list of enemies to create
	std::vector<EnemySetups> enemies = mEnemiesToCreate[_index];

	for (int i = 0; i < enemies.size(); i++)
	{
		EnemyType enemyType = enemies[i].pEnemyType;
		DirectX::XMVECTOR position = enemies[i].pPosition;
		DirectX::XMVECTOR rotation = enemies[i].pRotation;

		// Character we are about to create
		Odyssey::Entity* newCharacter = nullptr;
		// Character's HUD
		Odyssey::Entity* newHUD = nullptr;
		// Hud Id type
		CharacterFactory::HudID hudID;
		// Clickable Enemy UI
		GameUIManager::ClickableCharacterUI clickablePos;
		// Prefab we will need
		Odyssey::Entity* prefab;

		// Determine the enemy hud we need based on the enemy list
		// If there is only 1 player on the enemy team to start out with, make sure he is placed in the middle
		if (i == 0 && enemies.size() != 1)
		{
			hudID = CharacterFactory::HudID::EnemyLeft;
			clickablePos = GameUIManager::ClickableCharacterUI::EnemyLeft;
		}
		else if (i == 1 || enemies.size() == 1)
		{
			hudID = CharacterFactory::HudID::EnemyMiddle;
			clickablePos = GameUIManager::ClickableCharacterUI::EnemyMiddle;
		}
		else
		{
			hudID = CharacterFactory::HudID::EnemyRight;
			clickablePos = GameUIManager::ClickableCharacterUI::EnemyRight;
		}

		// Create the character based on the enum
		switch (enemyType)
		{
		case EnemyType::Skeleton:
			// Spawn Enemy
			prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Skeleton);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
			// Spawn Enemy HUD
			prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
			break;
		case EnemyType::Summoner:
			prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Summoner);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
			// Spawn Enemy HUD
			prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
			break;
		case EnemyType::MeleeDemon:
			prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::MeleeDemon);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
			// Spawn Enemy HUD
			prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
			break;
		case EnemyType::CasterDemon:
			prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::CasterDemon);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
			// Spawn Enemy HUD
			prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
			break;
		case EnemyType::Ganfaul:
			prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Ganfaul);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, position, rotation));
			// Spawn Enemy HUD
			prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
			Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, position, rotation));
			break;
		default:
			std::cout << "This enemy enum does not exist in the TeamManager.cpp CreateEnemyTeam function" << std::endl;
			break;
		}

		// Create the enemies clickable UI box
		Odyssey::Entity* clickableEnemyUI = nullptr;
		prefab = GameUIManager::getInstance().GetClickableUIPrefab(clickablePos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &clickableEnemyUI, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }));
		GameUIManager::getInstance().AddClickableElementToList(clickableEnemyUI);

		// Create the character's spot light
		Odyssey::Entity* spotLight = nullptr;
		prefab = CharacterFactory::getInstance().GetLightObjectPrefab(CharacterFactory::LightObjects::SpotLight);
		// Offset the y pos of the spotlight
		DirectX::XMVECTOR spotLightPos = { DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position) + 6.0f, DirectX::XMVectorGetZ(position), 1.0f };
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &spotLight, spotLightPos, DirectX::XMVECTOR{ 90.0f, 0.0f, 0.0f, 1.0f }));

		// Change spot light for different enemies
		if (enemyType == TeamManager::EnemyType::Skeleton)
		{
			spotLight->getComponent<Odyssey::Light>()->setIntensity(1.834f);
			spotLight->getComponent<Odyssey::Light>()->setRange(21.204f);
			spotLight->getComponent<Odyssey::Light>()->setSpotAngle(24.9f);
			spotLight->getComponent<Odyssey::Transform>()->setPosition(DirectX::XMVectorGetX(spotLightPos), 10.905f, DirectX::XMVectorGetZ(spotLightPos));
		}
		else if (enemyType == TeamManager::EnemyType::MeleeDemon || enemyType == TeamManager::EnemyType::CasterDemon)
		{
			spotLight->getComponent<Odyssey::Light>()->setIntensity(1.359f);
			spotLight->getComponent<Odyssey::Light>()->setRange(31.337f);
			spotLight->getComponent<Odyssey::Light>()->setSpotAngle(24.89f);
			spotLight->getComponent<Odyssey::Transform>()->setPosition(DirectX::XMVectorGetX(spotLightPos), 11.55f, DirectX::XMVectorGetZ(spotLightPos));
		}
		else if (enemyType == TeamManager::EnemyType::Ganfaul)
		{
			spotLight->getComponent<Odyssey::Light>()->setIntensity(1.41f);
			spotLight->getComponent<Odyssey::Light>()->setRange(23.934f);
			spotLight->getComponent<Odyssey::Light>()->setSpotAngle(24.525f);
			spotLight->getComponent<Odyssey::Transform>()->setPosition(DirectX::XMVectorGetX(spotLightPos), 9.48, DirectX::XMVectorGetZ(spotLightPos));
		}
		mEnemySpotLights.push_back(spotLight);

		// Create the impact indicator for the enemies
		Odyssey::Entity* impactIndicator = nullptr;
		DirectX::XMVECTOR impactIndicatorPosition = { DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position) + 0.1f, DirectX::XMVectorGetZ(position), 1.0f };
		prefab = CharacterFactory::getInstance().GetImpactIndicatorPrefab();
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &impactIndicator, impactIndicatorPosition, rotation));

		// Assign the impact indicator for the enemies
		impactIndicator->setActive(false);
		newCharacter->getComponent<Character>()->SetImpactIndicator(impactIndicator);

		// Assign the blood effect for the enemies
		prefab = CharacterFactory::getInstance().GetBloodEffectPrefab();
		newCharacter->getComponent<Character>()->SetBloodPrefab(prefab);

		// Set the hud to the init values
		//newHUD->getComponent< CharacterHUDElements>()->GetHealthBar()->setFill(1.0f);
		// Set the character's hud index number
		newCharacter->getComponent<Character>()->SetHudIndex(CharacterFactory::getInstance().GetCharacterHudIndex());
		// Increase the character index
		CharacterFactory::getInstance().IncreaseCharacterHUDIndex();
		// Add the new HUD to the list of HUDs
		GameUIManager::getInstance().AddHudToList(newHUD);

		// Set the elements of the character's HUD
		GameUIManager::getInstance().AssignCharacterHudElements(newCharacter->getComponent<Character>(), newHUD);

		// Add the character we created to the enemy team list
		mEnemyTeam.push_back(newCharacter);
	}

	// Set the enemy team to the createdEnemyTeam
	return mEnemyTeam;
}

void TeamManager::AddCharacterEnumToPlayerTeam(HeroType _characterHeroType)
{
	mPlayerTeamToCreate.push_back(_characterHeroType);
}

void TeamManager::AddCharacterToEnemyTeam(Odyssey::Entity* _characterToAdd)
{
	mEnemyTeam.push_back(_characterToAdd);
}

void TeamManager::UpdatePlayerTeam(std::vector<Odyssey::Entity*> _heroesToAdd)
{
	mPlayerTeamHeroComps.clear();

	for (int i = 0; i < _heroesToAdd.size(); i++)
	{
		mPlayerTeamHeroComps.push_back(*_heroesToAdd[i]->getComponent<HeroComponent>());
	}
}

HeroComponent TeamManager::GetUpdatedPlayerTeamHeroComp(int _index)
{
	return mPlayerTeamHeroComps[_index];
}

void TeamManager::ClearPlayerTeamEnumList()
{
	mPlayerTeamToCreate.clear();
	mPlayerTeamToCreate.resize(0);
}
