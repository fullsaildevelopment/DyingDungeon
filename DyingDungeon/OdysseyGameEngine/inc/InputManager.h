#pragma once
#include "EngineIncludes.h"
#include <bitset>

namespace Odyssey
{
	enum class Keycode
	{

	};
	class InputManager
	{
	public:
		static InputManager& getInstance();
		~InputManager() { }
	public:
		void registerInput(char input, bool state);
		bool getKeyDown(char input);
	private:
		std::bitset<256> mInputMap;
		InputManager() { }
	};
}