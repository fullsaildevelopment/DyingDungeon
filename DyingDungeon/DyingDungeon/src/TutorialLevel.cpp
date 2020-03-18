#include "TutorialLevel.h"
#include "BattleInstance.h"
#include "StatTracker.h"


CLASS_DEFINITION(Odyssey::Component, TutorialLevel)
TutorialLevel::TutorialLevel()
{
	currentState = TutorialState::NONE;
	mPlayer = nullptr;
	mEnemy = nullptr;

	// Asset Paths Player
	HighlightPaths["PlayerUI"] = L"assets/images/TutorialLevel/HighlightPlayerUI.png";
	HighlightPaths["Player"] = L"assets/images/TutorialLevel/HighlightPlayer.png";
	HighlightPaths["PlayerSkills"] = L"assets/images/TutorialLevel/HighlightPlayerSkills.png";
	HighlightPaths["PlayerBuffs"] = L"assets/images/TutorialLevel/HighlightPlayerBuffs.png";
	HighlightPaths["PlayerStats"] = L"assets/images/TutorialLevel/HighlightPlayerStats.png";

	// Asset Paths Enemy
	HighlightPaths["EnemyUI"] = L"assets/images/TutorialLevel/HighlightEnemyUI.png";
	HighlightPaths["Enemy"] = L"assets/images/TutorialLevel/HighlightEnemy.png";
	HighlightPaths["EnemyBuffs"] = L"assets/images/TutorialLevel/HighlightEnemyBuffs.png";

	// Asset Paths Generic
	HighlightPaths["Level"] = L"assets/images/TutorialLevel/HighlightLevel.png";
	HighlightPaths["TurnNumber"] = L"assets/images/TutorialLevel/HighlightTurnNumber.png";

}

void TutorialLevel::initialize()
{
	currentState = TutorialState::STAGEONE;
	
}

void TutorialLevel::update(double deltaTime)
{
	switch (currentState)
	{
		// Default
		case TutorialState::NONE:
		{
			return;
			break;
		};
		// 
		case TutorialState::STAGEONE:
		{
			Odyssey::EventManager::getInstance().publish(new Odyssey::SetTimeScaleEvent(0.0f));


			break;
		};
		// 
		case TutorialState::STAGETWO:
		{
			
			break;
		};
		// 
		case TutorialState::STAGETHREE:
		{
			
			break;
		};
	}
}

void TutorialLevel::GoToMainMenu()
{
	// Change timescale back to 1 if we have not already done so
	Odyssey::EventManager::getInstance().publish(new Odyssey::SetTimeScaleEvent(1.0f));


	// Change scene to the main menu
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("MainMenu"));
}

TutorialLevel::~TutorialLevel()
{
	currentState = TutorialState::NONE;
}
