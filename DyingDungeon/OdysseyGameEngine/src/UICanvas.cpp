#include "UICanvas.h"
#include "Rectangle2D.h"
#include "Sprite2D.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, UICanvas)
	UICanvas::UICanvas()
	{
		mIsActive = true;
		mUIElements.clear();
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

	void UICanvas::addUIElement(std::shared_ptr<UIElement> element)
	{
		mUIElements.push_back(element);
	}

	void UICanvas::addSprite2D(DirectX::XMFLOAT2 position, LPCWSTR filename, float width, float height)
	{
		std::shared_ptr<Sprite2D> sprite = std::make_shared<Sprite2D>(position, filename, width, height);
		mUIElements.push_back(sprite);
	}

	void UICanvas::addRectangle2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, float width, float height)
	{
		std::shared_ptr<Rectangle2D> rect = std::make_shared<Rectangle2D>(position, color, width, height);
		mUIElements.push_back(rect);
	}

	std::vector<std::shared_ptr<UIElement>> UICanvas::getUIElements()
	{
		return mUIElements;
	}
}