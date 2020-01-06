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
		Texture(RenderDevice& renderDevice);
		Texture(RenderDevice& renderDevice, TextureType textureType, const char* filename);
		~Texture() = default;
	public:
		void bind();
		void bind(int slot);
		void loadRenderTargetTexture(int width, int height, DXGI_FORMAT format, UINT bindFlag);
		void createSRV(DXGI_FORMAT format);
		ID3D11Texture2D* getTextureResource();
		void unbind();
		void unbind(UINT slot);
	private:
		void stringToWChar(const char* cPtr, wchar_t*& wPtr);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture2D;
		Microsoft::WRL::ComPtr<ID3D11Texture3D> mTexture3D;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResource;
		TextureType mTextureType;
		TextureBindFlag mBindFlag;
	};
}