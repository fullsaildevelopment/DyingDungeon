#pragma once
#include "EngineIncludes.h"
#include "Shader.h"

namespace Odyssey
{
	class ShaderManager
	{
	public:
		static ShaderManager& getInstance();
		~ShaderManager() { }
	private:
		ShaderManager() { }
	public:
		void setDefaultShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
		std::shared_ptr<Shader> createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
		Shader* getShader(int shaderID);
	private:
		LONG readShaderFile(const char* filename, char*& byteCode);
	private:
		typedef std::vector<std::shared_ptr<Shader>> ShaderList;
		ShaderList mShaderList;
		std::map<const char*, std::shared_ptr<Shader>> mShaderMap;
		int mDefaultPixelShader;
		int mDefaultVertexShader;
		int mSkinnedVertexShader;
		int mSkyboxVertexShader;
		int mSkyboxPixelShader;
	};
}