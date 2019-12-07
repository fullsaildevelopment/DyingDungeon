#include "RenderTarget.h"
#include "RenderManager.h"
#include "RenderWindow.h"

namespace Odyssey
{
	RenderTarget::RenderTarget()
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);
	}

	RenderTarget::RenderTarget(int width, int height, bool depthEnabled)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);

		createRenderTargetView(width, height);

		if (depthEnabled)
		{
			createDepthStencilView(width, height);
		}
	}

	RenderTarget::RenderTarget(int width, int height, bool depthEnabled, RenderWindow& renderWindow)
	{
		// Set up the device and device context
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);

		// Create the render target view
		HRESULT hr = mDevice->CreateRenderTargetView(renderWindow.getBackBuffer().Get(), nullptr, mRenderTargetView.GetAddressOf());

		// Set up a viewport for the render target
		mViewport = std::make_unique<Viewport>(renderWindow);

		// If a depth stencil is enabled, create it
		if (depthEnabled)
		{
			createDepthStencilView(width, height);
		}
	}

	void RenderTarget::bind()
	{
		if (mRenderTargetView)
		{
			if (mDepthStencilView)
			{
				mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
			}
			else
			{
				mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
			}
		}
		else if (mDepthStencilView)
		{
			mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		}

		if (mViewport)
		{
			mViewport->bind();
		}
	}

	void RenderTarget::bindDepthTexture()
	{
		mDSVTexture->bind();
	}

	void RenderTarget::unBind()
	{
		mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		if (mViewport)
		{
			mViewport->unbind();
		}
	}

	void RenderTarget::unbindDepthTexture()
	{
		mDSVTexture->unbind();
	}

	void RenderTarget::createDepthTarget(UINT bindFlags, int width, int height)
	{
		mDSVTexture = std::make_unique<Texture>();
		UINT flag = bindFlags;
		DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS;
		mDSVTexture->loadRenderTargetTexture(width, height, format, flag);

		ID3D11Texture2D* texture = mDSVTexture->getTextureResource();

		if (texture)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
			ZeroMemory(&zViewDesc, sizeof(zViewDesc));
			zViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			zViewDesc.Texture2D.MipSlice = 0;
			zViewDesc.Flags = 0;
			zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

			HRESULT hr = mDevice->CreateDepthStencilView(texture, &zViewDesc, mDepthStencilView.GetAddressOf());
		}
	}

	void RenderTarget::setViewport(Viewport& viewport)
	{
		mViewport = std::make_unique<Viewport>(viewport);
	}

	Texture* RenderTarget::getDepthTexture()
	{
		return mDSVTexture.get();
	}

	void RenderTarget::clearRenderView()
	{
		if (mRenderTargetView.Get())
		{
			mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), DirectX::Colors::Black);
		}
	}

	void RenderTarget::clearDepth()
	{
		if (mDepthStencilView.Get())
		{
			mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void RenderTarget::createRenderTargetView(int width, int height)
	{
		mRTVTexture = std::make_unique<Texture>();
		UINT flag = D3D11_BIND_RENDER_TARGET;
		DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		mRTVTexture->loadRenderTargetTexture(width, height, format, flag);

		ID3D11Texture2D* rtvTexture = mRTVTexture->getTextureResource();
		if (rtvTexture)
		{
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			ZeroMemory(&rtvDesc, sizeof(rtvDesc));
			rtvDesc.Format = format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			rtvDesc.Texture1D.MipSlice = 0;

			mDevice->CreateRenderTargetView(rtvTexture, &rtvDesc, mRenderTargetView.GetAddressOf());
		}
	}

	void RenderTarget::createDepthStencilView(int width, int height)
	{
		mDSVTexture = std::make_unique<Texture>();
		UINT flag = D3D11_BIND_DEPTH_STENCIL;
		DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;
		mDSVTexture->loadRenderTargetTexture(width, height, format, flag);

		ID3D11Texture2D* texture = mDSVTexture->getTextureResource();

		if (texture)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
			zViewDesc.Format = format;
			zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			zViewDesc.Texture2D.MipSlice = 0;

			HRESULT hr = mDevice->CreateDepthStencilView(texture, nullptr, mDepthStencilView.GetAddressOf());
		}
	}
}