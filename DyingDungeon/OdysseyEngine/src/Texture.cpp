#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "RenderManager.h"

namespace Odyssey
{
	Texture::Texture(TextureType textureType, const char* filename)
	{
		// Set the texture type and default to a shader resource bind flag
		mTextureType = textureType;
		mBindFlag = TextureBindFlag::ShaderResource;

		// Append the filename to the end of the textures path
		std::string fname = filename;
		fname = "assets\\textures\\" + fname;

		// Convert it from a string to a wstring
		wchar_t* file = nullptr;
		stringToWChar(fname.c_str(),file);

		// Get the DX11 device
		Microsoft::WRL::ComPtr<ID3D11Device> device = RenderManager::getInstance().getDX11Device();

		// Store the resource of the texture
		Microsoft::WRL::ComPtr<ID3D11Resource> res;

		// Load from DDS if we are importing a skybox, otherwise use WIC
		if (mTextureType == TextureType::Skybox)
		{
			// Create the texture from the DDS file
			HRESULT hr = DirectX::CreateDDSTextureFromFile(device.Get(), file, res.GetAddressOf(), mShaderResource.GetAddressOf());

			// Assert the file imported correctly
			assert(!FAILED(hr));
		}
		else
		{
			// Create the texture from the WIC file
			HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), file, res.GetAddressOf(), mShaderResource.GetAddressOf());
		}

		// Query the resource variable for the texture behind it
		HRESULT hr = res->QueryInterface(IID_ID3D11Texture2D, (void**)mTexture2D.GetAddressOf());

		// Assert we got the texture correctly
		assert(!FAILED(hr));

		// Delete the dynamic memory
		delete file;
	}

	Texture::Texture(TextureType textureType, int width, int height, DXGI_FORMAT format, UINT bindFlag)
	{
		// Store the texture type
		mTextureType = textureType;

		// Create an empty Texture2D description
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));

		// Set the custom dimensions
		texDesc.Width = width;
		texDesc.Height = height;
		// Set the custom format and bind flags
		texDesc.Format = format;
		texDesc.BindFlags = bindFlag;
		// Set default values for the rest
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 4;
		if (textureType == TextureType::Render && width == 4096)
			texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;

		// Create the texture
		HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateTexture2D(&texDesc, nullptr, mTexture2D.GetAddressOf());

		// Assert the texture was created successfully
		assert(!FAILED(hr));
	}

	void Texture::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Convert the enum texture type to a UINT for a shader slot to bind
		UINT slot = (UINT)mTextureType;

		// Bind the texture's SRV
		context->PSSetShaderResources(slot, 1, mShaderResource.GetAddressOf());
	}

	void Texture::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int slot)
	{
		// Bind the texture's SRV to the parameter shader slot
		context->PSSetShaderResources(slot, 1, mShaderResource.GetAddressOf());
	}

	void Texture::createSRV(DXGI_FORMAT format)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		if (DXGI_FORMAT_R24_UNORM_X8_TYPELESS == format)
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		else
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		shaderResourceViewDesc.Format = format;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		mShaderResource = nullptr;
		HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateShaderResourceView(mTexture2D.Get(), &shaderResourceViewDesc, mShaderResource.GetAddressOf());
	}

	ID3D11Texture2D* Texture::getTextureResource()
	{
		return mTexture2D.Get();
	}

	void Texture::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		UINT slot = (UINT)mTextureType;
		ID3D11ShaderResourceView* nSRV = nullptr;
		context->PSSetShaderResources(slot, 1, &nSRV);
	}

	void Texture::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT slot)
	{
		ID3D11ShaderResourceView* nSRV = nullptr;
		context->PSSetShaderResources(slot, 1, &nSRV);
	}

	void Texture::stringToWChar(const char* cPtr, wchar_t*& wPtr)
	{
		size_t cSize = strlen(cPtr) + 1;
		size_t convertedChars = 0;
		wPtr = new wchar_t[cSize];
		mbstowcs_s(&convertedChars, wPtr, cSize, cPtr, _TRUNCATE);
	}
}