#pragma once
#include "EngineIncludes.h"
#include "UIElement.h"

namespace Odyssey
{
	class Text2D : public UIElement
	{
	public:
		enum class TextAlignment
		{
			Left,
			Center,
			Right
		};
		enum class ParagraphAlignment
		{
			Left,
			Center,
			Right
		};
		struct TextProperties
		{
			TextAlignment textAlignment;
			ParagraphAlignment paragraphAlignment;
			std::wstring fontName;
			float fontSize;
			bool bold;
			bool italic;
		};

	ELEMENT_DECLARATION(Text2D)
	public:
		Text2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, std::wstring text, TextProperties textProperties, float width, float height);
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);
		void setText(std::wstring text);
		void addAlpha(float alpha);
		void setAlpha(float alpha);
	private:
		void convertTextProperties(TextProperties textProperties, DWRITE_FONT_WEIGHT& fontWeight, DWRITE_FONT_STYLE& fontStyle, DWRITE_TEXT_ALIGNMENT& textAlignment, DWRITE_PARAGRAPH_ALIGNMENT& paragraphAlignment);
	private:
		Microsoft::WRL::ComPtr<IDWriteFactory> mFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mFormat;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mBrush;
		D2D_RECT_F mShape;
		std::wstring mText;
	};
}