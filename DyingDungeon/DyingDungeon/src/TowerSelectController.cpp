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

	// Don't show the tower info canvas
	GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(false);

	// Clear the door list
	mDoorList.clear();

	// Resize the list to the number of doors
	mDoorList.resize(GameUIManager::getInstance().GetDoorSprites().size());

	// Reassing the values for each door
	for (int i = 0; i < mDoorList.size(); i++)
	{
		// Assign the door sprite
		mDoorList[i].doorImage = GameUIManager::getInstance().GetDoorSprites()[i];

		// Reset door animations components
		// Reset bools
		mDoorList[i].mDoorIsClosed = true;
		mDoorList[i].mDoOpenDoorAnimation = false;
		mDoorList[i].mDoCloseDoorAnimation = false;
		// Reset Index
		mDoorList[i].mDoorImageIndex = 1;

		// Reset the door image
		SetNextDoorImage(mDoorList[i]);
	}

	// Assign the callbacks for the door images
	static bool registeredCallback = false;
	if (!registeredCallback)
	{
		//Set static bool to true
		registeredCallback = true;
		mDoorList[0].doorImage->registerCallback("onMouseClick", this, &TowerSelectController::GoToTeamSelection);
		mDoorList[0].doorImage->registerCallback("onMouseEnter", this, &TowerSelectController::ChangeDoor1State);
		mDoorList[0].doorImage->registerCallback("onMouseExit", this, &TowerSelectController::ChangeDoor1State);
		mDoorList[1].doorImage->registerCallback("onMouseClick", this, &TowerSelectController::GoToTeamSelection);
		mDoorList[1].doorImage->registerCallback("onMouseEnter", this, &TowerSelectController::ChangeDoor2State);
		mDoorList[1].doorImage->registerCallback("onMouseExit", this, &TowerSelectController::ChangeDoor2State);
		mDoorList[2].doorImage->registerCallback("onMouseClick", this, &TowerSelectController::GoToTeamSelection);
		mDoorList[2].doorImage->registerCallback("onMouseEnter", this, &TowerSelectController::ChangeDoor3State);
		mDoorList[2].doorImage->registerCallback("onMouseExit", this, &TowerSelectController::ChangeDoor3State);
		mDoorList[3].doorImage->registerCallback("onMouseClick", this, &TowerSelectController::GoToTeamSelection);
		mDoorList[3].doorImage->registerCallback("onMouseEnter", this, &TowerSelectController::ChangeDoor4State);
		mDoorList[3].doorImage->registerCallback("onMouseExit", this, &TowerSelectController::ChangeDoor4State);
		// Have the fifth door take you to the Scene2
		mDoorList[4].doorImage->registerCallback("onMouseClick", this, &TowerSelectController::GoToScene2);
		mDoorList[4].doorImage->registerCallback("onMouseEnter", this, &TowerSelectController::ChangeDoor5State);
		mDoorList[4].doorImage->registerCallback("onMouseExit", this, &TowerSelectController::ChangeDoor5State);
	}
}

void TowerSelectController::update(double deltaTime)
{
	// If I need to open the door
	if (mDoorList[0].mDoOpenDoorAnimation || mDoorList[0].mDoCloseDoorAnimation)
		Door1Animation(deltaTime);
	if (mDoorList[1].mDoOpenDoorAnimation || mDoorList[1].mDoCloseDoorAnimation)
		Door2Animation(deltaTime);
	if (mDoorList[2].mDoOpenDoorAnimation || mDoorList[2].mDoCloseDoorAnimation)
		Door3Animation(deltaTime);
	if (mDoorList[3].mDoOpenDoorAnimation || mDoorList[3].mDoCloseDoorAnimation)
		Door4Animation(deltaTime);
	if (mDoorList[4].mDoOpenDoorAnimation || mDoorList[4].mDoCloseDoorAnimation)
		Door5Animation(deltaTime);
	if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::M))
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
}

void TowerSelectController::GoToTeamSelection()
{
	// Turn off the tower select canvas
	std::shared_ptr<Odyssey::Entity> towerSelectMenu = GameUIManager::getInstance().GetTowerSelectMenu();
	GameUIManager::getInstance().ToggleCanvas(towerSelectMenu->getComponent<Odyssey::UICanvas>(), false);

	// Switch to the team select scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("TeamSelection"));
}

void TowerSelectController::GoToScene2()
{
	// Turn off the tower select canvas
	std::shared_ptr<Odyssey::Entity> towerSelectMenu = GameUIManager::getInstance().GetTowerSelectMenu();
	GameUIManager::getInstance().ToggleCanvas(towerSelectMenu->getComponent<Odyssey::UICanvas>(), false);

	// Switch to the team select scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Scene2"));
}

