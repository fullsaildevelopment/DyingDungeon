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
		std::shared_ptr<Shader> createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
	private:
		LONG readShaderFile(const char* filename, char*& byteCode);
	private:
		typedef std::vector<std::shared_ptr<Shader>> ShaderList;
		ShaderList mShaderList;
		std::map<const char*, std::shared_ptr<Shader>> mShaderMap;
	};
}