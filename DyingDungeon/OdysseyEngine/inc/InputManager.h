#pragma once
#include "EngineIncludes.h"
#include "Event.h"
#include "Keycode.h"
#include <bitset>

namespace Odyssey
{
	class KeypressEvent : public Event
	{
	public:
		KeyCode keyCode;
		KeypressEvent(KeyCode code) : keyCode(code) { }
	};

	class KeyUpEvent : public Event
	{
	public:
		KeyCode keyCode;
		KeyUpEvent(KeyCode code) : keyCode(code) { }
	};

	class InputManager
	{
	public: // Singleton pattern
		/**
		 *	Get the singleton instance of the input manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the input manager.
		 */
		static InputManager& getInstance();
		~InputManager() { }

	private: // Singleton pattern
		InputManager();

	public: // Events
		/**
		 *	Event callback to register a key press.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onKeypress(KeypressEvent* evnt);

		/**
		 *	Event callback to register a key up.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onKeyUp(KeyUpEvent* evnt);

	public: // Interface
		/**
		 *	Get the state of a key being pressed.
		 *	@param[in] input Keycode ID of the key.
		 *	@return bool The state of the input key press.
		 */
		bool getKeyPress(KeyCode key);

		/**
		 *	Get the state of a key being held down.
		 *	@param[in] input Keycode ID of the key.
		 *	@return bool The state of the input key down.
		 */
		bool getKeyDown(KeyCode key);

		/**
		 *	Get the state of a key being released.
		 *	@param[in] input Keycode ID of the key.
		 *	@return bool The state of the input key up.
		 */
		bool getKeyUp(KeyCode key);

	private: // Members
		std::bitset<256> mKeyPressMap;
		std::bitset<256> mKeyDownMap;
		std::bitset<256> mKeyUpMap;
	};
}