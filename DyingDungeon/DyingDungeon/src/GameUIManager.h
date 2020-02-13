#pragma once
#include "Entity.h"
#include "Character.h"
#include "Scene.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Sprite2D.h"
#include "StatTracker.h"
#include "UIElement.h"
#include "CharacterHUDElements.h"

class GameUIManager
{
public:

	enum class CharacterType
	{
		Paladin, Mage
	};

	// This struct will contain all of the UI elements associated with the character HUD
	//struct CharacterHUD
	//{
	//	Odyssey::UICanvas* pCanvas;
	//	Odyssey::Sprite2D* pPortrait;
	//	Odyssey::Rectangle2D* pXpBar;
	//	Odyssey::Text2D* pCharacterName;
	//	Odyssey::Text2D* pLevelNumber;
	//	Odyssey::Text2D* pAttackNumber;
	//	Odyssey::Text2D* pDefenseNumber;
	//	Odyssey::Text2D* pSpeedNumber;
	//	Odyssey::Rectangle2D* pHealthBar;
	//	Odyssey::Text2D* pHealthNumber;
	//	Odyssey::Rectangle2D* pManaBar;
	//	Odyssey::Text2D* pManaNumber;
	//	Odyssey::Text2D* pTurnNumber;
	//
	//	// Skills
	//	Odyssey::Sprite2D* pSkill1;
	//	Odyssey::UICanvas* pSkill1Canvas;
	//	Odyssey::Sprite2D* pSkill2;
	//	Odyssey::UICanvas* pSkill2Canvas;
	//	Odyssey::Sprite2D* pSkill3;
	//	Odyssey::UICanvas* pSkill3Canvas;
	//	Odyssey::Sprite2D* pSkill4;
	//	Odyssey::UICanvas* pSkill4Canvas;
	//
	//	// Status Effects
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//};

	// This struct will hold the elements needed in order to animate the health and mana bars
	struct AnimatingBar
	{
		Odyssey::Rectangle2D* pBar = nullptr;
		Odyssey::Text2D* pBarText = nullptr;
		float pMaxValue;
		float pCurrValue; // This will change in the update function
		float pNewValue;

		// Used for checking while updating
		bool pTookDamage;
	};

public: // Singleton pattern
		/**
		 *	Get the singleton instance of the game ui manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the game ui manager.
		 */
	static GameUIManager& getInstance();
	~GameUIManager() { }

private: // Singleton pattern
	GameUIManager() { }

public: // Functions

	// Create the battle log UI 
	void CreateBattleLog(Odyssey::Scene* _sceneToAddTo);

