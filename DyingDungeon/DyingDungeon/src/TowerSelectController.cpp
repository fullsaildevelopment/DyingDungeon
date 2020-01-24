#include "TowerSelectController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "UICanvas.h"

CLASS_DEFINITION(Odyssey::Component, TowerSelectController)
TowerSelectController::TowerSelectController(Odyssey::Application* application)
{
	mApplication = application;
	mRect = nullptr;
}

void TowerSelectController::initialize()
{
	//RedAudioManager::Instance().Play("BackgroundMenu");
	// Turn on the tower select canvas
	std::shared_ptr<Odyssey::Entity> towerSelectMenu = GameUIManager::getInstance().GetTowerSelectMenu();
	GameUIManager::getInstance().ToggleCanvas(towerSelectMenu->getComponent<Odyssey::UICanvas>(), true);

	// Make the door switch to the team selection scene when clicked
	GameUIManager::getInstance().GetDoorSprites()[0]->registerCallback("onMouseClick", this, &TowerSelectController::GoToTeamSelection);
	GameUIManager::getInstance().GetDoorSprites()[0]->registerCallback("onMouseEnter", this, &TowerSelectController::ChangeDoorState);
	GameUIManager::getInstance().GetDoorSprites()[0]->registerCallback("onMouseExit", this, &TowerSelectController::ChangeDoorState);

	// Reset door animations components
	// Reset bools
	mDoorIsClosed = true;
	mDoOpenDoorAnimation = false;
	mDoCloseDoorAnimation = false;
	// Reset Index
	mDoorImageIndex = 1;
	// Reset the door image
	SetNextDoorImage(GameUIManager::getInstance().GetDoorSprites()[0]);
}

void TowerSelectController::update(double deltaTime)
{
	// If I need to open the door
	if (mDoOpenDoorAnimation || mDoCloseDoorAnimation)
	{
		//Increase the total time by aading the delta time
		totalTime += deltaTime;

		// Only chnage the door image every 0.25 seconds
		if (totalTime >= 0.1f)
		{
			// Set the door image index to be the next door image
			if (mDoOpenDoorAnimation)
			{
				mDoorImageIndex++;

				// Stop doing the animation if we reached the final valid index value(5)
				if (mDoorImageIndex == 5)
				{
					mDoOpenDoorAnimation = false;
				}
			}
			else if (mDoCloseDoorAnimation)
			{
				mDoorImageIndex--;

				// Stop doing the animation if we reached the final valid index value(5)
				if (mDoorImageIndex == 1)
				{
					mDoCloseDoorAnimation = false;
				}
			}

			// Set the new door image
			SetNextDoorImage(GameUIManager::getInstance().GetDoorSprites()[0]);

			// Reset the total time
			totalTime = 0.0f;
		}
	}
}

void TowerSelectController::GoToTeamSelection()
{
	// Turn off the tower select canvas
	std::shared_ptr<Odyssey::Entity> towerSelectMenu = GameUIManager::getInstance().GetTowerSelectMenu();
	GameUIManager::getInstance().ToggleCanvas(towerSelectMenu->getComponent<Odyssey::UICanvas>(), false);

	// Switch to the team select scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TeamSelection"));
}

void TowerSelectController::ChangeDoorState()
{
	if (mDoorIsClosed)
	{
		mDoCloseDoorAnimation = false;
		mDoOpenDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorOpen");
	}
	else
	{
		mDoOpenDoorAnimation = false;
		mDoCloseDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorClose");
	}

	// Flip the bool for next time
	mDoorIsClosed = !mDoorIsClosed;
}

void TowerSelectController::SetNextDoorImage(Odyssey::Sprite2D* _doorImageToChange)
{
	DirectX::XMFLOAT2 deminsions = _doorImageToChange->getDimensions();

	switch (mDoorImageIndex)
	{
		case 1:
			_doorImageToChange->setSprite(L"assets/images/DoorIMages/MedievalDoor-1.png", deminsions.x, deminsions.y);
			break;
		case 2:
			_doorImageToChange->setSprite(L"assets/images/DoorIMages/MedievalDoor-2.png", deminsions.x, deminsions.y);
			break;
		case 3:
			_doorImageToChange->setSprite(L"assets/images/DoorIMages/MedievalDoor-3.png", deminsions.x, deminsions.y);
			break;
		case 4:
			_doorImageToChange->setSprite(L"assets/images/DoorIMages/MedievalDoor-4.png", deminsions.x, deminsions.y);
			break;
		case 5:
			_doorImageToChange->setSprite(L"assets/images/DoorIMages/MedievalDoor-5.png", deminsions.x, deminsions.y);
			break;
	}
}
