#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "ReadWriteLock.h"
#include "EngineEvents.h"
#include "EventManager.h"

namespace Odyssey
{
	class Entity
	{
	public: // Rule of 3
		Entity();

	public: // Interface
		/**
		 *	Set the entity's parent entity.
		 *	@param[in] parent The entity's parent entity.
		 *	@return void
		 */
		void setParent(Entity* parent);

		/**
		 *	Get the entity's parent entity.
		 *	@param[in] void
		 *	@return Entity* A raw pointer to the entity's parent entity.
		 */
		Entity* getParent();

		/**
		 *	Add a child entity.
		 *	@param[in] child The child to add.
		 *	@return void
		 */
		void addChild(std::shared_ptr<Entity> child);

		/**
		 *	Get the entity's children.
		 *	@param[in] void
		 *	@return vector<shared_ptr<Entity>> Vector containing the entity's children.
		 */
		const std::vector<std::shared_ptr<Entity>> getChildren();

		/**
		 *	Get the entity's number of children.
		 *	@param[in] void
		 *	@return UINT The entity's number of children.
		 */
		const unsigned int getChildCount();

		/**
		 *	Set the entity's static state.
		 *	@param[in] isStatic The entity's static state.
		 *	@return void
		 */
		void setStatic(bool isStatic);

		/**
		 *	Get the entity's static state.
		 *	@param[in] void
		 *	@return bool The entity's static state.
		 */
		bool getStatic();

		/**
		 *	Set the entity's active state.
		 *	@param[in] active The entity's active state.
		 *	@return void 
		 */
		void setActive(bool active);

		/**
		 *	Get the entity's active state.
		 *	@param[in] void
		 *	@return bool The entity's active state.
		 */
		bool isActive();

		/**
		 *	Set the entity's visible state.
		 *	@param[in] active The entity's visible state.
		 *	@return void
		 */
		void setVisible(bool active);

		/**
		 *	Get the entity's visible state.
		 *	@param[in] void
		 *	@return bool The entity's visible state.
		 */
		bool isVisible();

		/**
		 *	Set the entity's debug enabled state.
		 *	@param[in] debug The entity's debug enabled state.
		 *	@return void
		 */
		void setDebugEnabled(bool debug);

		/**
		 *	Get the entity's debug enabled state.
		 *	@param[in] void
		 *	@return bool The entity's debug enabled state.
		 */
		bool getDebugEnabled();

	private: // Members
		std::vector<std::shared_ptr<Component>> mComponents;
		std::vector<std::shared_ptr<Entity>> mChildren;
		Entity* mParent;
		bool mDebugEnabled;
		bool mIsStatic;
		bool mIsActive;
		bool mIsVisible;
		ReadWriteLock mLock;

	public: // Templated interface
		/**
		 *	Add a component to the entity.
		 *	@param[in] params A constructor list for the component.
		 *	@return ComponentType* A raw pointer to the component.
		 */
		template<class ComponentType, typename... Args>
		ComponentType* addComponent(Args&&... params)
		{
			mLock.lock(LockState::Write);
			// Create a new component type shared pointer and store it in the components vector
			mComponents.emplace_back(std::make_shared<ComponentType>(std::forward<Args>(params)...));

			// Set the component's entity to this
			mComponents[mComponents.size() - 1]->setEntity(this);

			// Return a raw pointer to the created component
			ComponentType* component = static_cast<ComponentType*>(mComponents[mComponents.size() - 1].get());
			mLock.unlock(LockState::Write);
			return component;
		}

		/**
		 *	Get the first component of the matching type.
		 *	@param[in] void
		 *	@return ComponentType* A raw pointer to the component.
		 */
		template<class ComponentType>
		ComponentType* getComponent()
		{
			// Iterate through the components vector
			mLock.lock(LockState::Read);
			for (auto&& component : mComponents)
			{
				// Check the type against the templated type
				if (component->isClassType(ComponentType::Type))
				{
					ComponentType* foundComponent = static_cast<ComponentType*>(component.get());
					mLock.unlock(LockState::Read);
					return foundComponent;
				}
			}

			// Nothing found, return nullptr
			mLock.unlock(LockState::Read);
			return std::shared_ptr<ComponentType>(nullptr).get();
		}

