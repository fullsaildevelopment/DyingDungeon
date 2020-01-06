#include "Material.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderDevice.h"
#include "Texture.h"

namespace Odyssey
{
	Material::Material(RenderDevice& renderDevice)
	{
		mMaterialBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(MaterialProperties)), &mProperties);

		setDefaultMaterialProperties();
	}

	Material::Material(RenderDevice& renderDevice, TextureType textureType, std::shared_ptr<Texture> texture)
	{
		mMaterialBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(MaterialProperties)), &mProperties);

		setDefaultMaterialProperties();
		setTexture(textureType, texture);
	}

	void Material::bind()
	{
		if (mPixelShader)
			mPixelShader->bind();

		// Bind the material properties constant buffer to the pixel shader slot 0
		mMaterialBuffer->bind(0, ShaderType::PixelShader);

		// Bind the diffuse texture
		if (mTextureMap.count(TextureType::Diffuse) != 0)
		{
			mTextureMap[TextureType::Diffuse]->bind();
		}
		// Bind the emissive texture
		if (mTextureMap.count(TextureType::Emissive) != 0)
		{
			mTextureMap[TextureType::Emissive]->bind();
		}

		// Bind the specular texture
		if (mTextureMap.count(TextureType::Specular) != 0)
		{
			mTextureMap[TextureType::Specular]->bind();
		}

		// Bind the Normal texture
		if (mTextureMap.count(TextureType::NormalMap) != 0)
		{
			mTextureMap[TextureType::NormalMap]->bind();
		}

		if (mTextureMap.count(TextureType::Skybox) != 0)
		{
			mTextureMap[TextureType::Skybox]->bind();
		}
	}

	void Material::unbind()
	{
		// Bind the diffuse texture
		if (mTextureMap.count(TextureType::Diffuse) != 0)
		{
			mTextureMap[TextureType::Diffuse]->unbind();
		}
		// Bind the emissive texture
		if (mTextureMap.count(TextureType::Emissive) != 0)
		{
			mTextureMap[TextureType::Emissive]->unbind();
		}

		// Bind the specular texture
		if (mTextureMap.count(TextureType::Specular) != 0)
		{
			mTextureMap[TextureType::Specular]->unbind();
		}
	}

	void Material::setTexture(TextureType textureType, std::shared_ptr<Texture> texture)
	{
		switch (textureType)
		{
		case TextureType::Diffuse:
		{
			mProperties.mHasDiffuseTexture = 1;
			mMaterialBuffer->updateData(&mProperties);
			break;
		}
		case TextureType::Emissive:
		{
			mProperties.mHasEmissiveTexture = 1;
			mMaterialBuffer->updateData(&mProperties);
			break;
		}
		case TextureType::Specular:
		{
			mProperties.mHasSpecularTexture = 1;
			mMaterialBuffer->updateData(&mProperties);
			break;
		}
		case TextureType::NormalMap:
		{
			mProperties.mHasNormalTexture = 1;
			mMaterialBuffer->updateData(&mProperties);
			break;
		}
		}
		mTextureMap[textureType] = texture;
	}

	void Material::setGlobalAmbient(DirectX::XMFLOAT4 globalAmbient)
	{
		// Set the new global ambient value and update the constant buffer
		mProperties.mGlobalAmbient = globalAmbient;
		mMaterialBuffer->updateData(&mProperties);
	}

	void Material::setSpecularPower(float specularPower)
	{
		// Set the new specular power value and update the constant buffer
		mProperties.mSpecularPower = specularPower;
		mMaterialBuffer->updateData(&mProperties);
	}

	void Material::setReflectance(float reflectance)
	{
		mProperties.mReflectance = reflectance;
		mMaterialBuffer->updateData(&mProperties);
	}

	void Material::setDiffuseColor(DirectX::XMFLOAT4 color)
	{
		mProperties.mDiffuseColor = color;
		mMaterialBuffer->updateData(&mProperties);
	}

	void Material::setShader(std::shared_ptr<Shader> pixelShader)
	{
		mPixelShader = pixelShader;
	}

	Shader* Material::getShader()
	{
		return mPixelShader.get();
	}

	void Material::setDefaultMaterialProperties()
	{
		mProperties.mGlobalAmbient = DirectX::XMFLOAT4(0.035f, 0.035f, 0.035f, 1.0f);
		mProperties.mHasDiffuseTexture = 0;
		mProperties.mDiffuseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		mProperties.mHasSpecularTexture = 0;
		mProperties.mSpecularColor = { 0.25f, 0.25f, 0.25f, 1.0f };
		mProperties.mHasEmissiveTexture = 0;
		mProperties.mEmissiveColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		mProperties.mHasNormalTexture = 0;
		mProperties.mSpecularPower = 512.0f;
		mProperties.mReflectance = 0.01f;
		
		
	}
}