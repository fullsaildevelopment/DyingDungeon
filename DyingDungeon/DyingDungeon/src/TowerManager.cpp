#include "TowerManager.h"
#include "Transform.h"
#include "Character.h"
#include "HeroComponent.h"
#include "InputManager.h"
#include "EventManager.h"
#include "StatusEvents.h"
#include "GameUIManager.h"
#include "StatusEvents.h"
#include "UICanvas.h"
#include "RedAudioManager.h"
#include "TeamManager.h"
#include "CharacterFactory.h"
#include "CharacterHUDElements.h"
#include "SkillHUDElements.h"
#include "SkillHoverComponent.h"
#include "LoadingScreenController.h"

CLASS_DEFINITION(Component, TowerManager)
std::shared_ptr<Odyssey::Component> TowerManager::clone() const
{
	return std::make_shared<TowerManager>(*this);
}

TowerManager::~TowerManager()
{
	DestroyBattleInstance();
}

void TowerManager::initialize()
{
	// Reset cheat code bools
	mUsedBossCheatCode = false;

	// The tower will not be paused on start up
	mIsPaused = false;

	GameUIManager::getInstance().ClearClickableCharacterList();

	// Create the player team
	CreateThePlayerTeam();

	if (mCurrentLevel == mNumberOfLevels)
		mIsBossScene = true;
	else
		mIsBossScene = false;

	for (int i = 0; i < TeamManager::getInstance().GetUpdatedPlayerTeam().size(); i++)
	{
		HeroComponent* savedHeroComp = &TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i);
		// Reassign all of the properties
		mPlayerTeam[i]->getComponent<HeroComponent>()->SetBloodPrefab(savedHeroComp->GetBloodPrefab());
		mPlayerTeam[i]->getComponent<HeroComponent>()->SetHP(savedHeroComp->GetHP());
		mPlayerTeam[i]->getComponent<HeroComponent>()->SetImpactIndicator(savedHeroComp->GetInpactIndicator());
		mPlayerTeam[i]->getComponent<HeroComponent>()->SetMana(savedHeroComp->GetMana());
		mPlayerTeam[i]->getComponent<HeroComponent>()->SetProvoked(savedHeroComp->GetProvoked());
		mPlayerTeam[i]->getComponent<HeroComponent>()->SetState(savedHeroComp->GetState());

		CharacterHUDElements* hudElements = GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<CharacterHUDElements>();
		hudElements->GetHealthBar()->setFill(TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetHP() / TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetMaxHP());
		hudElements->GetHealthNumber()->setText(std::to_wstring((int)TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetHP()) + L"/" + std::to_wstring((int)TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetMaxHP()));
		hudElements->GetManaBar()->setFill(TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetMana() / TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetMaxMana());
		hudElements->GetManaNumber()->setText(std::to_wstring((int)TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetMana()) + L"/" + std::to_wstring((int)TeamManager::getInstance().GetUpdatedPlayerTeamHeroComp(i).GetMaxMana()));

		if (mPlayerTeam[i]->getComponent<HeroComponent>()->GetState() == STATE::DEAD)
		{
			mPlayerTeam[i]->getComponent<Odyssey::Animator>()->playClip("Dead");
			GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->setActive(false);
		}

		mIsBossScene = true;
	}

	// Create a Battle when we set up the tower !!THIS WILL BE TEMPORARY!!
	if (!mIsTutorial)
		CreateBattleInstance();
	else
		CreateTutorialInstance();

	// Set the pause menu button callbacks
	if (!mIsBossScene)
	{
		GameUIManager::getInstance().GetResumeButton(0)->registerCallback("onMouseClick", this, &TowerManager::TogglePauseMenu);
		GameUIManager::getInstance().GetOptionsVolumeButton(0)->registerCallback("onMouseClick", this, &TowerManager::ShowOptionsMenu);
		GameUIManager::getInstance().GetOptionsControlsButton(0)->registerCallback("onMouseClick", this, &TowerManager::ShowControlScreen);
		GameUIManager::getInstance().GetMainMenuButton(0)->registerCallback("onMouseClick", this, &TowerManager::GoToMainMenu);
	}
	else
	{
		GameUIManager::getInstance().GetResumeButton(1)->registerCallback("onMouseClick", this, &TowerManager::TogglePauseMenu);
		GameUIManager::getInstance().GetOptionsVolumeButton(1)->registerCallback("onMouseClick", this, &TowerManager::ShowOptionsMenu);
		GameUIManager::getInstance().GetOptionsControlsButton(1)->registerCallback("onMouseClick", this, &TowerManager::ShowControlScreen);
		GameUIManager::getInstance().GetMainMenuButton(1)->registerCallback("onMouseClick", this, &TowerManager::GoToMainMenu);
	}

	TeamManager::getInstance().ClearUpdatedPlayerTeam();
}

