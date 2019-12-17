#pragma once
#include "EngineIncludes.h"
#include "UIElement.h"

namespace Odyssey
{
	class Rectangle2D : public UIElement
	{
	ELEMENT_DECLARATION(Rectangle2D)
	public:
		Rectangle2D(DirectX::XMFLOAT2 centerPosition, DirectX::XMFLOAT4 color, float width, float height);
		void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);
		void addFill(float value);
		void setFill(float fill);
	private:
		DirectX::XMFLOAT2 mPosition;
		float mWidth;
		float mHeight;
		float mFill;
		D2D_RECT_F mShape;
	};
}