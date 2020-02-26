#pragma once
#include "OdysseyEngine.h"
#include "RenderManager.h"
#include "Scene.h"
#include "Application.h"
#include <map>


class CharacterFactory
{
public:
	// Singleton pattern
		/**
		 *	Get the singleton instance of the game ui manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the game ui manager.
		 */
	static CharacterFactory& getInstance();
	~CharacterFactory() { }
private: // Singleton pattern
	CharacterFactory() {}

public:
	enum CharacterOptions
	{
		Paladin,
		Mage,
		Bard,
		Warrior,
		Monk,
		Skeleton,
		Summoner,
		MeleeDemon,
		CasterDemon,
		Ganfaul
	};

	enum HudID
	{
		HeroLeft,
		HeroMiddle,
		HeroRight,
		EnemyLeft,
		EnemyMiddle,
		EnemyRight,
	};

	enum SkillHoverID
	{
		LeftHUD,
		MiddleHUD,
		RightHUD,
	};

public: // Functions

	void initialize(Odyssey::Application* _application);

	//Odyssey::Entity* CreateCharacter(CharacterOptions _characterToCreate, std::wstring _characterName, DirectX::XMVECTOR _position, DirectX::XMVECTOR _rotation, DirectX::XMFLOAT2 _hudPosition, bool _showHUD, DirectX::XMFLOAT2 _hpPopupPosition, Odyssey::Scene* _gameScene = nullptr);

	//Increase the character hud index
	void IncreaseCharacterHUDIndex() { mCharacterHudIndex++; }
	// Get the character hud index
	unsigned int GetCharacterHudIndex() { return mCharacterHudIndex; }

	//Getters
	Odyssey::Entity* GetCharacterPrefab(CharacterOptions _characterType);
	Odyssey::Entity* GetHUDPrefab(HudID _hudToGet);
	Odyssey::Entity* GetSkillHoverHUDPrefab(SkillHoverID _hoverHudToGet);
	Odyssey::Entity* GetImpactIndicatorPrefab();
	Odyssey::Entity* GetTurnIndicatorPrefab();
	Odyssey::Entity* GetBloodEffectPrefab();

	//Setters

private: // Varibales

	// Scene

	// Vectors

	// Queues

	// Entitys

	// Ints
	unsigned int mCharacterHudIndex = 0;

	// Floats

	// Bools


private: // Functions
	// Create character prefabs
	Odyssey::Entity* CreateCharacterPrefab(CharacterOptions _characterToCreate);

	// Create hero hud
	Odyssey::Entity* CreateHeroHudPrefab(DirectX::XMFLOAT2 _hudPosition);

	// Create enemy hud
	Odyssey::Entity* CreateEnemyHudPrefab(DirectX::XMFLOAT2 _hudPosition);

	// Create skill hover hud
	Odyssey::Entity* CreateSkillHoverHudPrefab(DirectX::XMFLOAT2 _hudPosition);

	// Create hp popup
	Odyssey::Entity* CreateHpPopupPrefab();

	// Create impact indicator
	Odyssey::Entity* CreateImpactIndicatorPrefab();

	// Create turn indicator
	Odyssey::Entity* CreateTurnIndicatorPrefab();

	// Create blood particle effect
	Odyssey::Entity* CreateBloodEffectPrefab();

	// Particle Creation Functions
	Odyssey::ParticleSystem* setUpFireButBetter(Odyssey::Scene* _sceneToAddTo);
	Odyssey::ParticleSystem* setUpFireStorm(Odyssey::Scene* _sceneToAddTo);
	Odyssey::ParticleSystem* setupBlood(Odyssey::Scene* _sceneToAddTo);

	// Character Map
	std::map<CharacterOptions, Odyssey::Entity*> mCharacterPrefabMap;

	// HUD Map
	std::map<HudID, Odyssey::Entity*> mHudPrefabMap;

	// Skill Hover Map
	std::map<SkillHoverID, Odyssey::Entity*> mSkillHoverPrefabMap;

	// Hp Popup 
	Odyssey::Entity* mHpPopupPrefab = nullptr;

	// Impact Indicator
	Odyssey::Entity* mImpactIndicatorPrefab = nullptr;

	// Turn Indicator
	Odyssey::Entity* mTurnIndicatorPrefab = nullptr;

	// Bleed Effect
	Odyssey::Entity* mBloodEffect = nullptr;

	// Application
	Odyssey::Application* mApplication = nullptr;
};

