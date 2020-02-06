#pragma once
#include "Event.h"
#include "EventHandler.h"
#include <vector>
#include <map>
#include <typeindex>
#include <memory>
#include "EngineEvents.h"

namespace Odyssey
{
	class EventManager
	{
	public: // Singleton pattern
		/**
		 *	Get the singleton instance of the event manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the event manager.
		 */
		static EventManager& getInstance();
		~EventManager() = default;
	private: // Singleton pattern
		EventManager();

	private: // Events
		/**
		 *	Event callback to process all stored commands.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void processCommands(CommandFlushEvent* evnt);

		/**
		 *	Event callback to shut down the event manager.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onShutdown(EngineShutdownEvent* evnt);

	private: // Helpers
		/**
		 *	Determine if an event is a command.
		 *	@param[in] evnt The event being invoked.
		 *	@return void
		 */
		bool isCommand(Event* evnt);

		/**
		 *	Process an event by invoking all event handler's call function.
		 *	@param[in] index The unique type index identifier of the event type.
		 *	@param[in] evnt The event being invoked.
		 *	@return void
		 */
		void processEvent(std::type_index index, Event* evnt);

	private: // Members
		typedef std::vector<std::shared_ptr<AbstractEventHandler>> EventHandlerList;
		std::map<std::type_index, std::shared_ptr<EventHandlerList>> mSubscribers;
		std::map<std::type_index, std::vector<Event*>> mCommands;
		bool mPublishCommands;
		bool mIsShutdown;

	public: // Templated interface
		/**
		 *	Publish and notify all subscribers of an event.
		 *	@param[in] evnt The event being invoked.
		 *	@return void
		 */
		template<typename EventType>
		void publish(EventType* evnt)
		{
			// Don't accept new events if shutting down.
			if (mIsShutdown)
			{
				delete evnt;
				evnt = nullptr;
				return;
			}

			// Check if the event is a command and if we are not currently publishing commands
			bool isSceneChange = (typeid(EventType) == typeid(SceneChangeEvent));

			if (isSceneChange || (mPublishCommands == false && isCommand(evnt)))
			{
				// Store the command and publish the command receive event
				// The command receive event will trigger a process commands event allowing us to process commands next frame
				mCommands[typeid(EventType)].push_back(evnt);
				publish(new CommandReceiveEvent());
			}
			else
			{
				// Process the event immediately
				processEvent(typeid(EventType), evnt);
			}
		}

		/**
		 *	Subscribe to an event to be notified when the event is fired.
		 *	@param[in] instance The instance of the object to call (typically this).
		 *	@param[in] void (T::&memberFunction)(EventType*) Function pointer to the event callback. See documentation for callback formatting.
		 *	@return void
		 */
		template<class T, class EventType>
		void subscribe(T* instance, void (T::* memberFunction)(EventType*))
		{
			// Get the handlers of all subscribers to this event type
			std::shared_ptr<EventHandlerList> handlers = mSubscribers[typeid(EventType)];

			// Check if this is the first subscriber
			if (handlers == nullptr)
			{
				// Create a new handler list and add it to the map
				handlers = std::make_shared<EventHandlerList>();
				mSubscribers[typeid(EventType)] = handlers;
			}

			// Create a new handler for the subscriber and add it to the list of handlers
			handlers->push_back(std::make_shared<EventHandler<T, EventType>>(instance, memberFunction));
		}
	};
}