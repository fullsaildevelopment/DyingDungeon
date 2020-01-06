#include "UIElement.h"
#include "EventManager.h"

namespace Odyssey
{
	const std::size_t UIElement::Type = std::hash<std::string>()(TO_STRING(Component));

	bool UIElement::isElementType(const std::size_t elementType) const
	{
		return elementType == Type;
	}

	UIElement::UIElement(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, UINT width, UINT height)
		: mPosition(position), mColor(color)
	{
		// Initialize the dimensions with the parameter width and height
		mDimensions = DirectX::XMFLOAT2(static_cast<float>(width), static_cast<float>(height));

		// Set the default scale
		mScale = DirectX::XMFLOAT2(1.0f, 1.0f);

		// Create the rectangle
		createShape();

		// Subscribe to the element resize event
		EventManager::getInstance().subscribe(this, &UIElement::onElementResize);
	}

	void UIElement::onElementResize(UIElementResizeEvent* evnt)
	{
		// Scale the position and dimensions of the UI element to match the change in window size
		mPosition.x *= evnt->xScale;
		mPosition.y *= evnt->yScale;
		mDimensions.x *= evnt->xScale;
		mDimensions.y *= evnt->yScale;

		// Recreate the shape to match the new position and dimensions
		createShape();
	}

	void UIElement::setCanvas(UICanvas* canvas)
	{
		mCanvas = canvas;
	}

	UICanvas* UIElement::getCanvas()
	{
		return mCanvas;
	}

	void UIElement::addPosition(float x, float y)
	{
		// Add the parameter x and y values to the position
		mPosition.x += x;
		mPosition.y += y;

		// Recreate the element's shape
		createShape();
	}

	void UIElement::setPosition(float x, float y)
	{
		// Set the position to the parameter x and y values
		mPosition.x = x;
		mPosition.y = y;

		// Recreate the element's shape
		createShape();
	}

	DirectX::XMFLOAT2 UIElement::getPosition()
	{
		// Return the position
		return mPosition;
	}

	void UIElement::addScale(float x, float y)
	{
		// Add the parameter x and y values to the scale
		mScale.x += x;
		mScale.y += y;

		// Recreate the element's shape
		createShape();
	}

	void UIElement::setScale(float x, float y)
	{
		// Set the scale to the parameter x and y values
		mScale.x = x;
		mScale.y = y;

		// Recreate the element's shape
		createShape();
	}

	DirectX::XMFLOAT2 UIElement::getScale()
	{
		// return the scale
		return mScale;
	}

	void UIElement::setDimensions(float width, float height)
	{
		// Set the dimensions to the parameter width and height values
		mDimensions.x = width;
		mDimensions.y = height;

		// Recreate the element's shape
		createShape();
	}

	DirectX::XMFLOAT2 UIElement::getDimensions()
	{
		// Return the width and height
		return mDimensions;
	}

	void UIElement::addColor(DirectX::XMFLOAT3 color)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and add it to the color
		mColor.x += (color.x / 255.0f);
		mColor.y += (color.y / 255.0f);
		mColor.z += (color.z / 255.0f);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		mBrush.Reset();
	}

	void UIElement::addColor(float r, float g, float b)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and add it to the color
		mColor.x += r / 255.0f;
		mColor.y += g / 255.0f;
		mColor.z += b / 255.0f;

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		mBrush.Reset();
	}

	void UIElement::setColor(DirectX::XMFLOAT3 color)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and assign it to the color
		mColor = DirectX::XMFLOAT4(color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, mColor.w);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		mBrush.Reset();
	}

	void UIElement::setColor(float r, float g, float b)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and assign it to the color
		mColor = DirectX::XMFLOAT4(r / 255.0f, g / 255.0f, b / 255.0f, mColor.w);
		
		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		mBrush.Reset();
	}

	DirectX::XMFLOAT4 UIElement::getColor()
	{
		// Convert the color from 0.0 - 1.0 into 0 - 255 and return it
		return { mColor.x * 255.0f, mColor.y * 255.0f, mColor.z * 255.0f, mColor.w * 255.0f };
	}

	void UIElement::addOpacity(float opacity)
	{
		// Add the parameter opacity to the opacity
		mColor.w += opacity;

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		mBrush.Reset();
	}

	void UIElement::setOpacity(float opacity)
	{
		// Assign the parameter opacity to the opacity
		mColor.w = opacity;

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		mBrush.Reset();
	}

	float UIElement::getOpacity()
	{
		// Return the opacity in a range of 0.0 - 1.0
		return mColor.w;
	}

	void UIElement::clampColor(DirectX::XMFLOAT4& color)
	{
		// Clamp the 4 color channels between 0.0 - 1.0
		color.x = max(0.0f, min(color.x, 1.0f));
		color.y = max(0.0f, min(color.y, 1.0f));
		color.z = max(0.0f, min(color.z, 1.0f));
		color.w = max(0.0f, min(color.w, 1.0f));
	}

	void UIElement::createShape()
	{
		// Create a D2D1 RectF based on the rectangle's position, dimensions, and scale values
		mShape = D2D1::RectF(mPosition.x, mPosition.y, mPosition.x + (mDimensions.x * mScale.x), mPosition.y + (mDimensions.y * mScale.y));
	}
}