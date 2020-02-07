#pragma once
#include "Entity.h"
#include "Character.h"
#include "Scene.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Sprite2D.h"
#include "StatTracker.h"
#include "UIElement.h"

class GameUIManager
{
public:

	enum class CharacterType
	{
		Paladin, Mage
	};

	// This struct will contain all of the UI elements associated with the character HUD
	struct CharacterHUD
	{
		Odyssey::UICanvas* pCanvas;
		Odyssey::Sprite2D* pPortrait;
		Odyssey::Rectangle2D* pXpBar;
		Odyssey::Text2D* pCharacterName;
		Odyssey::Text2D* pLevelNumber;
		Odyssey::Text2D* pAttackNumber;
		Odyssey::Text2D* pDefenseNumber;
		Odyssey::Text2D* pSpeedNumber;
		Odyssey::Rectangle2D* pHealthBar;
		Odyssey::Text2D* pHealthNumber;
		Odyssey::Rectangle2D* pManaBar;
		Odyssey::Text2D* pManaNumber;
		Odyssey::Text2D* pTurnNumber;

		// Skills
		Odyssey::Sprite2D* pSkill1;
		Odyssey::Sprite2D* pSkill2;
		Odyssey::Sprite2D* pSkill3;
		Odyssey::Sprite2D* pSkill4;
	};

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
	void CreateBattleLog(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	// Toggle canvases on and off
	void ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive);

	// Creation of the main menu
	void CreateMainMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	// Creation of the tower select menu
	void CreateTowerSelectMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	// Creation of the team select menu
	void CreateTeamSelectMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	// Creation of the pause menu
	void CreatePauseMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	// Creation of the stats menu
	void CreateStatsMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);

	// TODO: M3B1 ONLY REFACTOR LATER
	void CreateCreditsMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);
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
	 
	// Create The UI portraits for the characters
	Odyssey::UICanvas* CreateCharacterPortrait(DirectX::XMFLOAT2 _hudPosition, DirectX::XMFLOAT2 _hpPopupPosition, std::wstring _imageName, Odyssey::Entity* _gameObjectToAddTo, Character* owner);
	
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
	std::shared_ptr<Odyssey::Entity> GetMainMenu() { return mMainMenu; }
	Odyssey::Sprite2D* GetApeBackground() { return mApeBackground; }
	Odyssey::Sprite2D* GetAnimatedLaser() { return mAnimatedLaser; }
	Odyssey::Text2D* GetNewGameText() { return mNewGameText; }
	Odyssey::Text2D* GetStatsText() { return mStatsText; }
	// TODO: M3B1 ONLY REFACTOR LATER
	Odyssey::Text2D* GetCreditsText() { return mCreditsText; }
	Odyssey::Text2D* GetExitGameText() { return mExitGameText; }
	// TODO: M3B1 ONLY END

	// Get the tower select menu
	std::shared_ptr<Odyssey::Entity> GetTowerSelectMenu() { return mTowerSelectMenu; }
	// Get the sprite buttons from the tower select menu
	std::vector<Odyssey::Sprite2D*> GetDoorSprites() { return mDoorImages; }
	// Get the tower info canvas
	Odyssey::UICanvas* GetTowerInfoCanvas() { return mTowerInfoCanvas; }

	// Get the team select menu
	std::shared_ptr<Odyssey::Entity> GetTeamSelectMenu() { return mTeamSelectMenu; }
	// Get the arrow sprite when selecting a team
	std::vector<Odyssey::Sprite2D*> GetTeamSelectionArrows() { return mTeamSelectionArrows; }
	// Get the text slots for the names
	std::vector<Odyssey::Text2D*> GetNameSlots() { return mNameSlots; }
	// Get the enter battle button sprite
	Odyssey::Sprite2D* GetEnterBattleButton() { return mEnterBattleButton; }

	// Get the pause menu
	std::shared_ptr<Odyssey::Entity> GetPauseMenu() { return mPauseMenu; }
	// Get the options menu
	std::shared_ptr<Odyssey::Entity> GetOptionsMenu() { return mOptionsMenu; }
	// Get the stats menu
	std::shared_ptr<Odyssey::Entity> GetStatsMenu() { return mStatsMenu; }
	// Get the rectangle buttons from the pause menu
	Odyssey::Rectangle2D* GetResumeButton() { return mResumeBackground; }
	Odyssey::Rectangle2D* GetOptionsButton() { return mOptionsBackground; }
	Odyssey::Rectangle2D* GetMainMenuButton() { return mMainMenuBackground; }

	// Get the list of all of the character hud canvases
	std::vector<std::shared_ptr<CharacterHUD>> GetCharacterHuds() { return mCharacterHudList; }

	//Setters
	void SetPauseMenu(std::shared_ptr<Odyssey::Entity> _pauseMenu) { mPauseMenu = _pauseMenu; }

	void SetOptionsMenu(std::shared_ptr<Odyssey::Entity> _optionsMenu) { mOptionsMenu = _optionsMenu; }

	void SetStatsMenu(std::shared_ptr<Odyssey::Entity> _statsMenu) { mStatsMenu = _statsMenu; }

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
	std::shared_ptr<Odyssey::Entity> mBattleLog;
	std::vector<Odyssey::Text2D*> mBattleLogVec;
	std::vector<Odyssey::Sprite2D*> mCombatCasterIcons;
	std::vector<Odyssey::Sprite2D*> mCombatSkillIcons;
	std::vector<Odyssey::Sprite2D*> mCombatTargetIcons;

	// Battle Log Colors
	DirectX::XMFLOAT3 newCombatLogColor;

	// Menu Entities
	std::shared_ptr<Odyssey::Entity> mMainMenu;
	std::shared_ptr<Odyssey::Entity> mTowerSelectMenu;
	std::shared_ptr<Odyssey::Entity> mTeamSelectMenu;
	std::shared_ptr<Odyssey::Entity> mPauseMenu;
	std::shared_ptr<Odyssey::Entity> mOptionsMenu;
	std::shared_ptr<Odyssey::Entity> mStatsMenu;
	std::shared_ptr<Odyssey::Entity> mCreditsMenu;

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
	std::vector<std::shared_ptr<CharacterHUD>> mCharacterHudList;
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
	void CreateOptionsMenu(std::shared_ptr<Odyssey::Scene> _sceneToAddTo);
	void OptionsBackButton();
	void DecreaseVolume();
	void IncreaseVolume();

	// Skill Icon Creation Fucntions
	void SetupSkillIcons(Odyssey::Entity* _objToAddTo, Character* _newCharacter, DirectX::XMFLOAT2 _hudPosition, std::shared_ptr<CharacterHUD> _newHud);
	void SetupSkillHover(Odyssey::UICanvas* canvas, std::wstring character, std::wstring skillName, std::wstring icon, std::wstring manaCost, std::wstring description);
	void SetupHpPopup(Odyssey::Entity* _objToAddTo, DirectX::XMFLOAT2 _hpPopupPosition);

	// TODO: REFACTOR THIS LATER
	Odyssey::UICanvas* CreatePopup(Odyssey::Entity* entity);
};
