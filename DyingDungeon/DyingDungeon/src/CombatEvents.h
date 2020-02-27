#pragma once
#include "Event.h"
#include "EventManager.h"

class SetNewTargetEvent : public Odyssey::Event
{
public:
	enum class Player
	{
		Hero1,
		Hero2,
		Hero3,
		Enemy1,
		Enemy2,
		Enemy3,
	};

	Player mPlayerToTarget;
	SetNewTargetEvent(Player _playerToTarget)
	{
		mPlayerToTarget = _playerToTarget;
	}
};