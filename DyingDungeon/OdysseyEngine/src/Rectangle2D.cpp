#include "Rectangle2D.h"
#include "EventManager.h"

namespace Odyssey
{
	ELEMENT_DEFINITION(UIElement, Rectangle2D)
		Rectangle2D::Rectangle2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, UINT width, UINT height)
		: UIElement(position, color, width, height)
	{
		// Set default values
		mFill = 1.0f;
		mColorLerpEnabled = false;
		mZeroFillColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	std::shared_ptr<UIElement> Rectangle2D::clone() const
	{
		return std::make_shared<Rectangle2D>(*this);
	}

	void Rectangle2D::draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
	{
		// Check if color lerp is enabled
		mLock.lock(LockState::Write);
		if (mColorLerpEnabled)
		{
			// Calculate a lerped color
			DirectX::XMFLOAT4 brushColor = colorLerp(mZeroFillColor, mColor, mFill);

			// Reset and recreate the brush with the lerped color
			if (mBrush)
			{
				mBrush = nullptr;
			}
				
			HRESULT hr = context->CreateSolidColorBrush(D2D1::ColorF(brushColor.x, brushColor.y, brushColor.z, brushColor.w), mBrush.GetAddressOf());
			assert(!FAILED(hr));
		}

		// Check if the brush needs to be recreated
		if (mBrush == nullptr)
		{
			// Recreate the brush with the base color
			HRESULT hr = context->CreateSolidColorBrush(D2D1::ColorF(mColor.x, mColor.y, mColor.z, mColor.w), mBrush.GetAddressOf());
			assert(!FAILED(hr));
		}

		// Copy the shape and apply the fill to the width
		D2D_RECT_F fillShape = mShape;
		float width = fillShape.right - fillShape.left;
		fillShape.right = mShape.left + (width * mFill);

		// Draw the filled rectangle
		context->FillRectangle(&fillShape, mBrush.Get());
		mLock.unlock(LockState::Write);
	}

	void Rectangle2D::enableColorLerp(DirectX::XMFLOAT3 zeroFillColor)
	{
		// Enable color lerp, store the zero fill color and flag whether alpha lerp is enabled.
		mLock.lock(LockState::Write);
		mColorLerpEnabled = true;
		mZeroFillColor = DirectX::XMFLOAT4(zeroFillColor.x / 255.0f, zeroFillColor.y / 255.0f, zeroFillColor.z / 255.0f, mColor.w);
		mLock.unlock(LockState::Write);
		clampColor(mZeroFillColor);
	}

	void Rectangle2D::setZeroFillOpacity(float opacity)
	{
		mLock.lock(LockState::Write);
		mZeroFillColor.w = opacity;
		mLock.unlock(LockState::Write);
		clampColor(mZeroFillColor);
	}

	void Rectangle2D::disableColorLerp()
	{
		// Disable color lerp and alpha lerp.
		mLock.lock(LockState::Write);
		mColorLerpEnabled = false;
		mLock.unlock(LockState::Write);
	}

	DirectX::XMFLOAT4 Rectangle2D::colorLerp(DirectX::XMFLOAT4 zeroFillColor, DirectX::XMFLOAT4 maxFillColor, float ratio)
	{
		// Lerp the r g b a channels between the zero and max fill colors using the parameter ratio.
		DirectX::XMFLOAT4 finalColor;
		finalColor.x = (1.0f - ratio) * zeroFillColor.x + (ratio * maxFillColor.x);
		finalColor.y = (1.0f - ratio) * zeroFillColor.y + (ratio * maxFillColor.y);
		finalColor.z = (1.0f - ratio) * zeroFillColor.z + (ratio * maxFillColor.z);
		finalColor.w = (1.0f - ratio) * zeroFillColor.w + (ratio * maxFillColor.w);

		// Return the resulting lerped color
		return finalColor;
	}
}