void TowerManager::update(double deltaTime)
{
	if (!mIsBossScene)
	{
		// Always look for the pause input button
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::P) &&
			!GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>()->isActive() &&
			!GameUIManager::getInstance().GetOptionsMenu(0)->getComponent<Odyssey::UICanvas>()->isActive())
		{
			// Turn the pause menu either on or off
			TogglePauseMenu();
		}
	}
	else
	{
		// Always look for the pause input button
		if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::P) &&
			!GameUIManager::getInstance().GetPauseMenu(1)->getComponent<Odyssey::UICanvas>()->isActive() &&
			!GameUIManager::getInstance().GetOptionsMenu(1)->getComponent<Odyssey::UICanvas>()->isActive())
		{
			// Turn the pause menu either on or off
			TogglePauseMenu();
		}
	}


	// Toggles the combat canvas
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Tab))
	{
		Odyssey::UICanvas* combatCanvas = GameUIManager::getInstance().GetCombatLogCanvas();

		GameUIManager::getInstance().ToggleCanvas(combatCanvas, !combatCanvas->isActive());
	}

	// Go straight to the BOSS when F3 is hit
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::F3))
	{
		// Destroy the current battle instance
		DestroyBattleInstance();
		// Set the game to in rewards 
		mTowerState = IN_REWARDS;
		// Set the tower level to the last level which is the boss
		mCurrentLevel = mNumberOfLevels;
		// Set the cheat code bool
		mUsedBossCheatCode = true;
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::M))
	{
		if (!RedAudioManager::Instance().isMuted())
		{
			RedAudioManager::Instance().Mute();
		}
		else
		{
			RedAudioManager::Instance().Unmute();
		}
	}

	// Don't update unless the game is not paused
	if (!mIsPaused)
	{
		// Update the health popups
		//GameUIManager::getInstance().UpdateCharacterHealthPopups(deltaTime);
		// Update the UI bars
		GameUIManager::getInstance().UpdateCharacterBars(deltaTime);

		if (mIsTutorial && Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
			GameUIManager::getInstance().TutorialCallBack();

		// If we are in battle, Update the battle
		if (GetTowerState() == IN_BATTLE)
		{
			int result = mCurrentBattle->UpdateBattle();

			// If the result of the updated battle was DESTROY, destory the current battle instance
			if (result == mCurrentBattle->PLAYER_TEAM_DIED || result == mCurrentBattle->DESTORY)
			{
				// Destroy the battle instance
				DestroyBattleInstance();

				if (mIsTutorial)
				{
					SetTowerState(NOT_IN_BATTLE);
					mIsTutorial = false;
					GameUIManager::getInstance().EndTutorial();
					GoToMainMenu();
				}
				else
				{
					SetTowerState(IN_REWARDS);
					Odyssey::EventManager::getInstance().publish(new RewardsActiveEvent(mCurrentLevel));
					ToggleCharacterUI(false);
					Rewards->setActive(true);
				}

				//Check to see if the update returned PLAYER_TEAM_DIED
				if (result == mCurrentBattle->PLAYER_TEAM_DIED)
				{
					SetTowerState(NOT_IN_BATTLE);
					// Go to main menu screen
					GoToMainMenu();
				}
				else
				{
					if (!mIsTutorial)
					{
						// Update to the next level
						mCurrentLevel = GetCurrentLevel() + 1;

						if (mCurrentLevel > mNumberOfLevels)
							SetTowerState(NOT_IN_BATTLE);
					}
				}
			}
		}
		else if (GetTowerState() == IN_REWARDS)
		{
			if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter) || mUsedBossCheatCode)
			{
				// Reset cheat code bool
				mUsedBossCheatCode = false;
				// Create temp xp variable
				float tempXP = 0.0f;

				if (mIsTutorial)
				{
					SetTowerState(NOT_IN_BATTLE);
					mIsTutorial = false;
					GameUIManager::getInstance().EndTutorial();
					GoToMainMenu();
				}

				// Check to see if that was our last level for completing the tower
				if (GetCurrentLevel() > mNumberOfLevels)
				{
					SetTowerState(NOT_IN_BATTLE);

					// Give player some XP
					tempXP = 500.0f;
					// Print how much XP was given to the players
					for (int i = 0; i < mPlayerTeam.size(); i++)
					{
						mPlayerTeam[i]->getComponent<Character>()->AddExp(tempXP);
					}
				}
				else
				{
					// Give player some XP
					tempXP = 100.0f;
					// Print how much XP was given to the player
					for (int i = 0; i < mPlayerTeam.size(); i++)
					{
						Character* currCharacter = mPlayerTeam[i]->getComponent<Character>();
						currCharacter->AddExp(tempXP);

						// If the player made the finishing kill set him back to NONE state
						if (currCharacter->GetState() == STATE::FINISHED)
						{
							currCharacter->SetState(STATE::NONE);
						}
					}

					// Boss Level
					if (mCurrentLevel == mNumberOfLevels)
					{
						// Save team
						TeamManager::getInstance().UpdatePlayerTeam(mPlayerTeam);
						mEnemyTeam.clear();

						for (int i = 0; i < mPlayerTeam.size(); i++)
						{
							SkillHoverComponent* hover = GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<SkillHoverComponent>();
							
							for (int i = 0; i < hover->GetSkillSprites().size(); i++)
							{
								hover->GetSkillSprites()[i]->unregisterCallback("onMouseClick");
								hover->GetSkillSprites()[i]->unregisterCallback("onMouseEnter");
								hover->GetSkillSprites()[i]->unregisterCallback("onMouseExit");
							}
						}

						// Switch to the boss scene
						Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Boss Scene"));
					}
					else
						CreateBattleInstance();

					// Turn off the rewads screen
					Rewards->setActive(false);
					ToggleCharacterUI(true);
				}
			}
			float stat_opacity = Rewards->getElements<Odyssey::Text2D>()[Rewards->getElements<Odyssey::Text2D>().size() - 1]->getOpacity();
			if (stat_opacity <= 0.0f) {
				float newOpacity = stat_opacity + ((int(deltaTime) % 10) * 0.10f);
				Rewards->getElements<Odyssey::Text2D>()[Rewards->getElements<Odyssey::Text2D>().size() - 1]->setOpacity(newOpacity);
				stat_opacity = Rewards->getElements<Odyssey::Text2D>()[Rewards->getElements<Odyssey::Text2D>().size() - 1]->getOpacity();
			}
			else if (stat_opacity >= 1.0f)
			{
				float newOpacity = stat_opacity - ((int(deltaTime) % 10) * 0.10f);
				Rewards->getElements<Odyssey::Text2D>()[Rewards->getElements<Odyssey::Text2D>().size() - 1]->setOpacity(newOpacity);
				stat_opacity = Rewards->getElements<Odyssey::Text2D>()[Rewards->getElements<Odyssey::Text2D>().size() - 1]->getOpacity();
			}
		}
		else if (GetTowerState() == NOT_IN_BATTLE)
		{
			if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter))
			{
				Rewards->setActive(false);
				ToggleCharacterUI(true);
				GoToMainMenu();
			}
		}
	}
}