	// Toggle canvases on and off
	void ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive);

	// Creation of the main menu
	void CreateMainMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the tower select menu
	void CreateTowerSelectMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the team select menu
	void CreateTeamSelectMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the pause menu
	void CreatePauseMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the stats menu
	void CreateStatsMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// TODO: M3B1 ONLY REFACTOR LATER
	void CreateCreditsMenuCanvas(Odyssey::Scene* _sceneToAddTo);
	// TODO: M3B1 ONLY END

	void ShowStatsMenu();

	// TODO: M3B1 ONLY REFACTOR LATER
	void ShowCreditsMenu();
	void HideCreditsMenu();
	// TODO: M3B1 ONLY END

	void StatsMenuPrevTurn();

	void StatsMenuNextTurn();

	void StatsMenuPrevLevel();

	void StatsMenuNextLevel();

	void StatsMenuNextRound();

	void StatsMenuPrevRound();

	void HideStatsMenu();
	 
	// Create hero character portrait
	void CreateHeroHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition);

	// Create enemy character portrait
	void CreateEnemyHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition);

	// Create Hp Popup
	Odyssey::UICanvas* CreateHpPopup(Odyssey::Entity* _objToAddTo);

	// Create The UI portraits for the characters
	//Odyssey::UICanvas* CreateCharacterPortrait(DirectX::XMFLOAT2 _hudPosition, DirectX::XMFLOAT2 _hpPopupPosition, std::wstring _imageName, Odyssey::Entity* _gameObjectToAddTo, Character* owner);
	
	// Character info popup for team selection screen
	Odyssey::UICanvas* SetupInfoPopup(Odyssey::Entity* _objToAddTo, Character* _character, DirectX::XMFLOAT2 _popupPosition);

	// Add HUD to the character hud list
	void AddHudToList(Odyssey::Entity* _newHud);
	// Add character health and mana bars to update list in order for the bars to be animated
	void AddCharacterHpBarsToUpdateList(Character* _currCharacter, float _previousHpAmount, float _newHpAmount);
	void AddCharacterMpBarsToUpdateList(Character* _currCharacter, float _previousMpAmount, float _newMpAmount);
	// Update health bar
	void UpdateCharacterBars(float _deltaTime);
	// Add character's health popup to update list in order from them to be updated
	void AddHpPopupToUpdateList(Character* _currCharacter, bool _tookDamage, float _changeInHP);
	// Update health popups
	void UpdateCharacterHealthPopups(float _deltaTime);
	// UPdate turn number
	void UpdateCharacterTurnNumber(Character* _currCharacter, int _turnNumber);

	//Updates
	void UpdateStatsMenu();

	// Update Combat Log
	void UpdateCombatLogIcons(Character* caster, Character* target, Skills* skill);
	void UpdateCombatLogText(float dmg);
	void ClearCombatLog();

	//Getters
	// Get battle log text
	Odyssey::Text2D* GetBattleLogText() { return mBattleLogText; }

	// Get Combat Log Canvas
	Odyssey::UICanvas* GetCombatLogCanvas() { return mCombatLogCanvas; }

	// Get the main menu
	Odyssey::Entity* GetMainMenu() { return mMainMenu; }
	Odyssey::Sprite2D* GetApeBackground() { return mApeBackground; }
	Odyssey::Sprite2D* GetAnimatedLaser() { return mAnimatedLaser; }
	Odyssey::Text2D* GetNewGameText() { return mNewGameText; }
	Odyssey::Text2D* GetStatsText() { return mStatsText; }
	// TODO: M3B1 ONLY REFACTOR LATER
	Odyssey::Text2D* GetCreditsText() { return mCreditsText; }
	Odyssey::Text2D* GetExitGameText() { return mExitGameText; }
	// TODO: M3B1 ONLY END

	// Get the tower select menu
	Odyssey::Entity* GetTowerSelectMenu() { return mTowerSelectMenu; }
	// Get the sprite buttons from the tower select menu
	std::vector<Odyssey::Sprite2D*> GetDoorSprites() { return mDoorImages; }
	// Get the tower info canvas
	Odyssey::UICanvas* GetTowerInfoCanvas() { return mTowerInfoCanvas; }

	// Get the team select menu
	Odyssey::Entity* GetTeamSelectMenu() { return mTeamSelectMenu; }
	// Get the arrow sprite when selecting a team
	std::vector<Odyssey::Sprite2D*> GetTeamSelectionArrows() { return mTeamSelectionArrows; }
	// Get the text slots for the names
	std::vector<Odyssey::Text2D*> GetNameSlots() { return mNameSlots; }
	// Get the show info button sprites
	std::vector<Odyssey::Sprite2D*> GetShowInfoButtons() { return mShowInfoButtons; }
	// Get the enter battle button sprite
	Odyssey::Sprite2D* GetEnterBattleButton() { return mEnterBattleButton; }

	// Get the pause menu
	Odyssey::Entity* GetPauseMenu() { return mPauseMenu; }
	// Get the options menu		   
	Odyssey::Entity* GetOptionsMenu() { return mOptionsMenu; }
	// Get the stats menu		   
	Odyssey::Entity* GetStatsMenu() { return mStatsMenu; }
	// Get the rectangle buttons from the pause menu
	Odyssey::Rectangle2D* GetResumeButton() { return mResumeBackground; }
	Odyssey::Rectangle2D* GetOptionsButton() { return mOptionsBackground; }
	Odyssey::Rectangle2D* GetMainMenuButton() { return mMainMenuBackground; }

	// Get the list of all of the character hud canvases
	std::vector<Odyssey::Entity*> GetCharacterHuds() { return mCharacterHudList; }

	//Setters
	void SetPauseMenu(Odyssey::Entity* _pauseMenu) { mPauseMenu = _pauseMenu; }

	void SetOptionsMenu(Odyssey::Entity* _optionsMenu) { mOptionsMenu = _optionsMenu; }

	void SetStatsMenu(Odyssey::Entity* _statsMenu) { mStatsMenu = _statsMenu; }

	void SetScreenWidthAndHeight(UINT _width, UINT _height) { screenWidth = _width; screenHeight = _height; }

	/*
	 * Set the new battle text
	 * @param[in] wstring newText to be added and bool concat wether or not to concat.
	 * @return void
	*/
	void SetBattleLogText(std::wstring text, bool concat);
	
