
#include "InputManager.h"
#include "EventManager.h"

namespace Odyssey
{
	InputManager::InputManager()
	{
		// Subscribe to the key press event
		EventManager::getInstance().subscribe(this, &InputManager::onKeypress);

		// Subscribe to the key up event
		EventManager::getInstance().subscribe(this, &InputManager::onKeyUp);

		EventManager::getInstance().subscribe(this, &InputManager::onMouseMove);

		EventManager::getInstance().subscribe(this, &InputManager::onMouseHover);

		sendMouseHover = true;
	}

	InputManager& InputManager::getInstance()
	{
		// Create the static Singleton instance and return it.
		static InputManager instance;
		return instance;
	}

	void InputManager::onKeypress(KeypressEvent* evnt)
	{
		// Convert the keycode to an int
		int codeID = (int)(evnt->keyCode);

		// Reject the registration if the key is already contained inside the key press or down map
		if (mKeyPressMap[codeID] || mKeyDownMap[codeID])
			return;

		// Set the key press and key down state to true
		mKeyPressMap[codeID] = true;
		mKeyDownMap[codeID] = true;
	}

	void InputManager::onKeyUp(KeyUpEvent* evnt)
	{
		// Convert the keycode to an int
		int codeID = (int)(evnt->keyCode);

		// Set the key press and key down state to false
		mKeyPressMap[codeID] = false;
		mKeyDownMap[codeID] = false;

		// Set the key up state to true
		mKeyUpMap[codeID] = true;
	}

	void InputManager::onMouseMove(MouseInputEvent* evnt)
	{
		mouseX = evnt->mouseX;
		mouseY = evnt->mouseY;

		if (sendMouseHover)
		{
			EventManager::getInstance().publish(new MouseMoveEvent(mouseX, mouseY));
			sendMouseHover = false;
		}
	}

	void InputManager::onMouseHover(MouseMoveEvent* evnt)
	{
		sendMouseHover = true;
	}

	bool InputManager::getKeyPress(KeyCode key)
	{
		// Convert the keycode to an int
		int codeID = (int)(key);

		// Check if the map contains the key press
		if (mKeyPressMap[codeID])
		{
			// Consume the key press event and return true
			mKeyPressMap[codeID] = false;
			return true;
		}

		// The key was not found in the map, no key press
		return false;
	}

	bool InputManager::getKeyDown(KeyCode key)
	{
		// Convert the keycode to an int
		int codeID = (int)(key);

		// Return the key down state of the input
		return mKeyDownMap[codeID];
	}

	bool InputManager::getKeyUp(KeyCode key)
	{
		// Convert the keycode to an int
		int codeID = (int)(key);

		// Check if the map contains the key up
		if (mKeyUpMap[codeID])
		{
			// Consume the key up event and return true
			mKeyUpMap[codeID] = false;
			return true;
		}

		// The key was not found in the map, no key up
		return mKeyUpMap[codeID];
	}
}