		/**
		 *	Get all components of the matching type.
		 *	@param[in] void
		 *	@return vector<ComponentType*> A vector containing all matching components.
		 */
		template<class ComponentType>
		std::vector<ComponentType*> getComponents()
		{
			// Create a vector of component type raw pointers
			std::vector<ComponentType*> componentsOfType;

			mLock.lock(LockState::Read);
			// Iterate through the components vector
			for (auto&& component : mComponents)
			{
				// Check the type against the templated type
				if (component->isClassType(ComponentType::Type))
				{
					// Store the matching pointer
					componentsOfType.emplace_back(static_cast<ComponentType*>(component.get()));
				}

			}

			// Return the vector of component type raw pointers
			mLock.unlock(LockState::Read);
			return componentsOfType;
		}

		/**
		 *	Get the component of the matching type from the root entity.
		 *	@param[in] void
		 *	@return ComponentType* A raw pointer to the component.
		 */
		template<class ComponentType>
		ComponentType* getRootComponent()
		{
			// Check if the entity has a parent
			mLock.lock(LockState::Read);
			if (Entity* parent = mParent)
			{
				// Iterate up the list of parents until the rot is found
				while (parent->mParent != nullptr)
				{
					parent = parent->mParent;
				}
				// Return a get component on the root entity
				ComponentType* root = parent->getComponent<ComponentType>();
				mLock.unlock(LockState::Read);
				return root;
			}

			// Return a get component on this entity
			ComponentType* root = getComponent<ComponentType>();
			mLock.unlock(LockState::Read);
			return root;
		}

		/**
		 *	Remove the first component of the matching type.
		 *	@param[in] void
		 *	@return bool Whether an component was removed or not.
		 */
		template<class ComponentType>
		bool removeComponent()
		{
			// Can't remove from an empty vector
			mLock.lock(LockState::Write);
			if (mComponents.empty())
				return false;

			// Iterate through looking for a matching component type
			auto index = std::find_if(
				mComponents.begin(),
				mComponents.end(),
				[classType = ComponentType::Type](auto& component)
			{
				return component->isClassType(classType);
			});

			// Check if the search was successful
			bool success = index != mComponents.end();

			// A component type pointer was found so remove it
			if (success)
			{
				EventManager::getInstance().publish(new ComponentRemoveEvent(index->get()));
				mComponents.erase(index);
			}

			mLock.unlock(LockState::Write);
			return success;
		}

		/**
		 *	Remove a component matching the parameter pointer.
		 *	@param[in] search The pointer to search for in the UI element list.
		 *	@return bool Whether an element was removed or not.
		 */
		template<class ComponentType>
		bool removeComponent(ComponentType* searchTarget)
		{
			// Can't remove from an empty vector
			mLock.lock(LockState::Write);
			if (mComponents.empty())
				return false;

			// Iterate through looking for a matching component type
			for (int i = 0; i < mComponents.size(); i++)
			{
				// Check if the component type pointer matches the parameter pointer
				if (mComponents[i]->isClassType(ComponentType::Type) && mComponents[i].get() == searchTarget)
				{
					// Erase the component and return true
					EventManager::getInstance().publish(new ComponentRemoveEvent((mComponents.begin() + i)->get()));
					mComponents.erase(mComponents.begin() + i);
					mLock.unlock(LockState::Write);
					return true;
				}
			}
			mLock.unlock(LockState::Write);
			return false;
		}

		/**
		 *	Remove all components of the matching type.
		 *	@param[in] void
		 *	@return int The number of components removed.
		 */
		template<class ComponentType>
		int removeComponents()
		{
			// Can't remove from an empty vector
			mLock.lock(LockState::Write);
			if (mComponents.empty())
				return 0;

			// Track the number of components removed
			int numRemoved = 0;

			// Iterate through looking for a matching component type
			for (int i = 0; i < mComponents.size(); i++)
			{
				// Check if the component type matches
				if (mComponents[i]->isClassType(ComponentType::Type))
				{
					// Erase the component and increment the tracker
					EventManager::getInstance().publish(new ComponentRemoveEvent((mComponents.begin() + i)->get()));
					mComponents.erase(mComponents.begin() + i);
					mLock.unlock(LockState::Write);
					return numRemoved++;
				}
			}

			// Return the number of components removed
			mLock.unlock(LockState::Write);
			return numRemoved;
		}
	};
}