private: // Varibales

	// Battle Log Object
	Odyssey::UICanvas* mCombatLogCanvas;
	Odyssey::Text2D* mBattleLogText;
	Odyssey::Entity* mBattleLog;
	std::vector<Odyssey::Text2D*> mBattleLogVec;
	std::vector<Odyssey::Sprite2D*> mCombatCasterIcons;
	std::vector<Odyssey::Sprite2D*> mCombatSkillIcons;
	std::vector<Odyssey::Sprite2D*> mCombatTargetIcons;

	// Battle Log Colors
	DirectX::XMFLOAT3 newCombatLogColor;

	// Menu Entities
	Odyssey::Entity* mMainMenu;
	Odyssey::Entity* mTowerSelectMenu;
	Odyssey::Entity* mTeamSelectMenu;
	Odyssey::Entity* mPauseMenu;
	Odyssey::Entity* mOptionsMenu;
	Odyssey::Entity* mStatsMenu;
	Odyssey::Entity* mCreditsMenu;

	// Main Menu Items
	Odyssey::Sprite2D* mApeBackground;
	Odyssey::Sprite2D* mAnimatedLaser;
	Odyssey::Text2D* mNewGameText;
	Odyssey::Text2D* mStatsText;
	// TODO: M3B1 ONLY REFACTOR LATER
	Odyssey::Text2D* mCreditsText;
	Odyssey::Text2D* mExitGameText;
	Odyssey::Text2D* mCreditsBackText;
	// TODO: M3B1 ONLY END

	// Tower Menu Items
	Odyssey::Text2D* mTowerSelectTitle;
	std::vector<Odyssey::Sprite2D*> mDoorImages;
	Odyssey::UICanvas* mTowerInfoCanvas;

	// Team Menu Items
	// Enter Battle Button
	Odyssey::Sprite2D* mEnterBattleButton;
	// The arrow sprites
	std::vector<Odyssey::Sprite2D*> mTeamSelectionArrows;
	// The name text slots
	std::vector<Odyssey::Text2D*> mNameSlots;
	// The show info buttons
	std::vector<Odyssey::Sprite2D*> mShowInfoButtons;

	// Pause Menu Items
	Odyssey::Rectangle2D* mBlackBackground;
	Odyssey::Rectangle2D* mSmallerBlackBackground;
	Odyssey::Text2D* mPauseTitle;
	Odyssey::Rectangle2D* mResumeBackground;
	Odyssey::Text2D* mResumeText;
	Odyssey::Rectangle2D* mOptionsBackground;
	Odyssey::Text2D* mOptionsText;
	Odyssey::Rectangle2D* mMainMenuBackground;
	Odyssey::Text2D* mMainMenuText;

	// Options Menu Items
	Odyssey::Text2D* mOptionsTitle;
	Odyssey::Text2D* mVolumeText;
	Odyssey::Rectangle2D* mVolumeBar;
	Odyssey::Sprite2D* mPlusImage;
	Odyssey::Sprite2D* mMinusImage;
	Odyssey::Text2D* mBackButtonText;

	//Stats Menu Items
	unsigned int mStatMenuCurrentLevel = 1;
	unsigned int mStatMenuCurrentRound = 1;
	unsigned int mStatMenuCurrentTurn = 1;
	Odyssey::Text2D* mStatsBackButtonText;
	Odyssey::Sprite2D* mStatsPrevButtonTurn;
	Odyssey::Sprite2D* mStatsNextButtonTurn;
	Odyssey::Sprite2D* mStatsPrevButtonRound;
	Odyssey::Sprite2D* mStatsNextButtonRound;
	Odyssey::Sprite2D* mStatsPrevButtonLevel;
	Odyssey::Sprite2D* mStatsNextButtonLevel;

	// Colors
	DirectX::XMFLOAT4 mTextColor = { 255.0f, 255.0f, 255.0f, 1.0f };
	DirectX::XMFLOAT4 mHealthBarColor = { 0.0f, 180.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 mManaBarColor = { 0.0f, 180.0f, 180.0f, 1.0f };
	DirectX::XMFLOAT4 mTurnOrderColor = { 255.0f, 210.0f, 0.0f, 1.0f };

	// Vectors
	// List of the HUDs
	std::vector<Odyssey::Entity*> mCharacterHudList;
	std::vector<Odyssey::Text2D*> mCharacterHpPopupList;
	std::vector<Odyssey::Rectangle2D*> mCharacterBarsList;

	// List of HP popups I need to update
	std::vector<Odyssey::Text2D*> mUpdateHpPopupList;

	// List of character bars I need to update
	std::vector<std::shared_ptr<AnimatingBar>> mUpdateCharacterBarsList;

	// Queues

	// Entities

	// Ints
	UINT screenWidth = 0;
	UINT screenHeight = 0;
	UINT battleTextWidth = 0;
	UINT battleTextHeight = 0;

	// Floats

	// Bools

private: // Functions
	void CreateOptionsMenu(Odyssey::Scene* _sceneToAddTo);
	void OptionsBackButton();
	void DecreaseVolume();
	void IncreaseVolume();

	// Skill Icon Creation Fucntions
	void SetupSkillIcons(Odyssey::Entity* _objToAddTo, DirectX::XMFLOAT2 _hudPosition);
	void SetupSkillHover(CharacterHUDElements::SkillPopup* _skillPopup, DirectX::XMFLOAT2 _position);
	//void SetupStatusEffects(Odyssey::Entity* _objToAddTo, Character* _newCharacter, DirectX::XMFLOAT2 _hudPosition, Odyssey::Entity* _newHud);

	// TODO: REFACTOR THIS LATER
	Odyssey::UICanvas* CreatePopup(Odyssey::Entity* entity);
};