void TowerManager::SetUpTowerManager(int _numberOfBattles)
{
	// Set the number of levels for this tower
	//mNumberOfLevels = _numberOfBattles;
	//mCurrentBattle = nullptr;
}

void TowerManager::CreateBattleInstance()
{
	// TODO: REMOVE POST BUILD 02
	if (mCurrentLevel == 1)
		system("CLS");

	// Clear the combat at the start of each battle log
	GameUIManager::getInstance().ClearCombatLog();

	StatTracker::Instance().SetTutorialState(false);

	// Send off the current level number
	Odyssey::EventManager::getInstance().publish(new LevelStartEvent(mCurrentLevel, mPlayerTeam[0]->getComponent<Character>()->GetName(), mPlayerTeam[1]->getComponent<Character>()->GetName(), mPlayerTeam[2]->getComponent<Character>()->GetName(),
		mPlayerTeam[0]->getComponent<Character>()->GetPortraitPath(), mPlayerTeam[1]->getComponent<Character>()->GetPortraitPath(), mPlayerTeam[2]->getComponent<Character>()->GetPortraitPath(),
		mPlayerTeam[0]->getComponent<Character>(), mPlayerTeam[1]->getComponent<Character>(), mPlayerTeam[2]->getComponent<Character>()));

	// Remove the current enemy team from the scene
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		// Clear the status effects
		GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<CharacterHUDElements>()->ClearStatusEffects();
		// Destory the previous enemy's UI Elements
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the previous enemy's impact indicator
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetInpactIndicator()));
		// Destroy the previous enemy's blood particle effect
		//Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetPSBlood()->getEntity()));
		// Destroy the previous enemies
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]));
	}
	// Clear the previous enemy list
	mEnemyTeam.clear();
	// Create the new enemy team before creating the battle
	mEnemyTeam = TeamManager::getInstance().CreateEnemyTeam(mCurrentLevel - 1);

	// Set up clickable character UI
	GameUIManager::getInstance().SetupClickableCharacterUI();

	// Create the battle instance
	mCurrentBattle = new BattleInstance(mPlayerTeam, mEnemyTeam, false);

	// Since we created a BattleInstance we will be in combat
	SetTowerState(IN_BATTLE);
}

