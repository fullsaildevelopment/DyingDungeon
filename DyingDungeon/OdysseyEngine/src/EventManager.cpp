#include "EventManager.h"

namespace Odyssey
{
	EventManager::EventManager()
	{
		// Set default values
		mIsShutdown = false;
		mPublishCommands = false;

		// Subscribe to the command flush event with the process commands callback.
		subscribe(this, &EventManager::processCommands);

		// Subscribe to the engine shutdown event with the on shutdown callback.
		subscribe(this, &EventManager::onShutdown);
	}

	EventManager& EventManager::getInstance()
	{
		// Return the singleton instance
		static EventManager instance;
		return instance;
	}

	void EventManager::processCommands(CommandFlushEvent* evnt)
	{
		// Flag that we are now publishing commands
		mPublishCommands = true;

		// Iterate through the commands map
		for (std::pair<std::type_index, Event*> pair : mCommands)
		{
			// Process the commands as events
			processEvent(pair.first, pair.second);
		}

		// Clear the command list
		mCommands.clear();

		// Flag that we are no longer publishing commands
		mPublishCommands = false;
	}

	void EventManager::onShutdown(EngineShutdownEvent* evnt)
	{
		mIsShutdown = true;
	}

	bool EventManager::isCommand(Event* evnt)
	{
		// Deferred priority events are considered commands
		if (evnt->priority == EventPriority::Deferred)
			return true;
		else
			return false;
	}

	void EventManager::processEvent(std::type_index index, Event* evnt)
	{
		if (mIsShutdown)
			return;

		// Get the event handlers of all subscribers to this event
		std::shared_ptr<EventHandlerList> handlers = mSubscribers[index];

		// Check that there are handlers to notify
		if (handlers)
		{
			// Iterate through each handler
			for (auto& handler : *handlers)
			{
				// Check that the handler exists
				if (handler)
				{
					// Execute the function callback
					handler->execute(evnt);
				}
			}
		}
		
		// Delete the dynamic memory allocated by an event
		delete evnt;
		evnt = nullptr;
	}
}