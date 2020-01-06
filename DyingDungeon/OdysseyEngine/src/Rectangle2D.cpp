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

	void Rectangle2D::draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
	{
		// Check if color lerp is enabled
		if (mColorLerpEnabled)
		{
			// Calculate a lerped color
			DirectX::XMFLOAT4 brushColor = colorLerp(mZeroFillColor, mColor, mFill);

			// Reset and recreate the brush with the lerped color
			mBrush.Reset();
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

		// Copy the current shape and alter it's width using the fill
		//D2D_RECT_F fillShape = mShape;
		mShape.right *= mFill;

		// Draw the filled rectangle
		context->FillRectangle(&mShape, mBrush.Get());
	}

	void Rectangle2D::addFill(float value)
	{
		// Add the parameter value to the fill and clamp between 0.0 and 1.0
		mFill += value;
		mFill = max(0.0f, min(mFill, 1.0f));
	}

	void Rectangle2D::setFill(float fill)
	{
		// Set the parameter value to the fill and clamp between 0.0 and 1.0
		mFill = fill;
		mFill = max(0.0f, min(mFill, 1.0f));
	}

	void Rectangle2D::enableColorLerp(DirectX::XMFLOAT3 zeroFillColor)
	{
		// Enable color lerp, store the zero fill color and flag whether alpha lerp is enabled.
		mColorLerpEnabled = true;
		mZeroFillColor = DirectX::XMFLOAT4(zeroFillColor.x / 255.0f, zeroFillColor.y / 255.0f, zeroFillColor.z / 255.0f, mColor.w);
		clampColor(mZeroFillColor);
	}

	void Rectangle2D::setZeroFillOpacity(float opacity)
	{
		mZeroFillColor.w = opacity;
		clampColor(mZeroFillColor);
	}

	void Rectangle2D::disableColorLerp()
	{
		// Disable color lerp and alpha lerp.
		mColorLerpEnabled = false;
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