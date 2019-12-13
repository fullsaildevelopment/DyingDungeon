#include "Component.h"
#include "GameObject.h"

namespace Odyssey
{
	const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

	bool Component::isClassType(const std::size_t classType) const
	{
		return classType == Type;
	}

	void Component::update(double deltaTime)
	{

	}

	void Component::onEnable()
	{

	}

	void Component::onDisable()
	{

	}

	GameObject* Component::getGameObject()
	{
		return mGameObject;
	}

	void Component::setGameObject(GameObject* gameObject)
	{
		mGameObject = gameObject;
	}

	void Component::setEnabled(bool enable)
	{
		mEnabled = enable;

		if (enable)
			onEnable();
		else
			onDisable();
	}

	bool Component::isEnabled()
	{
		return mEnabled;
	}
}