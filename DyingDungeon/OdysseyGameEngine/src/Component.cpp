#include "Component.h"
#include "SceneObject.h"

namespace Odyssey
{
	void Component::update(double deltaTime)
	{

	}

	void Component::onEnable()
	{

	}

	void Component::onDisable()
	{

	}

	SceneObject* Component::getParent()
	{
		return mParent;
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