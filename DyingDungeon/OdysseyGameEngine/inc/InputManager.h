#pragma once
#include "EngineIncludes.h"
#include <bitset>

namespace Odyssey
{
	class InputManager
	{
	public:
		~InputManager() { }
	public:
		/**
		 *	Get the Singleton instance of the InputManager.
		 *	@param[in] void
		 *	@return InputManager& The Singleton instance of the InputManager.
		 */
		static InputManager& getInstance();

		/**
		 *	Register a key-down event with the InputManager.
		 *	@param[in] input Keycode ID of the key.
		 *	@return void
		 */
		void registerKeyDown(int input);

		/**
		 *	Register a key-up event with the InputManager.
		 *	@param[in] input Keycode ID of the key.
		 *	@return void
		 */
		void registerKeyUp(int input);

		/**
		 *	Get the state of a key being pressed.
		 *	@param[in] input Keycode ID of the key.
		 *	@return bool The state of the input key press.
		 */
		bool getKeyPress(char input);

		/**
		 *	Get the state of a key being held down.
		 *	@param[in] input Keycode ID of the key.
		 *	@return bool The state of the input key down.
		 */
		bool getKeyDown(char input);

		/**
		 *	Get the state of a key being released.
		 *	@param[in] input Keycode ID of the key.
		 *	@return bool The state of the input key up.
		 */
		bool getKeyUp(char input);
	private:
		std::bitset<256> mKeyPressMap;
		std::bitset<256> mKeyDownMap;
		std::bitset<256> mKeyUpMap;
		InputManager() { }
	};
}