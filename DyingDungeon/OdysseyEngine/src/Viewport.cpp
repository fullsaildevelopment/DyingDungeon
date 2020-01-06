#include "Viewport.h"
#include "RenderWindow.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Viewport::Viewport(RenderDevice& renderDevice, RenderWindow* renderWindow)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mWidth = renderWindow->getWidth();
		mHeight = renderWindow->getHeight();
		renderWindow->getPosition(mTopLeftX, mTopLeftY);
		mMinDepth = 0.0f;
		mMaxDepth = 1.0f;

		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	Viewport::Viewport(RenderDevice& renderDevice, UINT width, UINT height, UINT topLeftX, UINT topLeftY, float minDepth, float maxDepth)
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