void TowerManager::CreateTutorialInstance()
{
	StatTracker::Instance().SetTutorialState(true);

	GameUIManager::getInstance().ClearCombatLog();

	// Remove the current enemy team from the scene
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		// Clear the status effects
		GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<CharacterHUDElements>()->ClearStatusEffects();
		// Destory the previous enemy's UI Elements
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]));
		// Destroy the previous enemy's impact indicator
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetInpactIndicator()));
		// Destroy the previous enemy's blood particle effect
		//Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]->getComponent<Character>()->GetPSBlood()->getEntity()));
		// Destroy the previous enemies
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mEnemyTeam[i]));
	}
	
	// Clear the previous enemy list
	mEnemyTeam.clear();
	// Create the new enemy team before creating the battle
	mEnemyTeam = TeamManager::getInstance().CreateEnemyTeam(1);

	// Set up clickable character UI
	GameUIManager::getInstance().SetupClickableCharacterUI();

	// Create the battle instance
	mCurrentBattle = new BattleInstance(mPlayerTeam, mEnemyTeam, true);

	// Since we created a BattleInstance we will be in combat
	SetTowerState(IN_BATTLE);
}

void TowerManager::DestroyBattleInstance()
{
	// Destory pointer and set it to a nullptr
	if (mCurrentBattle)
	{
		// Destroy the current mTurnIndicator in the current battle
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mCurrentBattle->GetTurnIndicator()));

		// Delete the current battle
		delete mCurrentBattle;
		mCurrentBattle = nullptr;
	}

	// Since we destoryed the BattleInstance we will NOT be in combat
	SetTowerState(NOT_IN_BATTLE);
}

void TowerManager::TogglePauseMenu()
{
	//Toggle mIsPaused bool
	mIsPaused = !mIsPaused;

	// Toggle pause menu canvas
	Odyssey::UICanvas* pauseMenuCanvas = nullptr;
	// Always look for the pause input button
	if (!mIsBossScene)
		pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>();
	else
		pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu(1)->getComponent<Odyssey::UICanvas>();

	GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, !pauseMenuCanvas->isActive());

	ToggleCharacterUI(!pauseMenuCanvas->isActive());

	if (pauseMenuCanvas->isActive())
	{
		// Turn Off Tutorial UI
		if (mIsTutorial)
			GameUIManager::getInstance().ToggleCanvas(GameUIManager::getInstance().GetTutorialCanvas(), false);

		// Set the new cursor
		Odyssey::EventManager::getInstance().publish(new Odyssey::ChangeMouseCursorEvent(L"assets/images/Cursor/Cursor_Basic.cur"));
		// Set the time scale to 0 on pause
		Odyssey::EventManager::getInstance().publish(new Odyssey::SetTimeScaleEvent(0.0f));
	}
	// else if we are turning off the pause menu
	else if (!pauseMenuCanvas->isActive())
	{
		// Turn on Tutorial UI
		if (mIsTutorial)
			GameUIManager::getInstance().ToggleCanvas(GameUIManager::getInstance().GetTutorialCanvas(), true);

		// Set the time scale back to 1
		Odyssey::EventManager::getInstance().publish(new Odyssey::SetTimeScaleEvent(1.0f));
	}
}

