#include "Buffer.h"
#include "Shader.h"
#include "SamplerState.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Shader::Shader(RenderDevice& renderDevice, ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
		: mShaderType(shaderType)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);

		// Read the shader from the cso file
		char* byteCode = { 0 };
		size_t shaderSize = readShaderFile(filename, byteCode);

		switch (mShaderType)
		{
		case ShaderType::PixelShader:
		{
			// Create the pixel shader and assert it did not fail
			HRESULT hr = mDevice->CreatePixelShader(byteCode, shaderSize, nullptr, mPixelShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		case ShaderType::VertexShader:
		{
			// Create the vertex shader and assert it did not fail
			HRESULT hr = mDevice->CreateVertexShader(byteCode, shaderSize, nullptr, mVertexShader.GetAddressOf());
			assert(!FAILED(hr));

			// Assert that there is a layout and the number of elements is not 0
			if (layout && numberOfElements != 0)
			{
				// Create the input layout and assert it did not fail
				hr = mDevice->CreateInputLayout(layout, numberOfElements, byteCode, shaderSize, mInputLayout.GetAddressOf());
				assert(!FAILED(hr));
			}

			break;
		}
		case ShaderType::GeometryShader:
		{
			// Create the geometry shader and assert it did not fail
			HRESULT hr = mDevice->CreateGeometryShader(byteCode, shaderSize, nullptr, mGeometryShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		case ShaderType::ComputeShader:
		{
			// Create the compute shader and assert it did not fail
			HRESULT hr = mDevice->CreateComputeShader(byteCode, shaderSize, nullptr, mComputeShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		}

		// Delete the bytecode
		delete[] byteCode;
		byteCode = nullptr;
	}

	void Shader::bind()
	{
		if (mShaderType == ShaderType::PixelShader && mPixelShader.Get())
		{
			// Set the pixel shader
			mDeviceContext->PSSetShader(mPixelShader.Get(), 0, 0);

			// Iterate through each sampler and bind it
			for (std::shared_ptr<SamplerState> sampler : samplerList)
			{
				sampler->bind();
			}
		}
		else if (mShaderType == ShaderType::VertexShader && mVertexShader.Get())
		{
			// Set the vertex shader and input layout
			mDeviceContext->VSSetShader(mVertexShader.Get(), 0, 0);
			if (mInputLayout)
			{
				mDeviceContext->IASetInputLayout(mInputLayout.Get());
			}
		}
		else if (mShaderType == ShaderType::ComputeShader && mComputeShader.Get())
		{
			mDeviceContext->CSSetShader(mComputeShader.Get(), 0, 0);
		}
		else if (mShaderType == ShaderType::GeometryShader && mGeometryShader.Get())
		{
			mDeviceContext->GSSetShader(mGeometryShader.Get(), 0, 0);
		}
	}

	void Shader::addSampler(std::shared_ptr<SamplerState> sampler)
	{
		samplerList.push_back(sampler);
	}

	void Shader::unbind()
	{
		if (mShaderType == ShaderType::PixelShader && mPixelShader.Get())
		{
			mDeviceContext->PSSetShader(nullptr, 0, 0);

			// Iterate through each sampler and bind it
			for (std::shared_ptr<SamplerState> sampler : samplerList)
			{
				sampler->unbind();
			}
		}
		else if (mShaderType == ShaderType::VertexShader && mVertexShader.Get())
		{
			mDeviceContext->VSSetShader(nullptr, 0, 0);
		}
		else if (mShaderType == ShaderType::ComputeShader && mComputeShader.Get())
		{
			mDeviceContext->CSSetShader(nullptr, 0, 0);
		}
		else if (mShaderType == ShaderType::GeometryShader && mGeometryShader.Get())
		{
			mDeviceContext->GSSetShader(nullptr, 0, 0);
		}
	}

	void Shader::dispatch(int x, int y, int z)
	{
		if (mShaderType == ShaderType::ComputeShader)
		{
			mDeviceContext->Dispatch(x, y, z);
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