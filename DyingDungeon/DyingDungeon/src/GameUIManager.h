#pragma once
#include "Entity.h"
#include "Character.h"
#include "Scene.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Sprite2D.h"

class GameUIManager
{
public:

	enum class CharacterType
	{
		Paladin, Mage
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
	 
	// Create The UI portraits for the characters
	void CreateCharacterPortrait(float anchorX, float anchorY, LPCWSTR _imageName, std::shared_ptr<Odyssey::Entity> _gameObject, Character* owner);

	//Getters
	// Get battle log text
	Odyssey::Text2D* GetBattleLogText() { return mBattleLogText; }

	// Get the main menu
	std::shared_ptr<Odyssey::Entity> GetMainMenu() { return mMainMenu; }
	Odyssey::Sprite2D* GetTeamLogo() { return mTeamLogo; }
	Odyssey::Text2D* GetNewGameText() { return mNewGameText; }

	// Get the tower select menu
	std::shared_ptr<Odyssey::Entity> GetTowerSelectMenu() { return mTowerSelectMenu; }
	// Get the sprite buttons from the tower select menu
	Odyssey::Sprite2D* GetDoorButton() { return mDoorImage; }

	// Get the team select menu
	std::shared_ptr<Odyssey::Entity> GetTeamSelectMenu() { return mTeamSelectMenu; }
	// Get the team member slot 1, 2, or 3
	Odyssey::Sprite2D* GetTeamMemberSlot(int _teamMemberSlot) 
	{
		switch (_teamMemberSlot)
		{
			case 1: return firstTeamMemberSlot; break;
			case 2: return secondTeamMemberSlot; break;
			case 3: return thirdTeamMemberSlot; break;
		}
	}
	// Get the sprite of which character you selected
	Odyssey::Sprite2D* GetCharacterSelectImage(CharacterType _type)
	{
		switch (_type)
		{
		case CharacterType::Paladin: return paladinSelectionImage; break;
		case CharacterType::Mage: return mageSelectionImage; break;
		}
	}

	// Get the pause menu
	std::shared_ptr<Odyssey::Entity> GetPauseMenu() { return mPauseMenu; }
	// Get the options menu
	std::shared_ptr<Odyssey::Entity> GetOptionsMenu() { return mOptionsMenu; }
	// Get the rectangle buttons from the pause menu
	Odyssey::Rectangle2D* GetResumeButton() { return mResumeBackground; }
	Odyssey::Rectangle2D* GetOptionsButton() { return mOptionsBackground; }
	Odyssey::Rectangle2D* GetMainMenuButton() { return mMainMenuBackground; }

	//Setters
	void SetPauseMenu(std::shared_ptr<Odyssey::Entity> _pauseMenu) { mPauseMenu = _pauseMenu; }

	void SetOptionsMenu(std::shared_ptr<Odyssey::Entity> _optionsMenu) { mOptionsMenu = _optionsMenu; }

	void SetScreenWidthAndHeight(UINT _width, UINT _height) { screenWidth = _width; screenHeight = _height; }

	/*
	 * Set the new battle text
	 * @param[in] wstring newText to be added and bool concat wether or not to concat.
	 * @return void
	*/
	void SetBattleLogText(std::wstring text, bool concat);
	
private: // Varibales

	// Battle Log Object
	std::shared_ptr<Odyssey::Entity> mBattleLog;
	std::vector<Odyssey::Text2D*> mBattleLogVec;
	Odyssey::Text2D* mBattleLogText;


	// Menu Entities
	std::shared_ptr<Odyssey::Entity> mMainMenu;
	std::shared_ptr<Odyssey::Entity> mTowerSelectMenu;
	std::shared_ptr<Odyssey::Entity> mTeamSelectMenu;
	std::shared_ptr<Odyssey::Entity> mPauseMenu;
	std::shared_ptr<Odyssey::Entity> mOptionsMenu;

	// Main Menu Items
	Odyssey::Sprite2D* mTeamLogo;
	Odyssey::Text2D* mNewGameText;

	// Tower Menu Items
	Odyssey::Text2D* mTowerSelectTitle;
	Odyssey::Sprite2D* mDoorImage;

	// Team Menu Items
	// Team member slots
	Odyssey::Sprite2D* firstTeamMemberSlot;
	Odyssey::Sprite2D* secondTeamMemberSlot;
	Odyssey::Sprite2D* thirdTeamMemberSlot;
	// Rectangles around the characters
	Odyssey::Sprite2D* paladinSelectionImage;
	Odyssey::Sprite2D* mageSelectionImage;

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

	// Colors
	DirectX::XMFLOAT4 mHealthBarColor = { 0.0f, 180.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 mBigHealthTextColor = { 180.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 mManaBarColor = { 0.0f, 180.0f, 180.0f, 1.0f };
	DirectX::XMFLOAT4 mTurnOrderColor = { 255.0f, 210.0f, 0.0f, 1.0f };

	// Vectors

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
};
