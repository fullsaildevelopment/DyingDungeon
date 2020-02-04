#include "UIElement.h"
#include "EventManager.h"
#include "UICanvas.h"

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
		mFill = 1.0f;
		mIsVisible = true;

		// Set mouse tracking bools
		mTrackMouseEnter = true;
		mTrackMouseExit = false;

		// Clamp the color between 0 and 1
		mColor.x /= 255.0f;
		mColor.y /= 255.0f;
		mColor.z /= 255.0f;

		// Subscribe to the element resize event
		EventManager::getInstance().subscribe(this, &UIElement::onElementResize);
		mIsRegistered = false;

		// Get the screen scale
		EventManager::getInstance().publish(new UIScaleEvent(&mScreenScale.x, &mScreenScale.y));

		// Scale the position and dimensions of the UI element to match the change in window size
		mPosition.x *= mScreenScale.x;
		mPosition.y *= mScreenScale.y;
		mDimensions.x *= mScreenScale.x;
		mDimensions.y *= mScreenScale.y;

		// Recreate the shape to match the new position and dimensions
		createShape();
		createResource();
	}

	void UIElement::onElementResize(UIElementResizeEvent* evnt)
	{
		mLock.lock(LockState::Write);

		// Scale the position and dimensions of the UI element to match the change in window size
		mScreenScale.x = evnt->xScale;
		mScreenScale.y = evnt->yScale;

		mPosition.x *= mScreenScale.x;
		mPosition.y *= mScreenScale.y;
		mDimensions.x *= mScreenScale.x;
		mDimensions.y *= mScreenScale.y;

		mLock.unlock(LockState::Write);

		// Recreate the shape to match the new position and dimensions
		createShape();
		createResource();
	}

	void UIElement::onMouseClick(MouseClickEvent* evnt)
	{
		int xPosition = evnt->xPosition;
		int yPosition = evnt->yPosition;

		mLock.lock(LockState::Write);
		if (mCanvas->isActive())
		{
			if (xPosition >= mShape.left && xPosition <= mShape.right && yPosition >= mShape.top && yPosition <= mShape.bottom)
			{
				if (mCallbackMap.count(__func__) > 0)
				{
					mCallbackMap[__func__]->execute();
				}
			}
		}
		mLock.unlock(LockState::Write);
	}

	void UIElement::onMouseMove(MouseMoveEvent* evnt)
	{
		int xPosition = evnt->mouseX;
		int yPosition = evnt->mouseY;

		mLock.lock(LockState::Read);

		bool active = mCanvas->isActive();

		bool collision = xPosition >= mShape.left && xPosition <= mShape.right && yPosition >= mShape.top && yPosition <= mShape.bottom;

		mLock.unlock(LockState::Read);

		if (active)
		{
			if (collision)
			{
				if (mTrackMouseEnter)
				{
					onMouseEnter();
				}
			}
			else if (mTrackMouseExit)
			{
				onMouseExit();
			}
		}

	}

	void UIElement::onMouseEnter()
	{
		mTrackMouseExit = true;
		mTrackMouseEnter = false;

		if (mCallbackMap.count(__func__) > 0)
		{
			mCallbackMap[__func__]->execute();
		}
	}

	void UIElement::onMouseExit()
	{
		mTrackMouseExit = false;
		mTrackMouseEnter = true;

		if (mCallbackMap.count(__func__) > 0)
		{
			mCallbackMap[__func__]->execute();
		}
	}

	void UIElement::initialize()
	{
		if (mIsRegistered == false)
		{
			EventManager::getInstance().subscribe(this, &UIElement::onMouseClick);
			EventManager::getInstance().subscribe(this, &UIElement::onMouseMove);
			mIsRegistered = true;
		}
		
		// Set mouse tracking bools
		mTrackMouseEnter = true;
		mTrackMouseExit = false;
	}

	void UIElement::setCanvas(UICanvas* canvas)
	{
		mLock.lock(LockState::Write);
		mCanvas = canvas;
		mLock.unlock(LockState::Write);
	}

	UICanvas* UIElement::getCanvas()
	{
		return mCanvas;
	}

	void UIElement::addPosition(float x, float y)
	{
		// Add the parameter x and y values to the position
		mLock.lock(LockState::Write);
		mPosition.x += x;
		mPosition.y += y;
		mLock.unlock(LockState::Write);

		// Recreate the element's shape
		createShape();
	}

	void UIElement::setPosition(float x, float y)
	{
		// Set the position to the parameter x and y values
		mLock.lock(LockState::Write);
		mPosition.x = x;
		mPosition.y = y;
		mLock.unlock(LockState::Write);

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
		mLock.lock(LockState::Write);
		mScale.x += x;
		mScale.y += y;
		mLock.unlock(LockState::Write);

		// Recreate the element's shape
		createShape();
		createResource();
	}

	void UIElement::setScale(float x, float y)
	{
		// Set the scale to the parameter x and y values
		mLock.lock(LockState::Write);
		mScale.x = x;
		mScale.y = y;
		mLock.unlock(LockState::Write);

		// Recreate the element's shape
		createShape();
		createResource();
	}

	DirectX::XMFLOAT2 UIElement::getScale()
	{
		// return the scale
		return mScale;
	}

	void UIElement::setDimensions(float width, float height)
	{
		// Set the dimensions to the parameter width and height values
		mLock.lock(LockState::Write);
		mDimensions.x = width;
		mDimensions.y = height;
		mLock.unlock(LockState::Write);

		// Recreate the element's shape
		createShape();
		createResource();
	}

	DirectX::XMFLOAT2 UIElement::getDimensions()
	{
		// Return the width and height
		return mDimensions;
	}

	void UIElement::addColor(DirectX::XMFLOAT3 color)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and add it to the color
		mLock.lock(LockState::Write);
		mColor.x += (color.x / 255.0f);
		mColor.y += (color.y / 255.0f);
		mColor.z += (color.z / 255.0f);
		mLock.unlock(LockState::Write);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		resetBrush();
	}

	void UIElement::addColor(float r, float g, float b)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and add it to the color
		mLock.lock(LockState::Write);
		mColor.x += r / 255.0f;
		mColor.y += g / 255.0f;
		mColor.z += b / 255.0f;
		mLock.unlock(LockState::Write);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		resetBrush();
	}

	void UIElement::setColor(DirectX::XMFLOAT3 color)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and assign it to the color
		mLock.lock(LockState::Write);
		mColor = DirectX::XMFLOAT4(color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, mColor.w);
		mLock.unlock(LockState::Write);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		resetBrush();
	}

	void UIElement::setColor(float r, float g, float b)
	{
		// Convert the parameter color from 0 - 255 to 0.0 - 1.0 and assign it to the color
		mLock.lock(LockState::Write);
		mColor = DirectX::XMFLOAT4(r / 255.0f, g / 255.0f, b / 255.0f, mColor.w);
		mLock.unlock(LockState::Write);
		
		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		resetBrush();
	}

	DirectX::XMFLOAT4 UIElement::getColor()
	{
		// Convert the color from 0.0 - 1.0 into 0 - 255 and return it
		mLock.lock(LockState::Read);
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(mColor.x * 255.0f, mColor.y * 255.0f, mColor.z * 255.0f, mColor.w * 255.0f);
		mLock.unlock(LockState::Read);
		return color;
	}

	void UIElement::addOpacity(float opacity)
	{
		// Add the parameter opacity to the opacity
		mLock.lock(LockState::Write);
		mColor.w += opacity;
		mLock.unlock(LockState::Write);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		resetBrush();
	}

	void UIElement::setOpacity(float opacity)
	{
		// Assign the parameter opacity to the opacity
		mLock.lock(LockState::Write);
		mColor.w = opacity;
		mLock.unlock(LockState::Write);

		// Clamp the color values between 0.0 - 1.0
		clampColor(mColor);

		// Reset the brush to be recreated on the next draw
		resetBrush();
	}

	float UIElement::getOpacity()
	{
		// Return the opacity in a range of 0.0 - 1.0
		mLock.lock(LockState::Read);
		float opacity = mColor.w;
		mLock.unlock(LockState::Read);
		return mColor.w;
	}

	void UIElement::addFill(float value)
	{
		// Add the parameter value to the fill and clamp between 0.0 and 1.0
		mLock.lock(LockState::Write);
		mFill += value;
		mFill = max(0.0f, min(mFill, 1.0f));
		mLock.unlock(LockState::Write);
	}

	void Odyssey::UIElement::setFill(float fill)
	{
		// Set the parameter value to the fill and clamp between 0.0 and 1.0
		mLock.lock(LockState::Write);
		mFill = fill;
		mFill = max(0.0f, min(mFill, 1.0f));
		mLock.unlock(LockState::Write);
	}

	void UIElement::setVisible(bool visible)
	{
		mIsVisible = visible;
	}

	bool UIElement::isVisible()
	{
		return mIsVisible;
	}

	void UIElement::clampColor(DirectX::XMFLOAT4& color)
	{
		// Clamp the 4 color channels between 0.0 - 1.0
		mLock.lock(LockState::Write);
		color.x = max(0.0f, min(color.x, 1.0f));
		color.y = max(0.0f, min(color.y, 1.0f));
		color.z = max(0.0f, min(color.z, 1.0f));
		color.w = max(0.0f, min(color.w, 1.0f));
		mLock.unlock(LockState::Write);
	}

	void UIElement::createShape()
	{
		// Create a D2D1 RectF based on the rectangle's position, dimensions, and scale values
		mLock.lock(LockState::Write);
		mShape = D2D1::RectF(mPosition.x, mPosition.y, mPosition.x + (mDimensions.x * mScale.x), mPosition.y + (mDimensions.y * mScale.y));
		mLock.unlock(LockState::Write);
	}

	void UIElement::createResource()
	{
	}

	void UIElement::resetBrush()
	{
		mLock.lock(LockState::Write);
		if (mBrush)
		{
			mBrush.Reset();
		}
		mLock.unlock(LockState::Write);
	}
}