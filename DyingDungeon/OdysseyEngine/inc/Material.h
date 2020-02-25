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
		uint32_t mReceivesShadows;
		float pad;
	};  //------------------------------- ( 16 * 5 = 80 bytes )

	class Buffer;
	class RenderDevice;
	class Shader;
	class Texture;

	class Material
	{
	public:
		Material();
		Material(TextureType textureType, int textureID);
		~Material() = default;
	public:
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	public: // Mutators
		void setTexture(TextureType textureType, int textureID);
		void setGlobalAmbient(DirectX::XMFLOAT4 globalAmbient);
		void setSpecularPower(float specularPower);
		void setReflectance(float reflectance);
		void setDiffuseColor(DirectX::XMFLOAT4 color);
		void setReceiveShadow(bool receiveShadow);
		void setShader(const char* filename);
		void setAlphaBlend(bool alphaBlend);
		Shader* getShader();
	private:
		void setDefaultMaterialProperties();
		MaterialProperties mProperties;
	private:
		std::map<TextureType, int> mTextureMap;
		int mMaterialBuffer;
		int mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		ReadWriteLock mLock;
	};
}
