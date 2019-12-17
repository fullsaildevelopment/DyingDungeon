#pragma once
#include "EngineIncludes.h"
#include "UIElement.h"

namespace Odyssey
{

	class Sprite2D : public UIElement
	{
	ELEMENT_DECLARATION(Sprite2D)
	public:
		Sprite2D(DirectX::XMFLOAT2 position, LPCWSTR filename, float width = 0.0f, float height = 0.0f);
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);
	private:
		DirectX::XMFLOAT2 mPosition;
		float mWidth, mHeight;
		Microsoft::WRL::ComPtr<ID2D1Bitmap> mBitmap;
		Microsoft::WRL::ComPtr<IWICFormatConverter> mBitmapConverter;
	};
}