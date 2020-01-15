#include "Text2D.h"
#include "EventManager.h"

namespace Odyssey
{
	ELEMENT_DEFINITION( UIElement, Text2D ) 
	Text2D::Text2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, UINT width, UINT height, std::wstring text, TextProperties textProperties)
		: UIElement(position, color, width, height), mText(text), mProperties(textProperties)
	{
		// Create the DWrite factory
		HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mFactory), reinterpret_cast<IUnknown**>(mFactory.GetAddressOf()));
		assert(!FAILED(hr));

		// Create the text format resource
		createTextFormat(textProperties);

		// Subscribe to the element resize event
		EventManager::getInstance().subscribe(this, &Text2D::onTextResize);
	}

	void Text2D::onTextResize(UIElementResizeEvent* evnt)
	{
		mLock.lock(LockState::Write);
		mProperties.fontSize *= ((evnt->xScale + evnt->yScale) / 2.0f);
		mLock.unlock(LockState::Write);

		createTextFormat(mProperties);
	}

	void Text2D::draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
	{
		// Check if we have a valid brush
		mLock.lock(LockState::Write);
		if (mBrush == nullptr)
		{
			// Create the brush to render the text with
			context->CreateSolidColorBrush(D2D1::ColorF(mColor.x, mColor.y, mColor.z, 1.0f), mBrush.GetAddressOf());
			mBrush->SetOpacity(mColor.w);
		}

		// Render the text to the render target
		context->DrawTextW(mText.c_str(), static_cast<UINT32>(mText.length()), mFormat.Get(), mShape, mBrush.Get());
		mLock.unlock(LockState::Write);
	}

	void Text2D::setText(std::wstring text)
	{
		// Set the text to be rendered
		mLock.lock(LockState::Write);
		mText = text;
		mLock.unlock(LockState::Write);
	}

	void Text2D::setTextAlignment(TextAlignment alignment)
	{
		// Set the text alignment and re-create the text format
		mLock.lock(LockState::Write);
		mProperties.textAlignment = alignment;
		mLock.unlock(LockState::Write);
		createTextFormat(mProperties);
	}

	void Text2D::setParagraphAlignment(ParagraphAlignment alignment)
	{
		// Set the paragraph alignment and re-create the text format
		mLock.lock(LockState::Write);
		mProperties.paragraphAlignment = alignment;
		mLock.unlock(LockState::Write);
		createTextFormat(mProperties);
	}

	void Text2D::setFont(std::wstring fontName)
	{
		// Set the font name and re-create the text format
		mLock.lock(LockState::Write);
		mProperties.fontName = fontName;
		mLock.unlock(LockState::Write);
		createTextFormat(mProperties);
	}

	void Text2D::setFontSize(float fontSize)
	{
		// Set the font size and re-create the text format
		mLock.lock(LockState::Write);
		mProperties.fontSize = fontSize;
		mLock.unlock(LockState::Write);
		createTextFormat(mProperties);
	}

	void Text2D::setBoldStyle(bool bold)
	{
		// Set the bold style flag and re-create the text format
		mLock.lock(LockState::Write);
		mProperties.bold = bold;
		mLock.unlock(LockState::Write);
		createTextFormat(mProperties);
	}

	void Text2D::setItalicStyle(bool italic)
	{
		// Set the italic style flag and re-create the text format
		mLock.lock(LockState::Write);
		mProperties.italic = italic;
		mLock.unlock(LockState::Write);
		createTextFormat(mProperties);
	}

	void Text2D::createTextFormat(TextProperties textProperties)
	{
		DWRITE_FONT_WEIGHT fontWeight;
		DWRITE_FONT_STYLE fontStyle;
		DWRITE_TEXT_ALIGNMENT textAlignment;
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;

		// Convert the custom properties to D2D1 types
		convertTextProperties(textProperties, fontWeight, fontStyle, textAlignment, paragraphAlignment);

		// Create the text format
		mLock.lock(LockState::Write);
		HRESULT hr = mFactory->CreateTextFormat(textProperties.fontName.c_str(), NULL, fontWeight, fontStyle, DWRITE_FONT_STRETCH_NORMAL, textProperties.fontSize, L"", mFormat.GetAddressOf());

		// Set the text and paragraph alignment
		mFormat->SetTextAlignment(textAlignment);
		mFormat->SetParagraphAlignment(paragraphAlignment);
		mLock.unlock(LockState::Write);
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

		// Convert the font style
		if (textProperties.italic)
		{
			fontStyle = DWRITE_FONT_STYLE_ITALIC;
		}
		else
		{
			fontStyle = DWRITE_FONT_STYLE_NORMAL;
		}

		// Convert the text alignment
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

		// Convert the paragraph alignment
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