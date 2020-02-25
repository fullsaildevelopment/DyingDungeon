#include "Entity.h"
#include "Scene.h"

namespace Odyssey
{
	Entity::Entity()
	{
		// Default state for scene objects is non-renderable without a parent or children
		mScene = nullptr;
		mParent = nullptr;
		mIsStatic = true;
		mDebugEnabled = false;
		mIsActive = true;
		mIsVisible = true;
		mChildren.clear();
	}

	Entity::Entity(Scene* scene)
	{
		// Default state for scene objects is non-renderable without a parent or children
		mScene = scene;
		mParent = nullptr;
		mIsStatic = true;
		mDebugEnabled = false;
		mIsActive = true;
		mIsVisible = true;
		mChildren.clear();
	}

	Entity::Entity(const Entity& copy)
	{
		mScene = copy.mScene;
		mParent = copy.mParent;
		mIsStatic = copy.mIsStatic;
		mDebugEnabled = copy.mDebugEnabled;
		mIsActive = copy.mIsActive;
		mIsVisible = copy.mIsVisible;
		mChildren.clear();

		for (std::shared_ptr<Component> component : copy.mComponents)
		{
			mComponents.push_back(component->clone());
			mComponents[mComponents.size() - 1]->setEntity(this);
		}
	}

	Entity& Entity::operator=(const Entity& copy)
	{
		mScene = copy.mScene;
		mParent = copy.mParent;
		mIsStatic = copy.mIsStatic;
		mDebugEnabled = copy.mDebugEnabled;
		mIsActive = copy.mIsActive;
		mIsVisible = copy.mIsVisible;
		mChildren.clear();

		for (std::shared_ptr<Component> component : copy.mComponents)
		{
			mComponents.push_back(component->clone());
			mComponents[mComponents.size() - 1]->setEntity(this);
		}

		return *(this);
	}

	void Entity::setScene(Scene* scene)
	{
		mScene = scene;

		for (std::shared_ptr<Component> component : mComponents)
		{
			mScene->addComponent(component.get());
		}
	}

	Scene* Entity::getScene()
	{
		return mScene;
	}

	void Entity::addChild(Entity* child)
	{
		child->setParent(this);
		mChildren.push_back(child);
	}

	void Entity::setParent(Entity* parent)
	{
		mParent = parent;
	}

	void Entity::setStatic(bool isStatic)
	{
		mIsStatic = isStatic;

		for (Entity* child : mChildren)
		{
			child->setStatic(isStatic);
		}
	}

	const std::vector<Entity*> Entity::getChildren()
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

	void Entity::setActive(bool active)
	{
		mIsActive = active;
		
		for (Entity* child : mChildren)
		{
			child->setActive(active);
		}
	}

	bool Entity::isActive()
	{
		return mIsActive;
	}

	void Entity::setVisible(bool active)
	{
		mIsVisible = active;

		for (Entity* child : mChildren)
		{
			child->setVisible(active);
		}
	}

	bool Entity::isVisible()
	{
		return mIsVisible;
	}

	bool Entity::getDebugEnabled()
	{
		return mDebugEnabled;
	}
}