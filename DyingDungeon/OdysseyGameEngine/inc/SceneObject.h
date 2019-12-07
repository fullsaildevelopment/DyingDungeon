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
		void addPosition(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setScale(float x, float y, float z);
	public: // Components
		void attachAnimator();
		void attachMeshRenderer();
		void attachMeshRenderer(std::shared_ptr<Mesh> meshID, std::shared_ptr<Material> materialID);
		void attachParticleSystem();
		void attachAABB();
		Animator* getAnimator();
		Animator* getRootAnimator();
		MeshRenderer* getMeshRenderer();
		ParticleSystem* getParticleSystem();
		ParticleSystem* getRootParticleSystem();
		AABB* getAABB();
		bool hasAnimator();
		bool hasMeshRenderer();
		bool hasParticleSystem();
		bool hasAABB();
	public: // Accessors
		void getLocalTransform(DirectX::XMFLOAT4X4& localTransform);
		void getGlobalTransform(DirectX::XMFLOAT4X4& globalTransform);
		const std::vector<std::shared_ptr<SceneObject>> getChildren();
		const int getChildrenCount();
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
		ComponentType& getComponent();
		template<class ComponentType>
		bool removeComponent();
	public: // Multi-Component Interaction
		template<class ComponentType>
		std::vector<ComponentType*> getComponents();
		template<class ComponentType>
		int removeComponents();
	private:
		DirectX::XMFLOAT4X4 mWorldMatrix;
		std::vector<std::shared_ptr<SceneObject>> children;
		SceneObject* mParent;
		bool mDebugEnabled;
		// Components
		std::unique_ptr<Animator> mAnimator;
		std::unique_ptr<MeshRenderer> mMeshRenderer;
		std::unique_ptr<ParticleSystem> mParticleSystem;
		std::unique_ptr<AABB> mAABB;
	};

	template<class ComponentType, typename ...Args>
	inline void SceneObject::addComponent(Args&& ...params)
	{
		mComponents.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));
	}
	template<class ComponentType>
	inline ComponentType& SceneObject::getComponent()
	{
		for (auto&& component : mComponents)
		{
			if (component->isClassType(ComponentType::Type))
				return *static_cast<ComponentType*>(component.get());
		}
		return *std::unique_ptr<ComponentType>(nullptr);
	}
	template<class ComponentType>
	inline bool SceneObject::removeComponent()
	{
		if (mComponents.empty())
			return false;

		auto index = std::find_if(
			mComponents.begin(), 
			mComponents.end(), 
			[ classType = ComponentType::Type ](auto& component)
		{
			return component->isClassType(classType);
		});

		bool success = index != mComponents.end();

		if (success)
			mComponents.erase(index);

		return success;
	}
	template<class ComponentType>
	inline std::vector<ComponentType*> SceneObject::getComponents()
	{
		std::vector<ComponentType*> componentsOfType;

		for (auto&& component : mComponents)
		{
			if (component->isClassType(ComponentType::Type))
				componentsOfType.emplace_back(static_cast<ComponentType*>(component.get()));
		}
		return componentsOfType;
	}

	template<class ComponentType>
	inline int SceneObject::removeComponents()
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
}