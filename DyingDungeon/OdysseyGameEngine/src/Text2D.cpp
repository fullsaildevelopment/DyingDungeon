#include "Text2D.h"

namespace Odyssey
{
	ELEMENT_DEFINITION( UIElement, Text2D ) 
	Text2D::Text2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, std::wstring text, TextProperties textProperties, float width, float height)
	{
		// Create the DWrite factorry
		HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mFactory), reinterpret_cast<IUnknown**>(mFactory.GetAddressOf()));
		assert(!FAILED(hr));

		// Convert the custom properties to D2D1 types
		DWRITE_FONT_WEIGHT fontWeight;
		DWRITE_FONT_STYLE fontStyle;
		DWRITE_TEXT_ALIGNMENT textAlignment;
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;

		convertTextProperties(textProperties, fontWeight, fontStyle, textAlignment, paragraphAlignment);

		// Create the text format
		hr = mFactory->CreateTextFormat(textProperties.fontName.c_str(), NULL, fontWeight, fontStyle, DWRITE_FONT_STRETCH_NORMAL, textProperties.fontSize, L"", mFormat.GetAddressOf());

		// Center-align the format
		mFormat->SetTextAlignment(textAlignment);
		mFormat->SetParagraphAlignment(paragraphAlignment);

		// Create the shape to render the text onto
		mShape = D2D1::RectF(position.x, position.y, position.x + width, position.y + height);

		// Store the text to render
		mText = text;
		mColor = color;
	}

	void Text2D::draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget)
	{
		if (mBrush == nullptr)
		{
			// Create the brush to render the text with
			renderTarget->CreateSolidColorBrush(D2D1::ColorF(mColor.x, mColor.y, mColor.z, mColor.w), mBrush.GetAddressOf());
		}

		renderTarget->DrawTextW(mText.c_str(), static_cast<UINT32>(mText.length()), mFormat.Get(), mShape, mBrush.Get());
	}

	void Text2D::setText(std::wstring text)
	{
		mText = text;
	}

	void Text2D::addAlpha(float alpha)
	{
		mColor.w += alpha;
		mBrush = nullptr;

	}

	void Text2D::setAlpha(float alpha)
	{
		mColor.w = alpha;
		mBrush = nullptr;
	}
	void Text2D::convertTextProperties(TextProperties textProperties, DWRITE_FONT_WEIGHT& fontWeight, DWRITE_FONT_STYLE& fontStyle, DWRITE_TEXT_ALIGNMENT& textAlignment, DWRITE_PARAGRAPH_ALIGNMENT& paragraphAlignment)
	{
		// Convert the font weight
		if (textProperties.bold)
		{
			fontWeight = DWRITE_FONT_WEIGHT_BOLD;
		}
		else
		{
			fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
		}

		// Conver the font style
		if (textProperties.italic)
		{
			fontStyle = DWRITE_FONT_STYLE_ITALIC;
		}
		else
		{
			fontStyle = DWRITE_FONT_STYLE_NORMAL;
		}

		switch (textProperties.textAlignment)
		{
			case TextAlignment::Left:
			{
				textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
				break;
			}
			case TextAlignment::Center:
			{
				textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				break;
			}
			case TextAlignment::Right:
			{
				textAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
				break;
			}
		}

		switch (textProperties.paragraphAlignment)
		{
			case ParagraphAlignment::Left:
			{
				paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
				break;
			}
			case ParagraphAlignment::Center:
			{
				paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				break;
			}
			case ParagraphAlignment::Right:
			{
				paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
				break;
			}
		}
	}
}