void TowerManager::ToggleCharacterUI(bool _onOrOff)
{
	// Turn off the hero ui depening if the pause menu is on or off
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->setActive(_onOrOff);
	}

	// Turn off the enemy ui depening if the pause menu is on or off
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->setActive(_onOrOff);
	}
}

void TowerManager::ShowOptionsMenu()
{
	if (!mIsBossScene)
	{
		// Turn off the pause menu
		Odyssey::UICanvas* pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>();
		GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, false);
		// Turn on the options menu
		Odyssey::UICanvas* optionsMenuCanvas = GameUIManager::getInstance().GetOptionsMenu(0)->getComponent<Odyssey::UICanvas>();
		GameUIManager::getInstance().ToggleCanvas(optionsMenuCanvas, true);
	}
	else
	{
		// Turn off the pause menu
		Odyssey::UICanvas* pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu(1)->getComponent<Odyssey::UICanvas>();
		GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, false);
		// Turn on the options menu
		Odyssey::UICanvas* optionsMenuCanvas = GameUIManager::getInstance().GetOptionsMenu(1)->getComponent<Odyssey::UICanvas>();
		GameUIManager::getInstance().ToggleCanvas(optionsMenuCanvas, true);
	}
}

void TowerManager::ShowControlScreen()
{
	if (!mIsBossScene)
	{
		GameUIManager::getInstance().GetControlsImage(0)->setVisible(true);
		GameUIManager::getInstance().GetControlsBackText(0)->setVisible(true);
		GameUIManager::getInstance().GetControlsBackText(0)->registerCallback("onMouseClick", this, &TowerManager::HideControlScreen);
		GameUIManager::getInstance().GetResumeButton(0)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetOptionsVolumeButton(0)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetOptionsControlsButton(0)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetMainMenuButton(0)->unregisterCallback("onMouseClick");
	}
	else
	{
		GameUIManager::getInstance().GetControlsImage(1)->setVisible(true);
		GameUIManager::getInstance().GetControlsBackText(1)->setVisible(true);
		GameUIManager::getInstance().GetControlsBackText(1)->registerCallback("onMouseClick", this, &TowerManager::HideControlScreen);
		GameUIManager::getInstance().GetResumeButton(1)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetOptionsVolumeButton(1)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetOptionsControlsButton(1)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetMainMenuButton(1)->unregisterCallback("onMouseClick");
	}

}

void TowerManager::HideControlScreen()
{
	if (!mIsBossScene)
	{
		GameUIManager::getInstance().GetControlsImage(0)->setVisible(false);
		GameUIManager::getInstance().GetControlsBackText(0)->setVisible(false);
		GameUIManager::getInstance().GetControlsBackText(0)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetResumeButton(0)->registerCallback("onMouseClick", this, &TowerManager::TogglePauseMenu);
		GameUIManager::getInstance().GetOptionsVolumeButton(0)->registerCallback("onMouseClick", this, &TowerManager::ShowOptionsMenu);
		GameUIManager::getInstance().GetOptionsControlsButton(0)->registerCallback("onMouseClick", this, &TowerManager::ShowControlScreen);
		GameUIManager::getInstance().GetMainMenuButton(0)->registerCallback("onMouseClick", this, &TowerManager::GoToMainMenu);
	}
	else
	{
		GameUIManager::getInstance().GetControlsImage(1)->setVisible(false);
		GameUIManager::getInstance().GetControlsBackText(1)->setVisible(false);
		GameUIManager::getInstance().GetControlsBackText(1)->unregisterCallback("onMouseClick");
		GameUIManager::getInstance().GetResumeButton(1)->registerCallback("onMouseClick", this, &TowerManager::TogglePauseMenu);
		GameUIManager::getInstance().GetOptionsVolumeButton(1)->registerCallback("onMouseClick", this, &TowerManager::ShowOptionsMenu);
		GameUIManager::getInstance().GetOptionsControlsButton(1)->registerCallback("onMouseClick", this, &TowerManager::ShowControlScreen);
		GameUIManager::getInstance().GetMainMenuButton(1)->registerCallback("onMouseClick", this, &TowerManager::GoToMainMenu);
	}
}

