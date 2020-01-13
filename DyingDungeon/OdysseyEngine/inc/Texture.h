#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class RenderDevice;

	class Texture
	{
	public:

	public: // Constructor
		Texture(std::shared_ptr<RenderDevice> renderDevice);
		Texture(std::shared_ptr<RenderDevice> renderDevice, TextureType textureType, const char* filename);
		~Texture() = default;
	public:
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int slot);
		void loadRenderTargetTexture(int width, int height, DXGI_FORMAT format, UINT bindFlag);
		void createSRV(DXGI_FORMAT format);
		ID3D11Texture2D* getTextureResource();
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT slot);
	private:
		void stringToWChar(const char* cPtr, wchar_t*& wPtr);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture2D;
		Microsoft::WRL::ComPtr<ID3D11Texture3D> mTexture3D;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResource;
		TextureType mTextureType;
		TextureBindFlag mBindFlag;
	};
}