#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "UIElement.h"

namespace Odyssey
{
	class RenderDevice;

	class UICanvas : public Component
	{
		CLASS_DECLARATION(UICanvas)
	public:
		UICanvas();
		~UICanvas();
	public:
		/**
		 *	Initialize the UICanvas with default values. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Set the UICanvas's active state.
		 *	@param[in] active The new active state of the UICanvas.
		 *	@return void
		 */
		void setActive(bool active);

		/**
		 *	Get the UICanvas's active state.
		 *	@param[in] void
		 *	@return bool The active state of the UICanvas.
		 */
		bool getActive();
	public: // Single Component Interaction
		template<class ElementType, typename... Args>
		ElementType* addElement(Args&&... params);
		template<class ElementType>
		ElementType* getElement();
		template<class ElementType>
		bool removeElement();
		template<class ElementType>
		bool removeElement(ElementType* searching);
	public: // Multi-Component Interaction
		template<class ElementType>
		std::vector<ElementType*> getElements();
		template<class ElementType>
		int removeElements();
	private:
		std::vector<std::unique_ptr<UIElement>> mElements;
		bool mIsActive;
	};

	// Template Functions
	template<class ElementType, typename ...Args>
	ElementType* UICanvas::addElement(Args&& ...params)
	{
		mElements.emplace_back(std::make_unique<ElementType>(std::forward<Args>(params)...));
		return static_cast<ElementType*>(mElements[mElements.size() - 1].get());
	}
	template<class ElementType>
	ElementType* UICanvas::getElement()
	{
		for (auto&& element : mElements)
		{
			if (element->isElementType(ElementType::Type))
				return static_cast<ElementType*>(element.get());
		}
		return std::unique_ptr<ElementType>(nullptr).get();
	}
	template<class ElementType>
	bool UICanvas::removeElement()
	{
		if (mElements.empty())
			return false;

		auto index = std::find_if(
			mElements.begin(),
			mElements.end(),
			[elementType = ElementType::Type](auto& component)
		{
			return component->isElementType(elementType);
		});

		bool success = index != mElements.end();

		if (success)
			mElements.erase(index);

		return success;
	}

	template<class ElementType>
	inline bool UICanvas::removeElement(ElementType* searching)
	{
		if (mElements.empty())
			return false;

		for (int i = 0; i < mElements.size(); i++)
		{
			if (mElements[i]->isElementType(ElementType::Type) && mElements[i].get() == searching)
			{
				mElements.erase(mElements.begin() + i);
				return true;
			}
		}
		return false;
	}

	template<class ElementType>
	int UICanvas::removeElements()
	{
		if (mElements.empty())
			return 0;

		int numRemoved = 0;

		for (int i = 0; i < mElements.size(); i++)
		{
			if (mElements[i]->isElementType(ElementType::Type))
			{
				mElements.erase(mElements.begin() + i);
				return numRemoved++;
			}
		}
		return numRemoved;
	}

	template<class ElementType>
	std::vector<ElementType*> UICanvas::getElements()
	{
		std::vector<ElementType*> elementsOfType;

		for (auto&& element : mElements)
		{
			if (element->isElementType(ElementType::Type))
				elementsOfType.emplace_back(static_cast<ElementType*>(element.get()));
		}
		return elementsOfType;
	}
}