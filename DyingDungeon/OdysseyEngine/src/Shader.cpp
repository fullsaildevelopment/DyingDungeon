#include "Buffer.h"
#include "Shader.h"
#include "SamplerState.h"
#include "RenderManager.h"

namespace Odyssey
{
	Shader::Shader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
	{
		mShaderType = shaderType;
		// Read the shader from the cso file
		char* byteCode = { 0 };
		size_t shaderSize = readShaderFile(filename, byteCode);

		switch (mShaderType)
		{
		case ShaderType::PixelShader:
		{
			// Create the pixel shader and assert it did not fail
			HRESULT hr = RenderManager::getInstance().getDX11Device()->CreatePixelShader(byteCode, shaderSize, nullptr, mPixelShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		case ShaderType::VertexShader:
		{
			// Create the vertex shader and assert it did not fail
			HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateVertexShader(byteCode, shaderSize, nullptr, mVertexShader.GetAddressOf());
			assert(!FAILED(hr));

			// Assert that there is a layout and the number of elements is not 0
			if (layout && numberOfElements != 0)
			{
				// Create the input layout and assert it did not fail
				hr = RenderManager::getInstance().getDX11Device()->CreateInputLayout(layout, numberOfElements, byteCode, shaderSize, mInputLayout.GetAddressOf());
				assert(!FAILED(hr));
			}

			break;
		}
		case ShaderType::GeometryShader:
		{
			// Create the geometry shader and assert it did not fail
			HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateGeometryShader(byteCode, shaderSize, nullptr, mGeometryShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		case ShaderType::ComputeShader:
		{
			// Create the compute shader and assert it did not fail
			HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateComputeShader(byteCode, shaderSize, nullptr, mComputeShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		}

		// Delete the bytecode
		delete[] byteCode;
		byteCode = nullptr;
	}

	void Shader::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		if (mShaderType == ShaderType::PixelShader && mPixelShader.Get())
		{
			// Set the pixel shader
			context->PSSetShader(mPixelShader.Get(), 0, 0);

			// Iterate through each sampler and bind it
			for (int sampler : samplerList)
			{
				RenderManager::getInstance().getSamplerState(sampler)->bind(context);
			}
		}
		else if (mShaderType == ShaderType::VertexShader && mVertexShader.Get())
		{
			// Set the vertex shader and input layout
			context->VSSetShader(mVertexShader.Get(), 0, 0);
			if (mInputLayout)
			{
				context->IASetInputLayout(mInputLayout.Get());
			}
		}
		else if (mShaderType == ShaderType::ComputeShader && mComputeShader.Get())
		{
			context->CSSetShader(mComputeShader.Get(), 0, 0);
		}
		else if (mShaderType == ShaderType::GeometryShader && mGeometryShader.Get())
		{
			context->GSSetShader(mGeometryShader.Get(), 0, 0);
		}
	}

	void Shader::addSampler(int samplerState)
	{
		samplerList.push_back(samplerState);
	}

	void Shader::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		if (mShaderType == ShaderType::PixelShader && mPixelShader.Get())
		{
			context->PSSetShader(nullptr, 0, 0);

			// Iterate through each sampler and bind it
			for (int sampler : samplerList)
			{
				RenderManager::getInstance().getSamplerState(sampler)->unbind(context);
			}
		}
		else if (mShaderType == ShaderType::VertexShader && mVertexShader.Get())
		{
			context->VSSetShader(nullptr, 0, 0);
		}
		else if (mShaderType == ShaderType::ComputeShader && mComputeShader.Get())
		{
			context->CSSetShader(nullptr, 0, 0);
		}
		else if (mShaderType == ShaderType::GeometryShader && mGeometryShader.Get())
		{
			context->GSSetShader(nullptr, 0, 0);
		}
	}

	void Shader::dispatch(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int x, int y, int z)
	{
		if (mShaderType == ShaderType::ComputeShader)
		{
			context->Dispatch(x, y, z);
		}
	}

	LONG Shader::readShaderFile(const char* filename, char*& byteCode)
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