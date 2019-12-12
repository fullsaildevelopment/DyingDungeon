#include "InputManager.h"

namespace Odyssey
{
	InputManager& InputManager::getInstance()
	{
		static InputManager instance;
		return instance;
	}

	void InputManager::registerInput(char input, bool state)
	{
		if (mKeyPressMap[input] || mKeyDownMap[input])
			return;
		mKeyPressMap[input] = state;
		mKeyDownMap[input] = state;
	}

	void InputManager::unregisterInput(char input)
	{
		mKeyPressMap[input] = false;
		mKeyDownMap[input] = false;
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