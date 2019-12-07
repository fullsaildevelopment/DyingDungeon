#pragma once
#include "EngineIncludes.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "AABB.h"

namespace Odyssey
{
	class SceneObject
	{
	public:
		SceneObject();
	public:
		void importModel(const char* filename);
	public: // Components
		void attachParticleSystem();
		void attachAABB();
		ParticleSystem* getParticleSystem();
		ParticleSystem* getRootParticleSystem();
		AABB* getAABB();
		bool hasParticleSystem();
		bool hasAABB();
	public: // Accessors
		const std::vector<std::shared_ptr<SceneObject>> getChildren();
		const int getChildrenCount();
		SceneObject* getParent();
	public: //Debug
		void enableDebug();
		void disableDebug();
		bool getDebugEnabled();
	public: // Components
		std::vector<std::unique_ptr<Component>> mComponents;
	public: // Single Component Interaction
		template<class ComponentType, typename... Args>
		void addComponent(Args&&... params);
		template<class ComponentType>
		ComponentType* getComponent();
		template<class ComponentType>
		ComponentType* getRootComponent();
		template<class ComponentType>
		bool removeComponent();
	public: // Multi-Component Interaction
		template<class ComponentType>
		std::vector<ComponentType*> getComponents();
		template<class ComponentType>
		int removeComponents();
	private:
		std::vector<std::shared_ptr<SceneObject>> children;
		SceneObject* mParent;
		bool mDebugEnabled;
		// Components
		std::unique_ptr<Animator> mAnimator;
		std::unique_ptr<MeshRenderer> mMeshRenderer;
		std::unique_ptr<ParticleSystem> mParticleSystem;
		std::unique_ptr<AABB> mAABB;
	};

	// Template Functions
	template<class ComponentType, typename ...Args>
	void SceneObject::addComponent(Args&& ...params)
	{
		mComponents.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));
	}
	template<class ComponentType>
	ComponentType* SceneObject::getComponent()
	{
		for (auto&& component : mComponents)
		{
			if (component->isClassType(ComponentType::Type))
				return static_cast<ComponentType*>(component.get());
		}
		return std::unique_ptr<ComponentType>(nullptr).get();
	}
	template<class ComponentType>
	inline ComponentType* SceneObject::getRootComponent()
	{
		SceneObject* parent = mParent;

		while (parent->mParent != nullptr)
		{
			parent = parent->mParent;
		}
		return parent->getComponent<ComponentType>();
	}
	template<class ComponentType>
	bool SceneObject::removeComponent()
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
	int SceneObject::removeComponents()
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
	std::vector<ComponentType*> SceneObject::getComponents()
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