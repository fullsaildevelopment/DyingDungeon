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
		mInputMap[input] = state;
	}

	bool InputManager::getKeyDown(char input)
	{
		return mInputMap[input];
	}
}