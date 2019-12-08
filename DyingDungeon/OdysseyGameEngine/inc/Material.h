#pragma once
#include "EngineIncludes.h"

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

	struct MatFileData
	{
		std::vector<const char*> texFilenames;
		std::vector<DirectX::XMFLOAT3> texColors;
		std::vector<float> texFactors;
	};

	class Buffer;
	class Shader;

	class Material
	{
	public:
		Material();
		Material(TextureType textureType, int textureID);
		~Material() = default;
	public:
		void bind();
		void unbind();
	public: // Mutators
		void setTexture(TextureType textureType, int textureID);
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
		std::map<TextureType, int> mTextureMap;
		std::shared_ptr<Buffer> mMaterialBuffer;
		std::shared_ptr<Shader> mPixelShader;
	};
}
