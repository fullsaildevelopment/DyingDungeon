#include "Viewport.h"
#include "RenderWindow.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Viewport::Viewport(std::shared_ptr<RenderDevice> renderDevice, RenderWindow* renderWindow)
	{
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

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

	Viewport::Viewport(std::shared_ptr<RenderDevice> renderDevice, UINT width, UINT height, UINT topLeftX, UINT topLeftY, float minDepth, float maxDepth)
	{
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

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

	void Viewport::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->RSSetViewports(1, &mViewport);
	}

	void Viewport::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->RSSetViewports(0, nullptr);
	}
}