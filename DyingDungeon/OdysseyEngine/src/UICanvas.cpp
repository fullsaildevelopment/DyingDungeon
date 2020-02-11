#include "UICanvas.h"
#include "Rectangle2D.h"
#include "Sprite2D.h"
#include "Text2D.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, UICanvas)

	std::shared_ptr<Component> UICanvas::clone() const
	{
		return std::make_shared<UICanvas>(*this);
	}

	UICanvas::UICanvas(const UICanvas& copy)
	{
		for (std::shared_ptr<UIElement> element : copy.mElements)
		{
			mElements.push_back(element->clone());
			mElements[mElements.size() - 1]->setCanvas(this);
		}
	}

	UICanvas& UICanvas::operator=(const UICanvas& copy)
	{
		for (std::shared_ptr<UIElement> element : copy.mElements)
		{
			mElements.push_back(element->clone());
			mElements[mElements.size() - 1]->setCanvas(this);
		}

		return *(this);
	}

	void UICanvas::initialize()
	{
		for (std::shared_ptr<UIElement> element : mElements)
		{
			element->initialize();
		}
	}
}