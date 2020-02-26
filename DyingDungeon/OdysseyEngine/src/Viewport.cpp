#include "RenderWindow.h"
#include "RenderManager.h"
#include "Viewport.h"

namespace Odyssey
{
	Viewport::Viewport(RenderWindow* renderWindow)
	{
		// Set the viewport's width and height
		mWidth = renderWindow->getWidth();
		mHeight = renderWindow->getHeight();

		// Get the window's upper left position
		renderWindow->getPosition(mTopLeftX, mTopLeftY);

		// Default the min and max depth
		mMinDepth = 0.1f;
		mMaxDepth = 1.0f;

		// Set the viewport's properties to that of the window
		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	Viewport::Viewport(UINT width, UINT height, UINT topLeftX, UINT topLeftY, float minDepth, float maxDepth)
	{
		// Store the viewport's properties
		mWidth = width;
		mHeight = height;
		mTopLeftX = topLeftX;
		mTopLeftY = topLeftY;
		mMinDepth = minDepth;
		mMaxDepth = maxDepth;

		// Set the viewport's properties to the parameter values
		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	void Viewport::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->RSSetViewports(1, &mViewport);
	}

	void Viewport::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->RSSetViewports(0, nullptr);
	}
}