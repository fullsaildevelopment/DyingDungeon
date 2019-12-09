#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderDevice;

	class Buffer
	{
	public:
		// Default constructor for the Buffer class
		Buffer(RenderDevice& renderDevice, BufferBindFlag bindFlag, size_t count, UINT stride, const void* data);
		// Update the buffer's data to a new value
		void updateData(const void* data);
		// Bind the buffer to the rendering pipeline
		void bind();
		// Bind the buffer to a specific slot and shader
		void bind(UINT shaderSlot, ShaderType shaderType);
		// Unbinds the buffer from the rendering pipeline
		void unbind(UINT shaderSlot, ShaderType shaderType);
		~Buffer() = default;
	private:
		// Converts an engine-specific bind flag to the corresponding DirectX 11 bind flag
		UINT getD3D11BindFlag(BufferBindFlag bindFlag);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		BufferBindFlag mBindFlag;
		UINT mStride;
		size_t mCount;
		UINT mOffset;
	};
}