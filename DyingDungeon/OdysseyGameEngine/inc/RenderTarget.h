#pragma once
#include "EngineIncludes.h"
#include "Texture.h"
#include "Viewport.h"

namespace Odyssey
{
	// Forward declarations
	class RenderWindow;
	class RenderDevice;

	class RenderTarget
	{
	public: // Constructors
		RenderTarget(RenderDevice& renderDevice, int width, int height, bool depthEnabled);
		RenderTarget(RenderDevice& renderDevice, int width, int height, bool depthEnabled, RenderWindow* renderWindow);
	public: // Creation Functions
		void createDepthTarget(UINT bindFlags, int width, int height);
		void setViewport(std::shared_ptr<Viewport> viewport);
		void bind();
		void bindDepthTexture();
		void unBind();
		void unbindDepthTexture();
		Texture* getRenderTexture();
		Texture* getDepthTexture();
		void clearRenderView();
		void clearDepth();
		~RenderTarget() { }
	private:
		void createRenderTargetView(RenderDevice& renderDevice, int width, int height);
		void createDepthStencilView(RenderDevice& renderDevice, int width, int height);
	private: // DirectX Resources
		RenderDevice& mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	private: // Rendering Resources
		std::shared_ptr<Texture> mRTVTexture;
		std::shared_ptr<Texture> mDSVTexture;
		std::shared_ptr<Viewport> mViewport;
	};
}