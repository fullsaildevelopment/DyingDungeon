#include "Text2D.h"

namespace Odyssey
{
	ELEMENT_DEFINITION( UIElement, Text2D ) 
	Text2D::Text2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, std::wstring text, float fontSize, float width, float height)
	{
		// Create the DWrite factorry
		HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mFactory), reinterpret_cast<IUnknown**>(mFactory.GetAddressOf()));
		assert(!FAILED(hr));

		// Create the text format
		hr = mFactory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", mFormat.GetAddressOf());

		// Center-align the format
		mFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		mFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

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
}