#pragma once
#include "EngineIncludes.h"
#include "UIElement.h"

namespace Odyssey
{
	class Geometry2D : public UIElement
	{
	public:
		enum class GeometryType
		{
			Rectangle
		};
		void createRectangle(float width, float height, const DirectX::XMFLOAT2& centerPoint, const DirectX::XMFLOAT4& color);
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);
	private:
		DirectX::XMFLOAT2 mDimensions;
		D2D1_RECT_F mShape;
	};
}