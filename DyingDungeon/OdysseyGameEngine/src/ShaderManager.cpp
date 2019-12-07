#include "ShaderManager.h"
#include <d3dcompiler.h>

namespace Odyssey
{
	ShaderManager& ShaderManager::getInstance()
	{
		static ShaderManager instance;
		return instance;
	}

	void ShaderManager::setDefaultShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
	{
		// Read the shader from the cso file
		char* byteCode = { 0 };
		size_t shaderSize = readShaderFile(filename, byteCode);

		// Create the shader
		std::unique_ptr<Shader> shader = std::make_unique<Shader>(shaderType, byteCode, shaderSize, layout, numberOfElements);

		// Delete the bytecode used to make the shader
		delete[] byteCode;
		byteCode = nullptr;

		// Store the shader in the list
		mShaderList.push_back(std::move(shader));

		// Store the shader as the default of it's type
		switch (shaderType)
		{
		case ShaderType::PixelShader:
		{
			mDefaultPixelShader = static_cast<int>(mShaderList.size()) - 1;
			break;
		}
		case ShaderType::VertexShader:
		{
			mDefaultVertexShader = static_cast<int>(mShaderList.size()) - 1;
			break;
		}
		case ShaderType::GeometryShader:
		{
			break;
		}
		case ShaderType::ComputeShader:
		{
			break;
		}
		}
	}

	std::shared_ptr<Shader> ShaderManager::createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
	{
		if (mShaderMap.count(filename) != 0)
		{
			return mShaderMap[filename];
		}

		// Read the shader from the cso file
		char* byteCode = { 0 };
		size_t shaderSize = readShaderFile(filename, byteCode);

		// Create the shader
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderType, byteCode, shaderSize, layout, numberOfElements);

		// Delete the bytecode
		delete[] byteCode;
		byteCode = nullptr;

		// Add the shader to the list
		mShaderList.push_back(shader);

		// Add the shader to the map
		mShaderMap[filename] = shader;

		// Return the shader
		return shader;
	}

	Shader* ShaderManager::getShader(int index)
	{
		return mShaderList[index].get();
	}

	LONG ShaderManager::readShaderFile(const char* filename, char*& byteCode)
	{
		FILE* file;
		fopen_s(&file, filename, "rb");
		if (file)
		{
			fseek(file, 0, SEEK_END);
			long length = ftell(file);
			byteCode = (char*)(malloc(length));
			fseek(file, 0, SEEK_SET);
			fread(byteCode, 1, length, file);
			fclose(file);
			return length;
		}

		return 0;
	}
}