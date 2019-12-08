#include "GameObject.h"

namespace Odyssey
{
	GameObject::GameObject()
	{
		// Default state for scene objects is non-renderable without a parent or children
		mParent = nullptr;
	}

	void GameObject::addChild(std::shared_ptr<GameObject> child)
	{
		children.push_back(child);
	}

	void GameObject::setParent(GameObject* parent)
	{
		mParent = parent;
	}

	const std::vector<std::shared_ptr<GameObject>> GameObject::getChildren()
	{
		return children;
	}

	const int GameObject::getChildrenCount()
	{
		return static_cast<int>(children.size());
	}

	GameObject* GameObject::getParent()
	{
		return mParent;
	}

	void GameObject::enableDebug()
	{
		mDebugEnabled = true;
	}

	void GameObject::disableDebug()
	{
		mDebugEnabled = false;
	}

	bool GameObject::getDebugEnabled()
	{
		return mDebugEnabled;
	}
}