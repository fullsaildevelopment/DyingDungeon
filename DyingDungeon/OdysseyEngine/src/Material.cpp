#include "Material.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderManager.h"
#include "Texture.h"

namespace Odyssey
{
	Material::Material()
	{
		mMaterialBuffer = RenderManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(MaterialProperties)), &mProperties);

		setDefaultMaterialProperties();

		mPixelShader = -1;
		mBlendState = nullptr;
	}

	Material::Material(TextureType textureType, int textureID)
	{
		mMaterialBuffer = RenderManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(MaterialProperties)), &mProperties);

		setDefaultMaterialProperties();
		setTexture(textureType, textureID);
		mPixelShader = -1;
	}

	void Material::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mLock.lock(LockState::Write);

		if (mPixelShader != -1)
		{
			RenderManager::getInstance().getShader(mPixelShader)->bind(context);
		}

		if (mBlendState)
		{
			float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			context->OMSetBlendState(mBlendState.Get(), blendFactor, 0xFFFFFFFF);
		}

		// Bind the material properties constant buffer to the pixel shader slot 0
		Buffer* materialBuffer = RenderManager::getInstance().getBuffer(mMaterialBuffer);
		materialBuffer->updateData(context , &mProperties);
		materialBuffer->bind(context, 0, ShaderType::PixelShader);

		// Bind the diffuse texture
		if (mTextureMap.count(TextureType::Diffuse) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Diffuse])->bind(context);
		}
		// Bind the emissive texture
		if (mTextureMap.count(TextureType::Emissive) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Emissive])->bind(context);
		}

		// Bind the specular texture
		if (mTextureMap.count(TextureType::Specular) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Specular])->bind(context);
		}

		// Bind the Normal texture
		if (mTextureMap.count(TextureType::NormalMap) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::NormalMap])->bind(context);
		}

		if (mTextureMap.count(TextureType::Skybox) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Skybox])->bind(context);
		}
		mLock.unlock(LockState::Write);
	}

	void Material::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mLock.lock(LockState::Read);

		if (mBlendState)
		{
			float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);
		}

		// Bind the diffuse texture
		if (mTextureMap.count(TextureType::Diffuse) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Diffuse])->unbind(context);
		}
		// Bind the emissive texture
		if (mTextureMap.count(TextureType::Emissive) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Emissive])->unbind(context);
		}

		// Bind the specular texture
		if (mTextureMap.count(TextureType::Specular) != 0)
		{
			RenderManager::getInstance().getTexture(mTextureMap[TextureType::Specular])->unbind(context);
		}
		mLock.unlock(LockState::Read);
	}

	void Material::setTexture(TextureType textureType, int textureID)
	{
		mLock.lock(LockState::Write);
		switch (textureType)
		{
		case TextureType::Diffuse:
		{
			mProperties.mHasDiffuseTexture = 1;
			break;
		}
		case TextureType::Emissive:
		{
			mProperties.mHasEmissiveTexture = 1;
			break;
		}
		case TextureType::Specular:
		{
			mProperties.mHasSpecularTexture = 1;
			break;
		}
		case TextureType::NormalMap:
		{
			mProperties.mHasNormalTexture = 1;
			break;
		}
		}
		mTextureMap[textureType] = textureID;
		mLock.unlock(LockState::Write);
	}

	void Material::setGlobalAmbient(DirectX::XMFLOAT4 globalAmbient)
	{
		// Set the new global ambient value and update the constant buffer
		mLock.lock(LockState::Write);
		mProperties.mGlobalAmbient = globalAmbient;
		mLock.unlock(LockState::Write);
	}

	void Material::setSpecularPower(float specularPower)
	{
		// Set the new specular power value and update the constant buffer
		mLock.lock(LockState::Write);
		mProperties.mSpecularPower = specularPower;
		mLock.unlock(LockState::Write);
	}

	void Material::setReflectance(float reflectance)
	{
		mLock.lock(LockState::Write);
		mProperties.mReflectance = reflectance;
		mLock.unlock(LockState::Write);
	}

	void Material::setDiffuseColor(DirectX::XMFLOAT4 color)
	{
		mLock.lock(LockState::Write);
		mProperties.mDiffuseColor = color;
		mLock.unlock(LockState::Write);
	}

	void Material::setReceiveShadow(bool receiveShadow)
	{
		mLock.lock(LockState::Write);
		mProperties.mReceivesShadows = receiveShadow;
		mLock.unlock(LockState::Write);
	}

	void Material::setShader(const char* filename)
	{
		mLock.lock(LockState::Write);
		mPixelShader = RenderManager::getInstance().createShader(ShaderType::PixelShader, filename, nullptr, 0);
		mLock.unlock(LockState::Write);
	}

	void Material::setAlphaBlend(bool alphaBlend)
	{
		if (mBlendState == nullptr && alphaBlend)
		{
			// Create the blend state
			D3D11_RENDER_TARGET_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BlendEnable = true;
			desc.BlendEnable = true;
			desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.DestBlend = D3D11_BLEND_ONE;
			desc.BlendOp = D3D11_BLEND_OP_ADD;
			desc.SrcBlendAlpha = D3D11_BLEND_ZERO;
			desc.DestBlendAlpha = D3D11_BLEND_ONE;
			desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(blendDesc));
			blendDesc.AlphaToCoverageEnable = true;
			blendDesc.IndependentBlendEnable = false;
			blendDesc.RenderTarget[0] = desc;

			RenderManager::getInstance().getDX11Device()->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
		}
		else
		{
			mBlendState = nullptr;
		}
	}

	Shader* Material::getShader()
	{
		mLock.lock(LockState::Read);
		Shader* shader = RenderManager::getInstance().getShader(mPixelShader);
		mLock.unlock(LockState::Read);
		return shader;
	}

	void Material::setDefaultMaterialProperties()
	{
		mLock.lock(LockState::Write);
		mProperties.mGlobalAmbient = DirectX::XMFLOAT4(0.015f, 0.015f, 0.015f, 1.0f);
		mProperties.mHasDiffuseTexture = 0;
		mProperties.mDiffuseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		mProperties.mHasSpecularTexture = 0;
		mProperties.mSpecularColor = { 0.25f, 0.25f, 0.25f, 1.0f };
		mProperties.mHasEmissiveTexture = 0;
		mProperties.mEmissiveColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		mProperties.mHasNormalTexture = 0;
		mProperties.mSpecularPower = 256.0f;
		mProperties.mReflectance = 0.1f;
		mProperties.mReceivesShadows = 1;
		mLock.unlock(LockState::Write);
	}
}