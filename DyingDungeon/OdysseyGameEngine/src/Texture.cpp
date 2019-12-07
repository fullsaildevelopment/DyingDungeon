#include "Texture.h"
#include "RenderManager.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

namespace Odyssey
{
	Texture::Texture()
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);
	}

	Texture::Texture(TextureType textureType, const char* filename)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);
		mTextureType = textureType;
		mBindFlag = TextureBindFlag::ShaderResource;

		wchar_t* file = nullptr;
		stringToWChar(filename, file);

		HRESULT hr;
		if (mTextureType == TextureType::Skybox)
		{
			hr = DirectX::CreateDDSTextureFromFile(mDevice.Get(), file, (ID3D11Resource**)mTexture3D.Get(), mShaderResource.GetAddressOf());
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), file, (ID3D11Resource**)mTexture2D.Get(), mShaderResource.GetAddressOf());
		}

		delete file;
		assert(!FAILED(hr));
	}

	void Texture::bind()
	{
		UINT slot = (UINT)mTextureType;
		mDeviceContext->PSSetShaderResources(slot, 1, mShaderResource.GetAddressOf());
	}

	void Texture::loadRenderTargetTexture(int width, int height, DXGI_FORMAT format, UINT bindFlag)
	{
		mTextureType = TextureType::Shadow;

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = format;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = bindFlag;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;

		HRESULT hr = mDevice->CreateTexture2D(&texDesc, nullptr, mTexture2D.GetAddressOf());
	}

	void Texture::createSRV(DXGI_FORMAT format)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Format = format;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		HRESULT hr = mDevice->CreateShaderResourceView(mTexture2D.Get(), &shaderResourceViewDesc, mShaderResource.GetAddressOf());
	}

	ID3D11Texture2D* Texture::getTextureResource()
	{
		return mTexture2D.Get();
	}

	void Texture::unbind()
	{
		UINT slot = (UINT)mTextureType;
		ID3D11ShaderResourceView* nSRV = nullptr;
		mDeviceContext->PSSetShaderResources(slot, 1, &nSRV);
	}

	void Texture::stringToWChar(const char* cPtr, wchar_t*& wPtr)
	{
		size_t cSize = strlen(cPtr) + 1;
		size_t convertedChars = 0;
		wPtr = new wchar_t[cSize];
		mbstowcs_s(&convertedChars, wPtr, cSize, cPtr, _TRUNCATE);
	}
}