#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"

namespace Odyssey
{
	// Forward declarations
	class RenderWindow;
	class RenderDevice;
	class Texture;
	class Viewport;

	class RenderTarget
	{
	public: // Constructors
		RenderTarget(int width, int height, bool depthEnabled);
		RenderTarget(RenderWindow* renderWindow, bool depthEnabled);
		~RenderTarget() = default;
	public: // Creation Functions
		void createDepthTarget(UINT bindFlags, int width, int height);
		void setViewport(int viewport);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void bindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int slot);
		void unBind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbindDepthTexture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT slot);
		Texture* getRenderTexture();
		Texture* getDepthTexture();
		void clearRenderView(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void clearDepth(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	private:
		void createRenderTargetView(int width, int height);
		void createDepthStencilView(int width, int height);
	private: // DirectX Resources
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	private: // Rendering Resources
		int mRenderTexture;
		int mDepthTexture;
		int mViewport;
	};
}