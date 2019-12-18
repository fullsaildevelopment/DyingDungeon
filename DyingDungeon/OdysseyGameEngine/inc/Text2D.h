#pragma once
#include "EngineIncludes.h"
#include "UIElement.h"

namespace Odyssey
{
	class Text2D : public UIElement
	{
	ELEMENT_DECLARATION(Text2D)
	public:
		Text2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, std::wstring text, float fontSize, float width, float height);
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);
		void addAlpha(float alpha);
		void setAlpha(float alpha);
	private:
		Microsoft::WRL::ComPtr<IDWriteFactory> mFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mFormat;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mBrush;
		D2D_RECT_F mShape;
		std::wstring mText;
	};
}