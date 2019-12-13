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
		void registerKeyDown(int input);
		void registerKeyUp(int input);
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