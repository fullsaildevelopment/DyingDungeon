#include "TeamSelectionController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"

CLASS_DEFINITION(Odyssey::Component, TeamSelectionController)
TeamSelectionController::TeamSelectionController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void TeamSelectionController::initialize()
{
	//RedAudioManager::Instance().Loop("BackgroundMenu");
	//RedAudioManager::Instance().GetAudio("BackgroundMenu")->Stop();
	changedTheScene = false;

	teamCount = 0;
	teamIsFull = false;

	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Paladin)->registerCallback("onMouseClick", this, &TeamSelectionController::AddPaladinImage);
	GameUIManager::getInstance().GetCharacterSelectImage(GameUIManager::CharacterType::Mage)->registerCallback("onMouseClick", this, &TeamSelectionController::AddMageImage);
}

void TeamSelectionController::update(double deltaTime)
{
	if (teamIsFull && !changedTheScene)
	{
		changedTheScene = true;
		RedAudioManager::Instance().Stop("BackgroundMenu");
		RedAudioManager::Instance().Loop("BackgroundBattle");
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Game"));
	}
}

void TeamSelectionController::AddPaladinImage()
{
	if (!teamIsFull)
	{
		// Increase the team count
		teamCount++;

		// Get the deminsion of the original black square
		DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->getDimensions();
		//Set the new image to the slot
		GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->setSprite(L"assets/images/PaladinPortrait.jpg", deminsion.x, deminsion.y);

		// Check if the team is full now
		if (teamCount == 3)
		{
			teamIsFull = true;
		}
	}
}

void TeamSelectionController::AddMageImage()
{
	if (!teamIsFull)
	{
		// Increase the team count
		teamCount++;

		// Get the deminsion of the original black square
		DirectX::XMFLOAT2 deminsion = GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->getDimensions();
		//Set the new image to the slot
		GameUIManager::getInstance().GetTeamMemberSlot(teamCount)->setSprite(L"assets/images/MagePortrait.jpg", deminsion.x, deminsion.y);

		// Check if the team is full now
		if (teamCount == 3)
		{
			teamIsFull = true;
		}
	}
}

