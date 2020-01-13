#include "RenderDevice.h"
#include "RenderTarget.h"
#include "RenderWindowDX11.h"
#include "Texture.h"
#include "Viewport.h"

namespace Odyssey
{
	RenderTarget::RenderTarget(std::shared_ptr<RenderDevice> renderDevice, int width, int height, bool depthEnabled)
	{
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		createRenderTargetView(width, height);

		if (depthEnabled)
		{
			createDepthStencilView(width, height);
		}
	}

	RenderTarget::RenderTarget(std::shared_ptr<RenderDevice> renderDevice, int width, int height, bool depthEnabled, RenderWindow* renderWindow)
	{
		mRenderDevice = renderDevice;
		// Set up the device and device context
		mDevice = renderDevice->getDevice();

		// Create the render target view
		HRESULT hr = mDevice->CreateRenderTargetView(static_cast<RenderWindowDX11*>(renderWindow)->getBackBuffer().Get(), nullptr, mRenderTargetView.GetAddressOf());

		// Set up a viewport for the render target
		mViewport = renderDevice->createViewport(renderWindow);

		// If a depth stencil is enabled, create it
		if (depthEnabled)
		{
			createDepthStencilView(width, height);
		}
	}

	void RenderTarget::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		if (mRenderTargetView)
		{
			if (mDepthStencilView)
			{
				context->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
			}
			else
			{
				context->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
			}
		}
		else if (mDepthStencilView)
		{
			context->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		}

		if (mViewport)
		{
			mViewport->bind(context);
		}
	}

	void RenderTarget::bindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int slot)
	{
		mDSVTexture->bind(context, slot);
	}

	void RenderTarget::unBind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->OMSetRenderTargets(0, nullptr, nullptr);

		if (mViewport)
		{
			mViewport->unbind(context);
		}
	}

	void RenderTarget::unbindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mDSVTexture->unbind(context);
	}

	void RenderTarget::unbindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT slot)
	{
		mDSVTexture->unbind(context, slot);
	}

	Texture* RenderTarget::getRenderTexture()
	{
		return mRTVTexture.get();
	}

	void RenderTarget::createDepthTarget(UINT bindFlags, int width, int height)
	{
		mDSVTexture = mRenderDevice->createRenderTexture();
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

	void RenderTarget::setViewport(std::shared_ptr<Viewport> viewport)
	{
		mViewport = viewport;
	}

	Texture* RenderTarget::getDepthTexture()
	{
		return mDSVTexture.get();
	}

	void RenderTarget::clearRenderView(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		if (mRenderTargetView.Get())
		{
			float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			context->ClearRenderTargetView(mRenderTargetView.Get(), black);
		}
	}

	void RenderTarget::clearDepth(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		if (mDepthStencilView.Get())
		{
			context->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void RenderTarget::createRenderTargetView(int width, int height)
	{
		mRTVTexture = mRenderDevice->createRenderTexture();
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
		mDSVTexture = mRenderDevice->createRenderTexture();
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