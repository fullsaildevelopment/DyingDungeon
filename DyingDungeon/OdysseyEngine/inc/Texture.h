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
		Texture() = default;
		Texture(TextureType textureType, const char* filename);
		Texture(TextureType textureType, int width, int height, DXGI_FORMAT format, UINT bindFlag);
		~Texture() = default;
	public:
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int slot);
		void createSRV(DXGI_FORMAT format);
		ID3D11Texture2D* getTextureResource();
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT slot);
	private:
		void stringToWChar(const char* cPtr, wchar_t*& wPtr);
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture2D;
		Microsoft::WRL::ComPtr<ID3D11Texture3D> mTexture3D;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResource;
		TextureType mTextureType;
		TextureBindFlag mBindFlag;
	};
}