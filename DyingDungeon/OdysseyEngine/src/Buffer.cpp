#include "Buffer.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Buffer::Buffer(std::shared_ptr<RenderDevice> renderDevice, BufferBindFlag bindFlag, size_t count, UINT stride, const void* data)
		: mBuffer(nullptr), mBindFlag(bindFlag), mCount(count), mStride(stride), mOffset(0)
	{
		// Get the device and device context
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		// Create the buffer description with the parameter bind flag and calculated byte width
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = getD3D11BindFlag(bindFlag);
		bufferDesc.ByteWidth = mStride * static_cast<UINT>(mCount);
		bufferDesc.CPUAccessFlags = 0;

		// If we are creating a structured buffer set the misc flag 
		if (bindFlag == BufferBindFlag::StructuredBuffer)
		{
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bufferDesc.StructureByteStride = stride;
		}
		else
		{
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
		}

		// Create the sub resource data from the parameter data
		if (data)
		{
			D3D11_SUBRESOURCE_DATA resourceData;
			ZeroMemory(&resourceData, sizeof(resourceData));
			resourceData.pSysMem = data;
			resourceData.SysMemPitch = 0;
			resourceData.SysMemSlicePitch = 0;

			// Create the buffer and verify the success
			HRESULT hr = mDevice->CreateBuffer(&bufferDesc, &resourceData, &mBuffer);
			assert(!FAILED(hr));
		}
		else
		{
			// Create the buffer and verify the success
			HRESULT hr = mDevice->CreateBuffer(&bufferDesc, nullptr, &mBuffer);
			assert(!FAILED(hr));
		}

		// If the buffer is a structured buffer create the UAV and SRV to go with it
		if (bindFlag == BufferBindFlag::StructuredBuffer)
		{
			// Create the UAV to match this particle system buffer
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			ZeroMemory(&uavDesc, sizeof(uavDesc));
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
			HRESULT hr = mDevice->CreateUnorderedAccessView(mBuffer.Get(), &uavDesc, mUAV.GetAddressOf());

			// Create the SRV to match this particle system buffer
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			mBuffer.Get()->GetDesc(&bufferDesc);
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			srvDesc.BufferEx.FirstElement = 0;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.BufferEx.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
			hr = mDevice->CreateShaderResourceView(mBuffer.Get(), &srvDesc, mSRV.GetAddressOf());
		}
	}

	void Buffer::updateData(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, const void* data)
	{
		// Update the buffer using the parameter data
		context->UpdateSubresource(mBuffer.Get(), 0, nullptr, data, 0, 0);
	}

	void Buffer::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mLock.lock(LockState::Read);
		// Bind either a vertex or index buffer based on the bind flag of the buffer
		switch (mBindFlag)
		{
		case BufferBindFlag::VertexBuffer:
		{
			context->IASetVertexBuffers(0, 1, mBuffer.GetAddressOf(), &mStride, &mOffset);
			break;
		}
		case BufferBindFlag::IndexBuffer:
		{
			context->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, mOffset);
			break;
		}
		}
		mLock.unlock(LockState::Read);
	}

	void Buffer::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT shaderSlot, ShaderType shaderType)
	{
		mLock.lock(LockState::Read);
		// Bind the constant buffer to the shader type
		if (mBindFlag == BufferBindFlag::ConstantBuffer)
		{
			switch (shaderType)
			{
			case ShaderType::PixelShader:
			{
				context->PSSetConstantBuffers(shaderSlot, 1, mBuffer.GetAddressOf());
				break;
			}
			case ShaderType::VertexShader:
			{
				context->VSSetConstantBuffers(shaderSlot, 1, mBuffer.GetAddressOf());
				break;
			}
			case ShaderType::GeometryShader:
			{
				context->GSSetConstantBuffers(shaderSlot, 1, mBuffer.GetAddressOf());
				break;
			}
			case ShaderType::ComputeShader:
			{
				context->CSSetConstantBuffers(shaderSlot, 1, mBuffer.GetAddressOf());
			}
			}
		}
		// Bind the structured buffer to the shader type
		else if (mBindFlag == BufferBindFlag::StructuredBuffer)
		{
			switch (shaderType)
			{
			case ShaderType::GeometryShader:
			{
				context->GSSetShaderResources(shaderSlot, 1, mSRV.GetAddressOf());
				break;
			}
			case ShaderType::ComputeShader:
			{
				context->CSSetUnorderedAccessViews(shaderSlot, 1, mUAV.GetAddressOf(), nullptr);
				break;
			}
			}
		}
		mLock.unlock(LockState::Read);
	}

	void Buffer::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT shaderSlot, ShaderType shaderType)
	{
		if (mBindFlag == BufferBindFlag::VertexBuffer)
		{
			// Bind a null vertex buffer
			ID3D11Buffer* nBuffs[] = { nullptr };
			context->IASetVertexBuffers(shaderSlot, 1, nBuffs, &mStride, &mOffset);
		}
		else if (mBindFlag == BufferBindFlag::IndexBuffer)
		{
			context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
		}
		// Unbind the constant buffer from the rendering pipeline
		else if (mBindFlag == BufferBindFlag::ConstantBuffer)
		{
			switch (shaderType)
			{
			case ShaderType::PixelShader:
			{
				ID3D11Buffer* nBuffer = nullptr;
				context->PSSetConstantBuffers(shaderSlot, 1, &nBuffer);
				break;
			}
			case ShaderType::VertexShader:
			{
				ID3D11Buffer* nBuffer = nullptr;
				context->VSSetConstantBuffers(shaderSlot, 1, &nBuffer);
				break;
			}
			case ShaderType::GeometryShader:
			{
				ID3D11Buffer* nBuffer = nullptr;
				context->GSSetConstantBuffers(shaderSlot, 1, &nBuffer);
			}
			}
		}
		// Unbind the structured buffer from the rendering pipeline
		else if (mBindFlag == BufferBindFlag::StructuredBuffer)
		{
			switch (shaderType)
			{
			case ShaderType::GeometryShader:
			{
				ID3D11ShaderResourceView* nSRV = nullptr;
				context->GSSetShaderResources(shaderSlot, 1, &nSRV);
				break;
			}
			case ShaderType::ComputeShader:
			{
				ID3D11UnorderedAccessView* nUAV = nullptr;
				context->CSSetUnorderedAccessViews(shaderSlot, 1, &nUAV, nullptr);
				break;
			}
			}
		}
	}

	UINT Buffer::getD3D11BindFlag(BufferBindFlag bindFlag)
	{
		UINT dxBindFlag = 0;

		// Converts the engine bind flag to the corresponding d3d11 bind flag
		switch (bindFlag)
		{
		case BufferBindFlag::ConstantBuffer:
		{
			dxBindFlag = D3D11_BIND_CONSTANT_BUFFER;
			break;
		}
		case BufferBindFlag::IndexBuffer:
		{
			dxBindFlag = D3D11_BIND_INDEX_BUFFER;
			break;
		}
		case BufferBindFlag::VertexBuffer:
		{
			dxBindFlag = D3D11_BIND_VERTEX_BUFFER;
			break;
		}
		case BufferBindFlag::StructuredBuffer:
		{
			dxBindFlag = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		}
		return dxBindFlag;
	}
}