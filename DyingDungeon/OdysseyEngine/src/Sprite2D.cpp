#include "Sprite2D.h"
#include "EventManager.h"

namespace Odyssey
{
	ELEMENT_DEFINITION(UIElement, Sprite2D)

		Sprite2D::Sprite2D(DirectX::XMFLOAT2 position, std::wstring filename, UINT width, UINT height)
		: UIElement(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, height)
	{
		// Create the WIC factory
		HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
		assert(!FAILED(hr));

		mFilename = filename;
		mBitmapBrush = nullptr;
		createResource();
	}

	void Sprite2D::draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
	{
		mLock.lock(LockState::Write);

		if (mBitmap == nullptr && mBitmapConverter)
		{
			HRESULT hr = context->CreateBitmapFromWicBitmap(mBitmapConverter, nullptr, mBitmap.GetAddressOf());
			mBitmapConverter = nullptr;
		}

		if (mBitmapBrush == nullptr)
		{
			// Create the bitmap brush
			HRESULT hr = context->CreateBitmapBrush(mBitmap.Get(), mBitmapBrush.GetAddressOf());
			assert(!FAILED(hr));

			// Offset the brush to be in the upper left of the shape
			D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Identity();
			translation.dx = mShape.left;
			translation.dy = mShape.top;

			// Set the brush's transform
			mBitmapBrush->SetTransform(translation);
		}

		// Calculate a new rect shape based on the fill
		D2D_RECT_F fillShape = mShape;
		fillShape.right -= (mDimensions.x * (1.0f - mFill));

		// Fill the rectangle
		if (context)
		{
			context->FillRectangle(fillShape, mBitmapBrush.Get());
		}

		mLock.unlock(LockState::Write);
	}

	void Sprite2D::createResource()
	{
		mLock.lock(LockState::Write);
		mBitmap.Reset();
		mBitmapConverter = nullptr;
		mBitmapBrush = nullptr;
		createBitmapFromFile(mFilename, mDimensions.x * mScale.x, mDimensions.y * mScale.y);
		mLock.unlock(LockState::Write);

	}
	void Sprite2D::setSprite(std::wstring filename, UINT width, UINT height)
	{
		// TODO: Find a better way to swap sprites. Probably create a 2nd instance of a bitmap and only swap when ready.
		mLock.lock(LockState::Write);
		mFilename = filename;
		mBitmap.Reset();
		mBitmapConverter = nullptr;
		mBitmapBrush = nullptr;
		createBitmapFromFile(filename, width, height);
		mLock.unlock(LockState::Write);
	}

	std::wstring Sprite2D::getSpriteFilename()
	{
		mLock.lock(LockState::Read);
		std::wstring temp = mFilename;
		mLock.unlock(LockState::Read);
		return temp;
	}

	void Sprite2D::createBitmapFromFile(std::wstring filename, float width, float height)
	{
		// Create the bimap decoder
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
		HRESULT hr = factory->CreateDecoderFromFilename(filename.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
		assert(!FAILED(hr));

		// Create the bitmap frame
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
		hr = decoder->GetFrame(0, frame.GetAddressOf());
		assert(!FAILED(hr));

		// Create the bitmap converter
		hr = factory->CreateFormatConverter(&mBitmapConverter);
		assert(!FAILED(hr));

		// Get the original size of the image
		UINT originalWidth, originalHeight;
		hr = frame->GetSize(&originalWidth, &originalHeight);
		mDimensions.x = static_cast<float>(originalWidth);
		mDimensions.y = static_cast<float>(originalHeight);

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

			mDimensions.x = static_cast<float>(width);
			mDimensions.y = static_cast<float>(height);

			// Create a bitmap scaler
			Microsoft::WRL::ComPtr<IWICBitmapScaler> bitmapScaler;
			hr = factory->CreateBitmapScaler(&bitmapScaler);
			assert(!FAILED(hr));

			// Initialize the bitmap scaler with the image, width and height
			hr = bitmapScaler->Initialize(frame.Get(), static_cast<UINT>(mDimensions.x), static_cast<UINT>(mDimensions.y), WICBitmapInterpolationModeCubic);
			assert(!FAILED(hr));

			// Initialize the converter with the scaler
			hr = mBitmapConverter->Initialize(bitmapScaler.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
			assert(!FAILED(hr));
		}
		else
		{
			hr = mBitmapConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
		}
	}
}