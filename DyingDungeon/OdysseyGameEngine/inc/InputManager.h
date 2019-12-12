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
		void unregisterInput(char input);
		bool getKeyPress(char input);
		bool getKeyDown(char input);
		bool getKeyUp(char input);
	private:
		std::bitset<256> mKeyPressMap;
		std::bitset<256> mKeyDownMap;
		std::bitset<256> mKeyUpMap;
		InputManager() { }
	};
}