void TowerSelectController::ChangeDoor1State()
{
	if (mDoorList[0].mDoorIsClosed)
	{
		mDoorList[0].mDoCloseDoorAnimation = false;
		mDoorList[0].mDoOpenDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorOpen");

		// Show the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(true);
	}
	else
	{
		mDoorList[0].mDoOpenDoorAnimation = false;
		mDoorList[0].mDoCloseDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorClose");

		// Hide the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(false);
	}

	// Flip the bool for next time
	mDoorList[0].mDoorIsClosed = !mDoorList[0].mDoorIsClosed;
}

void TowerSelectController::ChangeDoor2State()
{
	if (mDoorList[1].mDoorIsClosed)
	{
		mDoorList[1].mDoCloseDoorAnimation = false;
		mDoorList[1].mDoOpenDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorOpen");

		// Show the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(true);
	}
	else
	{
		mDoorList[1].mDoOpenDoorAnimation = false;
		mDoorList[1].mDoCloseDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorClose");

		// Hide the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(false);
	}

	// Flip the bool for next time
	mDoorList[1].mDoorIsClosed = !mDoorList[1].mDoorIsClosed;
}

void TowerSelectController::ChangeDoor3State()
{
	if (mDoorList[2].mDoorIsClosed)
	{
		mDoorList[2].mDoCloseDoorAnimation = false;
		mDoorList[2].mDoOpenDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorOpen");

		// Show the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(true);
	}
	else
	{
		mDoorList[2].mDoOpenDoorAnimation = false;
		mDoorList[2].mDoCloseDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorClose");

		// Hide the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(false);
	}

	// Flip the bool for next time
	mDoorList[2].mDoorIsClosed = !mDoorList[2].mDoorIsClosed;
}

void TowerSelectController::ChangeDoor4State()
{
	if (mDoorList[3].mDoorIsClosed)
	{
		mDoorList[3].mDoCloseDoorAnimation = false;
		mDoorList[3].mDoOpenDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorOpen");

		// Show the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(true);
	}
	else
	{
		mDoorList[3].mDoOpenDoorAnimation = false;
		mDoorList[3].mDoCloseDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorClose");

		// Hide the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(false);
	}

	// Flip the bool for next time
	mDoorList[3].mDoorIsClosed = !mDoorList[3].mDoorIsClosed;
}

void TowerSelectController::ChangeDoor5State()
{
	if (mDoorList[4].mDoorIsClosed)
	{
		mDoorList[4].mDoCloseDoorAnimation = false;
		mDoorList[4].mDoOpenDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorOpen");

		// Show the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(true);
	}
	else
	{
		mDoorList[4].mDoOpenDoorAnimation = false;
		mDoorList[4].mDoCloseDoorAnimation = true;
		RedAudioManager::Instance().PlaySFX("DoorClose");

		// False the tower info canvas
		GameUIManager::getInstance().GetTowerInfoCanvas()->setActive(false);
	}

	// Flip the bool for next time
	mDoorList[4].mDoorIsClosed = !mDoorList[4].mDoorIsClosed;
}

void TowerSelectController::Door1Animation(double _deltaTime)
{
	
	//Increase the total time by aading the delta time
	mDoorList[0].totalTime += _deltaTime;

	// Only change the door image every 0.1 seconds
	if (mDoorList[0].totalTime >= 0.1f)
	{
		// Set the door image index to be the next door image
		if (mDoorList[0].mDoOpenDoorAnimation)
		{
			mDoorList[0].mDoorImageIndex += 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[0].mDoorImageIndex == 5)
			{
				mDoorList[0].mDoOpenDoorAnimation = false;
			}
		}
		else if (mDoorList[0].mDoCloseDoorAnimation)
		{
			mDoorList[0].mDoorImageIndex -= 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[0].mDoorImageIndex == 1)
			{
				mDoorList[0].mDoCloseDoorAnimation = false;
			}
		}

		// Set the new door image
		SetNextDoorImage(mDoorList[0]);

		// Reset the total time
		mDoorList[0].totalTime = 0.0f;
	}
}

void TowerSelectController::Door2Animation(double _deltaTime)
{

	//Increase the total time by aading the delta time
	mDoorList[1].totalTime += _deltaTime;

	// Only change the door image every 0.1 seconds
	if (mDoorList[1].totalTime >= 0.1f)
	{
		// Set the door image index to be the next door image
		if (mDoorList[1].mDoOpenDoorAnimation)
		{
			mDoorList[1].mDoorImageIndex += 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[1].mDoorImageIndex == 5)
			{
				mDoorList[1].mDoOpenDoorAnimation = false;
			}
		}
		else if (mDoorList[1].mDoCloseDoorAnimation)
		{
			mDoorList[1].mDoorImageIndex -= 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[1].mDoorImageIndex == 1)
			{
				mDoorList[1].mDoCloseDoorAnimation = false;
			}
		}

		// Set the new door image
		SetNextDoorImage(mDoorList[1]);

		// Reset the total time
		mDoorList[1].totalTime = 0.0f;
	}
}

