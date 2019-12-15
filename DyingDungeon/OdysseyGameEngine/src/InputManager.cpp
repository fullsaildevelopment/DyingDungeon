#include "InputManager.h"

namespace Odyssey
{
	InputManager& InputManager::getInstance()
	{
		// Create the static Singleton instance and return it.
		static InputManager instance;
		return instance;
	}

	void InputManager::registerKeyDown(int input)
	{
		// Reject the registration if the key is already contained inside the key press or down map
		if (mKeyPressMap[input] || mKeyDownMap[input])
			return;

		// Set the key press and key down state to true
		mKeyPressMap[input] = true;
		mKeyDownMap[input] = true;
	}

	void InputManager::registerKeyUp(int input)
	{
		// Set the key press and key down state to false
		mKeyPressMap[input] = false;
		mKeyDownMap[input] = false;

		// Set the key up state to true
		mKeyUpMap[input] = true;
	}

	bool InputManager::getKeyPress(char input)
	{
		// Check if the map contains the key press
		if (mKeyPressMap[input])
		{
			// Consume the key press event and return true
			mKeyPressMap[input] = false;
			return true;
		}

		// The key was not found in the map, no key press
		return false;
	}

	bool InputManager::getKeyDown(char input)
	{
		// Return the key down state of the input
		return mKeyDownMap[input];
	}

	bool InputManager::getKeyUp(char input)
	{
		// Check if the map contains the key up
		if (mKeyUpMap[input])
		{
			// Consume the key up event and return true
			mKeyUpMap[input] = false;
			return true;
		}

		// The key was not found in the map, no key up
		return mKeyUpMap[input];
	}
}