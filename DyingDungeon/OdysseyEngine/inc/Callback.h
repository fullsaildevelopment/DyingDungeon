#pragma once

class AbstractCallbackHandler
{
private:
	virtual void call() = 0;

public:
	void execute()
	{
		call();
	}
};

template<class T>
class CallbackHandler : public AbstractCallbackHandler
{
public:
	typedef void (T::* MemberFunction)();

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
	CallbackHandler(T* instance, MemberFunction memberFunction) : mInstance{ instance }, mFunction{ memberFunction }
	{

	}

	/**
	 *	Execute the event handler’s member function.
	 *	@param[in] evnt A raw pointer to the invoking event.
	 *	@return void
	 */
	void call()
	{
		(mInstance->*mFunction)();
	}
};