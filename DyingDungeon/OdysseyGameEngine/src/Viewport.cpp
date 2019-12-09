#include "Viewport.h"
#include "RenderWindow.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Viewport::Viewport(RenderDevice& renderDevice, RenderWindow* renderWindow)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mWidth = renderWindow->mMainWindow.width;
		mHeight = renderWindow->mMainWindow.height;
		mTopLeftX = renderWindow->mMainWindow.left;
		mTopLeftY = renderWindow->mMainWindow.top;
		mMinDepth = 0.0f;
		mMaxDepth = 1.0f;

		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	Viewport::Viewport(RenderDevice& renderDevice, int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mWidth = width;
		mHeight = height;
		mTopLeftX = topLeftX;
		mTopLeftY = topLeftY;
		mMinDepth = minDepth;
		mMaxDepth = maxDepth;
		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	void Viewport::bind()
	{
		mDeviceContext->RSSetViewports(1, &mViewport);
	}

	void Viewport::unbind()
	{
		mDeviceContext->RSSetViewports(0, nullptr);
	}
}