void TowerManager::GoToMainMenu()
{
	SetTowerState(NOT_IN_BATTLE);

	// Remove the current player team from the scene
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<CharacterHUDElements>()->ClearStatusEffects();
		
		SkillHoverComponent* hover = GameUIManager::getInstance().GetCharacterHuds()[mPlayerTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<SkillHoverComponent>();

		for (int i = 0; i < hover->GetSkillSprites().size(); i++)
		{
			hover->GetSkillSprites()[i]->unregisterCallback("onMouseClick");
			hover->GetSkillSprites()[i]->unregisterCallback("onMouseEnter");
			hover->GetSkillSprites()[i]->unregisterCallback("onMouseExit");
		}
	}
	mPlayerTeam.clear();

	// Remove the current enemy team from the scene
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		GameUIManager::getInstance().GetCharacterHuds()[mEnemyTeam[i]->getComponent<Character>()->GetHudIndex()]->getComponent<CharacterHUDElements>()->ClearStatusEffects();
	}
	mEnemyTeam.clear();

	// Remove the clickable UI elements
	//for (int i = 0; i < GameUIManager::getInstance().GetClickableUIElements().size(); i++)
	//{
	//	Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(GameUIManager::getInstance().GetClickableUIElements()[i]));
	//}

	// Deactivate the rewards screen
	Rewards->setActive(false);

	for (int i = 0; i < 2; i++)
	{
		// Deactivate the pause menu
		Odyssey::Entity* pauseMenu = GameUIManager::getInstance().GetPauseMenu(i);
		GameUIManager::getInstance().ToggleCanvas(pauseMenu->getComponent<Odyssey::UICanvas>(), false);
	}

	for (int i = 0; i < GameUIManager::getInstance().GetClickableUIElements().size(); i++)
	{
		Odyssey::Entity* clickObj = GameUIManager::getInstance().GetClickableUIElements()[i];
		clickObj->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>()->unregisterCallback("onMouseClick");
		clickObj->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>()->unregisterCallback("onMouseEnter");
		clickObj->getComponent<Odyssey::UICanvas>()->getElement<Odyssey::Rectangle2D>()->unregisterCallback("onMouseExit");
	}

	GameUIManager::getInstance().ClearClickableCharacterList();
	TeamManager::getInstance().ClearUpdatedPlayerTeam();

	// Set the current level back to 1
	mCurrentLevel = 1;

	// Turn off battle music
	RedAudioManager::Instance().Stop("BackgroundBattle");

	// Destory the battle instance
	DestroyBattleInstance();

	mIsPaused = true;
	// Set the time scale back to 1
	Odyssey::EventManager::getInstance().publish(new Odyssey::SetTimeScaleEvent(1.0f));
	// Switch to main menu scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("MainMenu"));
}

