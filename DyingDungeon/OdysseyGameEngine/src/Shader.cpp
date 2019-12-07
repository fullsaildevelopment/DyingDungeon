#include "BufferManager.h"
#include "RenderManager.h"
#include "Buffer.h"
#include "Shader.h"
#include "SamplerState.h"

namespace Odyssey
{
	Shader::Shader(ShaderType shaderType, void* shaderCode, size_t shaderSize, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements) : mShaderType(shaderType)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(&mDeviceContext);

		switch (mShaderType)
		{
		case ShaderType::PixelShader:
		{
			// Create the pixel shader and assert it did not fail
			HRESULT hr = mDevice->CreatePixelShader(shaderCode, shaderSize, nullptr, mPixelShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		case ShaderType::VertexShader:
		{
			// Create the vertex shader and assert it did not fail
			HRESULT hr = mDevice->CreateVertexShader(shaderCode, shaderSize, nullptr, mVertexShader.GetAddressOf());
			assert(!FAILED(hr));

			// Assert that there is a layout and the number of elements is not 0
			if (layout && numberOfElements != 0)
			{
				// Create the input layout and assert it did not fail
				hr = mDevice->CreateInputLayout(layout, numberOfElements, shaderCode, shaderSize, mInputLayout.GetAddressOf());
				assert(!FAILED(hr));
			}

			break;
		}
		case ShaderType::GeometryShader:
		{
			// Create the geometry shader and assert it did not fail
			HRESULT hr = mDevice->CreateGeometryShader(shaderCode, shaderSize, nullptr, mGeometryShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		case ShaderType::ComputeShader:
		{
			// Create the compute shader and assert it did not fail
			HRESULT hr = mDevice->CreateComputeShader(shaderCode, shaderSize, nullptr, mComputeShader.GetAddressOf());
			assert(!FAILED(hr));
			break;
		}
		}
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

	void Shader::addSampler(SamplerState& state)
	{
		std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerState>(state);
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
}