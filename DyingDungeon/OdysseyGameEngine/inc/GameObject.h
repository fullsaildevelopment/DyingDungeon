#pragma once
#include "EngineIncludes.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "AABB.h"

namespace Odyssey
{
	class GameObject
	{
	public:
		GameObject();
	public:
		void addChild(std::shared_ptr<GameObject> child);
		void setParent(GameObject* parent);
		void setStaticObject(bool isStatic);
	public: // Accessors
		const std::vector<std::shared_ptr<GameObject>> getChildren();
		const int getChildrenCount();
		GameObject* getParent();
		bool getStaticObject();
	public: //Debug
		void enableDebug();
		void disableDebug();
		bool getDebugEnabled();
	public: // Single Component Interaction
		template<class ComponentType, typename... Args>
		void addComponent(Args&&... params);
		template<class ComponentType>
		ComponentType* getComponent();
		template<class ComponentType>
		ComponentType* getRootComponent();
		template<class ComponentType>
		bool removeComponent();
		template<class ComponentType>
		bool removeComponent(ComponentType* searching);
	public: // Multi-Component Interaction
		template<class ComponentType>
		std::vector<ComponentType*> getComponents();
		template<class ComponentType>
		int removeComponents();
	private:
		std::vector<std::unique_ptr<Component>> mComponents;
		std::vector<std::shared_ptr<GameObject>> children;
		GameObject* mParent;
		bool mDebugEnabled;
		bool mIsStatic;
	};

	// Template Functions
	template<class ComponentType, typename ...Args>
	void GameObject::addComponent(Args&& ...params)
	{
		mComponents.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));
	}
	template<class ComponentType>
	ComponentType* GameObject::getComponent()
	{
		for (auto&& component : mComponents)
		{
			if (component->isClassType(ComponentType::Type))
				return static_cast<ComponentType*>(component.get());
		}
		return std::unique_ptr<ComponentType>(nullptr).get();
	}
	template<class ComponentType>
	inline ComponentType* GameObject::getRootComponent()
	{
		GameObject* parent = mParent;

		while (parent->mParent != nullptr)
		{
			parent = parent->mParent;
		}
		return parent->getComponent<ComponentType>();
	}
	template<class ComponentType>
	bool GameObject::removeComponent()
	{
		if (mComponents.empty())
			return false;

		auto index = std::find_if(
			mComponents.begin(),
			mComponents.end(),
			[classType = ComponentType::Type](auto& component)
		{
			return component->isClassType(classType);
		});

		bool success = index != mComponents.end();

		if (success)
			mComponents.erase(index);

		return success;
	}

	template<class ComponentType>
	inline bool GameObject::removeComponent(ComponentType* searching)
	{
		if (mComponents.empty())
			return false;

		for (int i = 0; i < mComponents.size(); i++)
		{
			if (mComponents[i]->isClassType(ComponentType::Type) && mComponents[i].get() == searching)
			{
				mComponents.erase(mComponents.begin() + i);
				return true;
			}
		}
		return false;
	}

	template<class ComponentType>
	int GameObject::removeComponents()
	{
		if (mComponents.empty())
			return 0;

		int numRemoved = 0;

		bool success = true;

		while (success)
		{
			auto& index = std::find_if(
				mComponents.begin(),
				mComponents.end(),
				[classType = ComponentType::Type](auto& component)
			{ return component->isClassType(classType); });

			success = index != mComponents.end();

			if (success)
			{
				mComponents.erase(index);
				++numRemoved;
			}
		}
		return numRemoved;
	}

	template<class ComponentType>
	std::vector<ComponentType*> GameObject::getComponents()
	{
		std::vector<ComponentType*> componentsOfType;

		for (auto&& component : mComponents)
		{
			if (component->isClassType(ComponentType::Type))
				componentsOfType.emplace_back(static_cast<ComponentType*>(component.get()));
		}
		return componentsOfType;
	}
}