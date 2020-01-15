#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"
#include "ReadWriteLock.h"

namespace Odyssey
{
	struct MaterialProperties
	{
		DirectX::XMFLOAT4 mGlobalAmbient;
		//------------------------------- ( 16 bytes )
		DirectX::XMFLOAT4 mDiffuseColor;
		//------------------------------- ( 16 bytes )
		DirectX::XMFLOAT4 mSpecularColor;
		//------------------------------- ( 16 bytes )
		DirectX::XMFLOAT4 mEmissiveColor;
		//------------------------------- ( 16 bytes )
		float mSpecularPower;
		float mReflectance;
		uint32_t mHasDiffuseTexture;
		uint32_t mHasSpecularTexture;
		//------------------------------- ( 16 bytes )
		uint32_t mHasEmissiveTexture;
		uint32_t mHasNormalTexture;
		DirectX::XMFLOAT2 pad;
	};  //------------------------------- ( 16 * 5 = 80 bytes )

	class Buffer;
	class RenderDevice;
	class Shader;
	class Texture;

	class Material
	{
	public:
		Material(std::shared_ptr<RenderDevice> renderDevice);
		Material(std::shared_ptr<RenderDevice> renderDevice, TextureType textureType, std::shared_ptr<Texture> texture);
		~Material() = default;
	public:
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	public: // Mutators
		void setTexture(TextureType textureType, std::shared_ptr<Texture> texture);
		void setGlobalAmbient(DirectX::XMFLOAT4 globalAmbient);
		void setSpecularPower(float specularPower);
		void setReflectance(float reflectance);
		void setDiffuseColor(DirectX::XMFLOAT4 color);
		void setShader(std::shared_ptr<Shader> pixelShader);
		Shader* getShader();
	private:
		void setDefaultMaterialProperties();
		MaterialProperties mProperties;
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::map<TextureType, std::shared_ptr<Texture>> mTextureMap;
		std::shared_ptr<Buffer> mMaterialBuffer;
		std::shared_ptr<Shader> mPixelShader;
		ReadWriteLock mLock;
	};
}
