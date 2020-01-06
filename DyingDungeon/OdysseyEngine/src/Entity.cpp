#include "Entity.h"

namespace Odyssey
{
	Entity::Entity()
	{
		// Default state for scene objects is non-renderable without a parent or children
		mParent = nullptr;
	}

	void Entity::addChild(std::shared_ptr<Entity> child)
	{
		mChildren.push_back(child);
	}

	void Entity::setParent(Entity* parent)
	{
		mParent = parent;
	}

	void Entity::setStatic(bool isStatic)
	{
		mIsStatic = isStatic;

		for (std::shared_ptr<Entity> child : mChildren)
		{
			child->setStatic(isStatic);
		}
	}

	const std::vector<std::shared_ptr<Entity>> Entity::getChildren()
	{
		return mChildren;
	}

	const UINT Entity::getChildCount()
	{
		return static_cast<int>(mChildren.size());
	}

	Entity* Entity::getParent()
	{
		return mParent;
	}

	void Entity::setDebugEnabled(bool debug)
	{
		mDebugEnabled = debug;
	}

	bool Entity::getStatic()
	{
		return mIsStatic;
	}

	bool Entity::getDebugEnabled()
	{
		return mDebugEnabled;
	}
}