#include "Sprite2D.h"

namespace Odyssey
{
	ELEMENT_DEFINITION( UIElement, Sprite2D ) 

	Sprite2D::Sprite2D(DirectX::XMFLOAT2 position, LPCWSTR filename, UINT width, UINT height)
	{
		// Create the WIC factory
		Microsoft::WRL::ComPtr <IWICImagingFactory> factory;
		HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(factory.GetAddressOf()));
		assert(!FAILED(hr));

		// Create the bimap decoder
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
		hr = factory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
		assert(!FAILED(hr));

		// Create the bitmap frame
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
		hr = decoder->GetFrame(0, frame.GetAddressOf());
		assert(!FAILED(hr));

		// Create the bitmap converter
		hr = factory->CreateFormatConverter(mBitmapConverter.GetAddressOf());
		assert(!FAILED(hr));

		// Get the original size of the image
		UINT originalWidth, originalHeight;
		hr = frame->GetSize(&originalWidth, &originalHeight);
		mWidth = originalWidth;
		mHeight = originalHeight;

		// Check if the user entered a custom width or height
		if (width != 0.0f || height != 0.0f)
		{
			assert(!FAILED(hr));

			// A custom height was input
			if (width == 0.0f)
			{
				// Get ratio and calculate a new width
				float scalar = static_cast<float>(height) / static_cast<float>(originalHeight);
				width = static_cast<UINT>(scalar * static_cast<float>(originalWidth));
			}
			// A custom width was input
			else if (height == 0.0f)
			{
				// Get the ratio of the new width to the original width
				float scalar = static_cast<float>(width) / static_cast<float>(originalWidth);
				// Calculate a new height based on the ratio
				height = static_cast<UINT>(scalar * static_cast<float>(originalHeight));
			}

			mWidth = width;
			mHeight = height;
			// Create a bitmap scaler
			Microsoft::WRL::ComPtr<IWICBitmapScaler> bitmapScaler;
			hr = factory->CreateBitmapScaler(&bitmapScaler);
			assert(!FAILED(hr));

			// Initialize the bitmap scaler with the image, width and height
			hr = bitmapScaler->Initialize(frame.Get(), mWidth, mHeight, WICBitmapInterpolationModeCubic);
			assert(!FAILED(hr));

			// Initialize the converter with the scaler
			hr = mBitmapConverter->Initialize(bitmapScaler.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
			assert(!FAILED(hr));
		}
		else
		{
			hr = mBitmapConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
		}

		mPosition = position;
	}

	void Sprite2D::draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget)
	{
		if (mBitmap == nullptr)
			HRESULT hr = renderTarget->CreateBitmapFromWicBitmap(mBitmapConverter.Get(), nullptr, mBitmap.GetAddressOf());
		else
		{
			renderTarget->DrawBitmap(mBitmap.Get(), D2D1::RectF(mPosition.x, mPosition.y, mPosition.x + mWidth, mPosition.y + mHeight));
		}
	}
}