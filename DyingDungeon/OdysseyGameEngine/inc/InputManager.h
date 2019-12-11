#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class InputManager
	{
	public:
		static InputManager& getInstance();
		~InputManager() { }
	private:
		InputManager() { }
	};
}