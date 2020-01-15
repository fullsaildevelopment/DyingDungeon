#pragma once

namespace Odyssey
{
	enum class EventPriority
	{
		Immediate,
		Deferred
	};

	class Event
	{
	public:
		EventPriority priority;
		virtual ~Event() = default;
	};
}