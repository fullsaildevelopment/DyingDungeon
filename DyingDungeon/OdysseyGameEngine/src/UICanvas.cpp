#include "UICanvas.h"
#include "Rectangle2D.h"
#include "Sprite2D.h"
#include "Text2D.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, UICanvas)
	UICanvas::UICanvas()
	{
		mIsActive = true;
	}

	UICanvas::~UICanvas()
	{
		removeElements<UIElement>();
	}

	void UICanvas::initialize()
	{

	}

	void UICanvas::setActive(bool active)
	{
		mIsActive = active;
	}

	bool UICanvas::getActive()
	{
		return mIsActive;
	}
}