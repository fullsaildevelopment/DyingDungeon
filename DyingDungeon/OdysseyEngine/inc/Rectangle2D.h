#pragma once
#include "EngineIncludes.h"
#include "EngineEvents.h"
#include "UIElement.h"

namespace Odyssey
{
	class Rectangle2D : public UIElement
	{
	ELEMENT_DECLARATION(Rectangle2D)

	public: // Rule of 3
		Rectangle2D(DirectX::XMFLOAT2 centerPosition, DirectX::XMFLOAT4 color, UINT width, UINT height);

	public: // Interface
		/**
		 *	Render the 2D rectangle to the parameter render target.
		 *	@param[in] renderTarget The 2D render target to render the rectangle to.
		 *	@return void
		 */
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

		/**
		 *	Add to the rectangle's fill.
		 *	@param[in] fill The value to add to the fill.
		 *	@return void
		 */
		void addFill(float fill);

		/**
		 *	Set the rectangle's fill.
		 *	@param[in] fill The value of the fill.
		 *	@return void
		 */
		void setFill(float fill);

		/**
		 *	Lerp the rectangle's color based on the fill amount. The color will be lerped from the zero fill color to the rectangle's base color.
		 *	@param[in] zeroFillColor The color of the rectangle at zero fill. (0 - 255 color channels)
		 *	@return void
		 */
		void enableColorLerp(DirectX::XMFLOAT3 zeroFillColor);

		/**
		 *	Set the zero fill color's opacity.
		 *	@param[in] opacity The zero fill color's opacity.
		 *	@return void
		 */
		void setZeroFillOpacity(float opacity);

		/**
		 *	Disable the rectangle's color lerp.
		 *	@param[in] void
		 *	@return void
		 */
		void disableColorLerp();

	private: // Helpers
		/**
		 *	Lerp the color channels between the zero fill and max fill colors.
		 *	@param[in] ratio The ratio used to lerp between the two colors. Typically the rectangle's fill.
		 *	@return XMFloat4 The resulting color of the lerp.
		 */
		DirectX::XMFLOAT4 colorLerp(DirectX::XMFLOAT4 zeroFillColor, DirectX::XMFLOAT4 maxFillColor, float ratio);

	private: // Members
		DirectX::XMFLOAT4 mZeroFillColor;
		float mFill;
		bool mColorLerpEnabled;
	};
}