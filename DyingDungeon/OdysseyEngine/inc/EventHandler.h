#pragma once
#include "Event.h"

namespace Odyssey
{
	class AbstractEventHandler
	{
	private:
		virtual void call(Event* evnt) = 0;

	public:
		void execute(Event* evnt)
		{
			call(evnt);
		}
	};

	template<class T, class EventType>
	class EventHandler : public AbstractEventHandler
	{
	public:
		typedef void (T::* MemberFunction)(EventType*);
	private:
		T* mInstance;
		MemberFunction mFunction;

	public:
		/**
		 *	Create an event handler with the invoking instance and member function
		 *	@param[in] instance Templated raw pointer to the instance containing the member function.
		 *	@param[in] memberFunction The member function of the instance to call when the event handler is invoked.
		 *	@return void
		 */
		EventHandler(T* instance, MemberFunction memberFunction) : mInstance{ instance }, mFunction{ memberFunction }
		{

		}

		/**
		 *	Execute the event handler’s member function.
		 *	@param[in] evnt A raw pointer to the invoking event.
		 *	@return void
		 */
		void call(Event* evnt)
		{
			(mInstance->*mFunction)(static_cast<EventType*>(evnt));
		}
	};
}