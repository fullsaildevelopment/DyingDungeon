#include "UICanvas.h"
#include "Rectangle2D.h"
#include "Sprite2D.h"
#include "Text2D.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, UICanvas)

	void UICanvas::initialize()
	{
		for (std::shared_ptr<UIElement> element : mElements)
		{
			element->initialize();
		}
	}
}