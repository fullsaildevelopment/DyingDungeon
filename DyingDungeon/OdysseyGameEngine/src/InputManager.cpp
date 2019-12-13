#include "InputManager.h"

namespace Odyssey
{
	InputManager& InputManager::getInstance()
	{
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
		if (mKeyPressMap[input])
		{
			mKeyPressMap[input] = false;
			return true;
		}
		return false;
	}

	bool InputManager::getKeyDown(char input)
	{
		return mKeyDownMap[input];
	}

	bool InputManager::getKeyUp(char input)
	{
		if (mKeyUpMap[input])
		{
			mKeyUpMap[input] = false;
			return true;
		}
		return mKeyUpMap[input];
	}
}