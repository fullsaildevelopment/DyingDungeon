#pragma once
#include "EngineIncludes.h"
#include "EngineEvents.h"
#include "UIElement.h"

namespace Odyssey
{
	class Sprite2D : public UIElement
	{
		ELEMENT_DECLARATION(Sprite2D)

	public: // Rule of 3
		Sprite2D(DirectX::XMFLOAT2 position, std::wstring filename, UINT width = 0, UINT height = 0);

	public: // Interface
		virtual std::shared_ptr<UIElement> clone() const;
		/**
		 *	Render the 2D sprite to the parameter render target.
		 *	@param[in] renderTarget The 2D render target to render the sprite to.
		 *	@return void
		 */
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

		/**
		 *	Create the sprite image resource to be rendered.
		 *	@param[in] filename The filename of the sprite image.
		 *	@param[in] width The width of the image. Default 0 uses the file width.
		 *	@param[in] height The height of the image. Default 0 uses the file height.
		 *	@return void
		 */
		void setSprite(std::wstring filename, UINT width = 0, UINT height = 0);

		std::wstring getSpriteFilename();

	protected:
		virtual void createResource();
	private: // Helpers
		/**
		 *	Create the bitmap resource from the parameter file.
		 *	@param[in] filename The filename of the sprite image.
		 *	@param[in] width The width of the image. Default 0 uses the file width.
		 *	@param[in] height The height of the image. Default 0 uses the file height.
		 *	@return void
		 */
		void createBitmapFromFile(std::wstring filename, float width, float height);

	private: // Members
		Microsoft::WRL::ComPtr<ID2D1BitmapBrush1> mBitmapBrush;
		IWICImagingFactory* factory;
		Microsoft::WRL::ComPtr<ID2D1Bitmap> mBitmap;
		IWICFormatConverter* mBitmapConverter;
		std::wstring mFilename;
	};
}