void TowerManager::CreateThePlayerTeam()
{
	// Create the player characters
	std::vector<DirectX::XMVECTOR> mPlayerPositions;
	mPlayerPositions.resize(3);
	mPlayerPositions[0] = DirectX::XMVectorSet(-5.0f, 0.0f, 10.0f, 1.0f); // First Character Selected
	mPlayerPositions[1] = DirectX::XMVectorSet(0.0f, 0.0f, 10.0f, 1.0f); // Second Character Selected
	mPlayerPositions[2] = DirectX::XMVectorSet(5.0f, 0.0f, 10.0f, 1.0f); // Third Character Selected

	mPlayerTeam.clear();
	
	bool createdPlayerTeamPreviously = false;
	if (TeamManager::getInstance().GetUpdatedPlayerTeam().size() > 0)
	{
		createdPlayerTeamPreviously = true;
		// Reset HUD stuff
		//CharacterFactory::getInstance().ResetCharacterHudIndex();
		//GameUIManager::getInstance().ClearHudList();
		// This must also be the boss scene
		mCurrentLevel = mNumberOfLevels;
	}

	// Create each player
	for (int i = 0; i < TeamManager::getInstance().GetPlayerTeamToCreate().size(); i++)
	{
		// Character we are about to create
		Odyssey::Entity* newCharacter = nullptr;
		// Character's HUD
		Odyssey::Entity* newHUD = nullptr;
		// Prefab
		Odyssey::Entity* prefab;
		// Hud Id type
		CharacterFactory::HudID hudID;
		// Clickable Character position
		GameUIManager::ClickableCharacterUI clickablePos;
		// Position taht the skill popup will needed to be spawned at
		SkillHoverComponent::HudPosition skillPopupPos;
		// Define the character type we need to create
		CharacterFactory::CharacterOptions characterToCreate = CharacterFactory::CharacterOptions::Paladin;
		// Get the hero type
		TeamManager::HeroType newHeroType = TeamManager::getInstance().GetPlayerTeamToCreate()[i];
		// Set the player rotations
		DirectX::XMVECTOR mPlayerRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		if (i == 0)
		{
			hudID = CharacterFactory::HudID::HeroLeft;
			skillPopupPos = SkillHoverComponent::HudPosition::Left;
			clickablePos = GameUIManager::ClickableCharacterUI::HeroLeft;
		}
		else if (i == 1)
		{
			hudID = CharacterFactory::HudID::HeroMiddle;
			skillPopupPos = SkillHoverComponent::HudPosition::Middle;
			clickablePos = GameUIManager::ClickableCharacterUI::HeroMiddle;
		}
		else
		{
			hudID = CharacterFactory::HudID::HeroRight;
			skillPopupPos = SkillHoverComponent::HudPosition::Right;
			clickablePos = GameUIManager::ClickableCharacterUI::HeroRight;
		}

		// Set the enum based on the name of the character
		if (newHeroType == TeamManager::HeroType::Paladin)
			characterToCreate = CharacterFactory::CharacterOptions::Paladin;
		else if (newHeroType == TeamManager::HeroType::Mage)
			characterToCreate = CharacterFactory::CharacterOptions::Mage;
		else if (newHeroType == TeamManager::HeroType::Bard)
			characterToCreate = CharacterFactory::CharacterOptions::Bard;
		else if (newHeroType == TeamManager::HeroType::Warrior)
			characterToCreate = CharacterFactory::CharacterOptions::Warrior;
		else if (newHeroType == TeamManager::HeroType::Monk)
		{
			characterToCreate = CharacterFactory::CharacterOptions::Monk;
			mPlayerRotation = DirectX::XMVectorSet(DirectX::XMVectorGetX(mPlayerRotation), 180.0f, DirectX::XMVectorGetZ(mPlayerRotation), 1.0f);
		}

		// Create the character prefab
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(characterToCreate);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newCharacter, mPlayerPositions[i], mPlayerRotation));

		// Create the hud prefab
		prefab = CharacterFactory::getInstance().GetHUDPrefab(hudID);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &newHUD, mPlayerPositions[i], mPlayerRotation));


		// Set up the clickable UI and skill hover huds
		CharacterHUDElements* hudElements = newHUD->getComponent<CharacterHUDElements>();

		if (!createdPlayerTeamPreviously)
		{
			// Reset the colors of the stats
			hudElements->GetAttackNumber()->setColor(255.0f, 255.0f, 255.0f);
			hudElements->GetDefenseNumber()->setColor(255.0f, 255.0f, 255.0f);
			hudElements->GetSpeedNumber()->setColor(255.0f, 255.0f, 255.0f);
			// Make sure the bars are filled
			hudElements->GetHealthBar()->setFill(1.0f);
			hudElements->GetManaBar()->setFill(1.0f);
		}

		// Assign the character component
		SkillHoverComponent* hover = newHUD->addComponent<SkillHoverComponent>();
		hover->characterComponent = newCharacter->getComponent<Character>();
		// Register the skill sprites for hovering over them
		hover->registerSprite(hudElements->GetSkill1());
		hover->registerSprite(hudElements->GetSkill2());
		hover->registerSprite(hudElements->GetSkill3());
		hover->registerSprite(hudElements->GetSkill4());
		// Assign the characters skills to the hover list
		hover->mCharacterSkills = newCharacter->getComponent<Character>()->GetSkills();
		// Assign the position that the prefab will needed to be spawned at
		hover->mHudPositionEnum = skillPopupPos;
		// Clickable Skills
		HeroComponent* heroComp = newCharacter->getComponent<HeroComponent>();
		std::vector<Odyssey::Sprite2D*> skillSprites = hover->GetSkillSprites();
		heroComp->SetupClickableUI(skillSprites[0], skillSprites[1], skillSprites[2], skillSprites[3]);


		// Create the hero clickable UI box
		Odyssey::Entity* clickableHeroUI = nullptr;
		prefab = GameUIManager::getInstance().GetClickableUIPrefab(clickablePos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &clickableHeroUI, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }));
		GameUIManager::getInstance().AddClickableElementToList(clickableHeroUI);

		// Create the character's spot light
		Odyssey::Entity* spotLight = nullptr;
		prefab = CharacterFactory::getInstance().GetLightObjectPrefab(CharacterFactory::LightObjects::SpotLight);
		// Offset the y pos of the spotlight
		DirectX::XMVECTOR spotLightPos = { DirectX::XMVectorGetX(mPlayerPositions[i]), DirectX::XMVectorGetY(mPlayerPositions[i]) + 6.0f, DirectX::XMVectorGetZ(mPlayerPositions[i]), 1.0f };
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &spotLight, spotLightPos, DirectX::XMVECTOR{ 90.0f, 0.0f, 0.0f, 1.0f }));
		mCharacterSpotLights[i] = spotLight;

		// Change spot light for different heroes
		if (newHeroType == TeamManager::HeroType::Mage)
		{
			mCharacterSpotLights[i]->getComponent<Odyssey::Light>()->setIntensity(1.895f);
			mCharacterSpotLights[i]->getComponent<Odyssey::Light>()->setRange(12.45);
			mCharacterSpotLights[i]->getComponent<Odyssey::Light>()->setSpotAngle(24.69);
		}
		else if (newHeroType == TeamManager::HeroType::Warrior || newHeroType == TeamManager::HeroType::Monk)
		{
			mCharacterSpotLights[i]->getComponent<Odyssey::Light>()->setIntensity(1.895f);
			mCharacterSpotLights[i]->getComponent<Odyssey::Light>()->setRange(30.033f);
			mCharacterSpotLights[i]->getComponent<Odyssey::Light>()->setSpotAngle(24.96f);
			mCharacterSpotLights[i]->getComponent<Odyssey::Transform>()->setPosition(DirectX::XMVectorGetX(spotLightPos), DirectX::XMVectorGetY(spotLightPos) + 9.675f, DirectX::XMVectorGetZ(spotLightPos));
		}

		// Create the impact indicator for the heroes
		Odyssey::Entity* impactIndicator = nullptr;
		//DirectX::XMVECTOR impactIndicatorPosition = mPlayerPositions[i];
		DirectX::XMVECTOR impactIndicatorPosition = { DirectX::XMVectorGetX(mPlayerPositions[i]), DirectX::XMVectorGetY(mPlayerPositions[i]) + 0.1f, DirectX::XMVectorGetZ(mPlayerPositions[i]), 1.0f };
		prefab = CharacterFactory::getInstance().GetImpactIndicatorPrefab();
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &impactIndicator, impactIndicatorPosition, mPlayerRotation));
		// Assign the impact indicator for the heroes
		impactIndicator->setActive(false);
		newCharacter->getComponent<Character>()->SetImpactIndicator(impactIndicator);

		// Assign the blood effect for the heroes
		prefab = CharacterFactory::getInstance().GetBloodEffectPrefab();
		newCharacter->getComponent<Character>()->SetBloodPrefab(prefab);

		// Set the character's hud index number
		newCharacter->getComponent<Character>()->SetHudIndex(CharacterFactory::getInstance().GetCharacterHudIndex());
		// Increase the character index
		CharacterFactory::getInstance().IncreaseCharacterHUDIndex();
		// Add the new HUD to the list of HUDs
		GameUIManager::getInstance().AddHudToList(newHUD);

		// Set the elements of the character's HUD
		GameUIManager::getInstance().AssignCharacterHudElements(newCharacter->getComponent<Character>(), newHUD);

		// Add the character in the player list
		mPlayerTeam.push_back(newCharacter);
	}
}