void TowerSelectController::Door3Animation(double _deltaTime)
{

	//Increase the total time by aading the delta time
	mDoorList[2].totalTime += _deltaTime;

	// Only change the door image every 0.1 seconds
	if (mDoorList[2].totalTime >= 0.1f)
	{
		// Set the door image index to be the next door image
		if (mDoorList[2].mDoOpenDoorAnimation)
		{
			mDoorList[2].mDoorImageIndex += 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[2].mDoorImageIndex == 5)
			{
				mDoorList[2].mDoOpenDoorAnimation = false;
			}
		}
		else if (mDoorList[2].mDoCloseDoorAnimation)
		{
			mDoorList[2].mDoorImageIndex -= 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[2].mDoorImageIndex == 1)
			{
				mDoorList[2].mDoCloseDoorAnimation = false;
			}
		}

		// Set the new door image
		SetNextDoorImage(mDoorList[2]);

		// Reset the total time
		mDoorList[2].totalTime = 0.0f;
	}
}

void TowerSelectController::Door4Animation(double _deltaTime)
{

	//Increase the total time by aading the delta time
	mDoorList[3].totalTime += _deltaTime;

	// Only change the door image every 0.1 seconds
	if (mDoorList[3].totalTime >= 0.1f)
	{
		// Set the door image index to be the next door image
		if (mDoorList[3].mDoOpenDoorAnimation)
		{
			mDoorList[3].mDoorImageIndex += 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[3].mDoorImageIndex == 5)
			{
				mDoorList[3].mDoOpenDoorAnimation = false;
			}
		}
		else if (mDoorList[3].mDoCloseDoorAnimation)
		{
			mDoorList[3].mDoorImageIndex -= 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[3].mDoorImageIndex == 1)
			{
				mDoorList[3].mDoCloseDoorAnimation = false;
			}
		}

		// Set the new door image
		SetNextDoorImage(mDoorList[3]);

		// Reset the total time
		mDoorList[3].totalTime = 0.0f;
	}
}

void TowerSelectController::Door5Animation(double _deltaTime)
{

	//Increase the total time by aading the delta time
	mDoorList[4].totalTime += _deltaTime;

	// Only change the door image every 0.1 seconds
	if (mDoorList[4].totalTime >= 0.1f)
	{
		// Set the door image index to be the next door image
		if (mDoorList[4].mDoOpenDoorAnimation)
		{
			mDoorList[4].mDoorImageIndex += 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[4].mDoorImageIndex == 5)
			{
				mDoorList[4].mDoOpenDoorAnimation = false;
			}
		}
		else if (mDoorList[4].mDoCloseDoorAnimation)
		{
			mDoorList[4].mDoorImageIndex -= 1;

			// Stop doing the animation if we reached the final valid index value(5)
			if (mDoorList[4].mDoorImageIndex == 1)
			{
				mDoorList[4].mDoCloseDoorAnimation = false;
			}
		}

		// Set the new door image
		SetNextDoorImage(mDoorList[4]);

		// Reset the total time
		mDoorList[4].totalTime = 0.0f;
	}
}

void TowerSelectController::SetNextDoorImage(Door _doorToChange)
{
	DirectX::XMFLOAT2 deminsions = _doorToChange.doorImage->getDimensions();

	switch (_doorToChange.mDoorImageIndex)
	{
		case 1:
			_doorToChange.doorImage->setSprite(L"assets/images/DoorIMages/MedievalDoor-1.png", deminsions.x, deminsions.y);
			break;
		case 2:
			_doorToChange.doorImage->setSprite(L"assets/images/DoorIMages/MedievalDoor-2.png", deminsions.x, deminsions.y);
			break;
		case 3:
			_doorToChange.doorImage->setSprite(L"assets/images/DoorIMages/MedievalDoor-3.png", deminsions.x, deminsions.y);
			break;
		case 4:
			_doorToChange.doorImage->setSprite(L"assets/images/DoorIMages/MedievalDoor-4.png", deminsions.x, deminsions.y);
			break;
		case 5:
			_doorToChange.doorImage->setSprite(L"assets/images/DoorIMages/MedievalDoor-5.png", deminsions.x, deminsions.y);
			break;
	}
}
