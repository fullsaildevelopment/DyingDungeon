#include "RenderManager.h"
#include "RenderWindowDX11.h"
#include "Texture.h"
#include "Viewport.h"
#include "RenderTarget.h"

namespace Odyssey
{
	RenderTarget::RenderTarget(int width, int height, bool depthEnabled)
	{
		createRenderTargetView(width, height);

		if (depthEnabled)
		{
			createDepthStencilView(width, height);
		}

		mViewport = -1;
	}

	RenderTarget::RenderTarget(RenderWindow* renderWindow, bool depthEnabled)
	{
		// Create the render target view from the render window backbuffer
		HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateRenderTargetView(static_cast<RenderWindowDX11*>(renderWindow)->getBackBuffer().Get(), nullptr, mRenderTargetView.GetAddressOf());

		// Set up a viewport for the render target
		mViewport = RenderManager::getInstance().createViewport(renderWindow);

		// If a depth stencil is enabled, create it
		if (depthEnabled)
		{
			createDepthStencilView(renderWindow->getWidth(), renderWindow->getHeight());
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

		if (mViewport != -1)
		{
			RenderManager::getInstance().getViewport(mViewport)->bind(context);
		}
	}

	void RenderTarget::bindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int slot)
	{
		RenderManager::getInstance().getTexture(mDepthTexture)->bind(context, slot);
	}

	void RenderTarget::unBind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->OMSetRenderTargets(0, nullptr, nullptr);

		if (mViewport)
		{
			RenderManager::getInstance().getViewport(mViewport)->unbind(context);
		}
	}

	void RenderTarget::unbindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		RenderManager::getInstance().getTexture(mDepthTexture)->unbind(context);
	}

	void RenderTarget::unbindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT slot)
	{
		RenderManager::getInstance().getTexture(mDepthTexture)->unbind(context, slot);
	}

	Texture* RenderTarget::getRenderTexture()
	{
		return RenderManager::getInstance().getTexture(mRenderTexture);
	}

	void RenderTarget::createDepthTarget(UINT bindFlags, int width, int height)
	{
		UINT flag = bindFlags;
		DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS;
		mDepthTexture = RenderManager::getInstance().createTexture(TextureType::Render, width, height, format, flag);

		ID3D11Texture2D* texture = RenderManager::getInstance().getTexture(mDepthTexture)->getTextureResource();

		if (texture)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
			ZeroMemory(&zViewDesc, sizeof(zViewDesc));
			zViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			zViewDesc.Texture2D.MipSlice = 0;
			zViewDesc.Flags = 0;
			zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

			HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateDepthStencilView(texture, &zViewDesc, mDepthStencilView.GetAddressOf());
		}
	}

	void RenderTarget::setViewport(int viewport)
	{
		mViewport = viewport;
	}

	Texture* RenderTarget::getDepthTexture()
	{
		return RenderManager::getInstance().getTexture(mDepthTexture);
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
		UINT flag = D3D11_BIND_RENDER_TARGET;
		DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		mRenderTexture = RenderManager::getInstance().createTexture(TextureType::Render, width, height, format, flag);

		ID3D11Texture2D* rtvTexture = RenderManager::getInstance().getTexture(mRenderTexture)->getTextureResource();

		if (rtvTexture)
		{
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			ZeroMemory(&rtvDesc, sizeof(rtvDesc));
			rtvDesc.Format = format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			rtvDesc.Texture1D.MipSlice = 0;

			RenderManager::getInstance().getDX11Device()->CreateRenderTargetView(rtvTexture, &rtvDesc, mRenderTargetView.GetAddressOf());
		}
	}

	void RenderTarget::createDepthStencilView(int width, int height)
	{
		UINT flag = D3D11_BIND_DEPTH_STENCIL;
		DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;
		mDepthTexture = RenderManager::getInstance().createTexture(TextureType::Render, width, height, format, flag);

		ID3D11Texture2D* texture = RenderManager::getInstance().getTexture(mDepthTexture)->getTextureResource();

		if (texture)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
			zViewDesc.Format = format;
			if (width == 4096)
				zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			else
				zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

			zViewDesc.Texture2D.MipSlice = 0;

			HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateDepthStencilView(texture, nullptr, mDepthStencilView.GetAddressOf());
		}
	}
}