#include "Viewport.h"
#include "RenderWindow.h"
#include "RenderManager.h"

namespace Odyssey
{
	Viewport::Viewport(const Viewport& other)
	{
		mDevice = other.mDevice;
		mDeviceContext = other.mDeviceContext;
		mViewport = other.mViewport;
		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mTopLeftX = other.mTopLeftX;
		mTopLeftY = other.mTopLeftY;
		mMinDepth = other.mMinDepth;
		mMaxDepth = other.mMaxDepth;
	}
	Viewport::Viewport(RenderWindow& renderWindow)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mWidth = renderWindow.mMainWindow.width;
		mHeight = renderWindow.mMainWindow.height;
		mTopLeftX = renderWindow.mMainWindow.left;
		mTopLeftY = renderWindow.mMainWindow.top;
		mMinDepth = 0.0f;
		mMaxDepth = 1.0f;

		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	Viewport::Viewport(int width, int height)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mWidth = width;
		mHeight = height;
		mTopLeftX = 0;
		mTopLeftY = 0;
		mMinDepth = 0.0f;
		mMaxDepth = 1.0f;
		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	Viewport::Viewport(int width, int height, int topLeftX, int topLeftY)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mWidth = width;
		mHeight = height;
		mTopLeftX = topLeftX;
		mTopLeftY = topLeftY;
		mMinDepth = 0.0f;
		mMaxDepth = 1.0f;
		mViewport.Height = static_cast<float>(mHeight);
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.TopLeftX = static_cast<float>(mTopLeftX);
		mViewport.TopLeftY = static_cast<float>(mTopLeftY);
		mViewport.MinDepth = mMinDepth;
		mViewport.MaxDepth = mMaxDepth;
	}

	Viewport::Viewport(int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth)
	{
		mDevice = RenderManager::getInstance().getDevice();
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