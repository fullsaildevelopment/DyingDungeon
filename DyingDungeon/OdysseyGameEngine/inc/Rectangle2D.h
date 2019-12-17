#pragma once
#include "EngineIncludes.h"
#include "UIElement.h"

namespace Odyssey
{
	class Rectangle2D : public UIElement
	{
	public:
		Rectangle2D(DirectX::XMFLOAT2 centerPosition, DirectX::XMFLOAT4 color, float width, float height);
		void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);
	private:
		D2D_RECT_F mShape;
	};
}