#pragma once
#include "EngineIncludes.h"
#include "Texture.h"
#include "Viewport.h"

namespace Odyssey
{
	// Forward declarations
	class RenderWindow;

	class RenderTarget
	{
	public: // Constructors
		RenderTarget();
		RenderTarget(int width, int height, bool depthEnabled);
		RenderTarget(int width, int height, bool depthEnabled, RenderWindow& renderWindow);
	public: // Creation Functions
		void createDepthTarget(UINT bindFlags, int width, int height);
		void setViewport(Viewport& viewport);
		void bind();
		void bindDepthTexture();
		void unBind();
		void unbindDepthTexture();
		Texture* getDepthTexture();
		void clearRenderView();
		void clearDepth();
		~RenderTarget() { }
	private:
		void createRenderTargetView(int width, int height);
		void createDepthStencilView(int width, int height);
	private: // DirectX Resources
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	private: // Rendering Resources
		std::unique_ptr<Texture> mRTVTexture;
		std::unique_ptr<Texture> mDSVTexture;
		std::unique_ptr<Viewport> mViewport;
	};
}