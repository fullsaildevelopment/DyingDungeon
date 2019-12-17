#include "Rectangle2D.h"

namespace Odyssey
{
	ELEMENT_DEFINITION( UIElement, Rectangle2D ) 
	Rectangle2D::Rectangle2D(DirectX::XMFLOAT2 centerPosition, DirectX::XMFLOAT4 color, float width, float height)
	{
		mWidth = width;
		mHeight = height;
		mFill = 1.0f;
		mShape = D2D1::RectF(centerPosition.x, centerPosition.y, centerPosition.x + width, centerPosition.y + height);
		mColor = color;
		mPosition = centerPosition;
	}

	void Rectangle2D::draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget)
	{
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
		HRESULT hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(mColor.x, mColor.y, mColor.z, mColor.w), brush.GetAddressOf());
		D2D1_COLOR_F c = brush->GetColor();
		renderTarget->FillRectangle(&mShape, brush.Get());
	}

	void Rectangle2D::addFill(float value)
	{
		mShape = D2D1::RectF(mPosition.x, mPosition.y, mPosition.x + (mWidth * mFill), mPosition.y + mHeight);
		mFill += value;
	}

	void Rectangle2D::setFill(float fill)
	{
		mShape = D2D1::RectF(mPosition.x, mPosition.y, mPosition.x + (mWidth * mFill), mPosition.y + mHeight);
		mFill = fill;
	}
}