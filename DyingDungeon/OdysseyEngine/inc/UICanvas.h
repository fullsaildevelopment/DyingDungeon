#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "UIElement.h"
#include "Scene.h"
#include "Entity.h"

namespace Odyssey
{
	class RenderDevice;

	class UICanvas : public Component
	{
		CLASS_DECLARATION(UICanvas)

	public:
		UICanvas() = default;
		virtual std::shared_ptr<Component> clone() const;
		//UICanvas(const UICanvas& copy);
	public: // Rule of 3
		virtual void initialize();

	private: // Members
		std::vector<std::shared_ptr<UIElement>> mElements;

	public: // Templated interface
		/**
		 *	Add a UI element to the canvas.
		 *	@param[in] params A constructor list for the UI element.
		 *	@return ElementType* A raw pointer to the UI element.
		 */
		template<class ElementType, typename... Args>
		ElementType* addElement(Args&&... params)
		{
			// Create a new element type shared pointer and store it in the elements vector
			mElements.emplace_back(std::make_shared<ElementType>(std::forward<Args>(params)...));

			mElements[mElements.size() - 1]->setCanvas(this);

			if (mEntity->getScene())
			{
				mEntity->getScene()->addElement(mElements[mElements.size() - 1].get());
			}
			
			// Return a raw pointer to the created element type
			return static_cast<ElementType*>(mElements[mElements.size() - 1].get());
		}

		/**
		 *	Get the first UI element of the matching type.
		 *	@param[in] void
		 *	@return ElementType* A raw pointer to the UI element.
		 */
		template<class ElementType>
		ElementType* getElement()
		{
			// Iterate through the elements vector
			for (auto&& element : mElements)
			{
				// Check the type against the templated type
				if (element->isElementType(ElementType::Type))
				{
					// Return the matching pointer
					return static_cast<ElementType*>(element.get());
				}
			}

			// Nothing found, return nullptr
			return std::shared_ptr<ElementType>(nullptr).get();
		}

		/**
		 *	Get all UI elements of the matching type.
		 *	@param[in] void
		 *	@return vector<ElementType*> A vector containing all matching UI elements.
		 */
		template<class ElementType>
		std::vector<ElementType*> getElements()
		{
			// Create a vector of element type raw pointers
			std::vector<ElementType*> elementsOfType;

			// Iterate through the element vector
			for (auto&& element : mElements)
			{
				// Check the type against the templated type
				if (element->isElementType(ElementType::Type))
				{
					// Store the matching pointer
					elementsOfType.emplace_back(static_cast<ElementType*>(element.get()));
				}
			}

			// Return the vector of element type raw pointers
			return elementsOfType;
		}

		/**
		 *	Remove the first UI element of the matching type.
		 *	@param[in] void
		 *	@return bool Whether an element was removed or not.
		 */
		template<class ElementType>
		bool removeElement()
		{
			// Can't remove from an empty vector
			if (mElements.empty())
				return false;

			// Iterate through looking for a matching element type
			auto index = std::find_if(
				mElements.begin(),
				mElements.end(),
				[elementType = ElementType::Type](auto& component)
			{
				return component->isElementType(elementType);
			});

			// Check if the search was successful
			bool success = index != mElements.end();

			// An element type pointer was found so remove it
			if (success)
			{
				mElements.erase(index);
			}

			return success;
		}

		/**
		 *	Remove a UI element matching the parameter pointer.
		 *	@param[in] searchTarget The pointer to search for in the UI element list.
		 *	@return bool Whether an element was removed or not.
		 */
		template<class ElementType>
		bool removeElement(ElementType* searchTarget)
		{
			// Can't remove from an empty vector
			if (mElements.empty())
				return false;

			// Iterate through looking for a matching element type
			for (int i = 0; i < mElements.size(); i++)
			{
				// Check if the element type pointer matches the parameter pointer
				if (mElements[i]->isElementType(ElementType::Type) && mElements[i].get() == searchTarget)
				{
					// Erase the element and return true
					mElements.erase(mElements.begin() + i);
					return true;
				}
			}
			return false;
		}

		/**
		 *	Remove all UI elements of the matching type.
		 *	@param[in] void
		 *	@return int The number of UI elements removed.
		 */
		template<class ElementType>
		int removeElements()
		{
			// Can't remove from an empty vector
			if (mElements.empty())
				return 0;

			// Track the number of elements removed
			int numRemoved = 0;

			// Iterate through looking for a matching element type
			for (int i = 0; i < mElements.size(); i++)
			{
				// Check if the element type matches
				if (mElements[i]->isElementType(ElementType::Type))
				{
					// Erase the element and increment the tracker
					mElements.erase(mElements.begin() + i);
					return numRemoved++;
				}
			}

			// Return the number of elements removed
			return numRemoved;
		}
	};
}