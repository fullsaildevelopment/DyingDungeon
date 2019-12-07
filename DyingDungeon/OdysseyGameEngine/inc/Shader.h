#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class SamplerState;
	class Shader
	{
	public:
		Shader(ShaderType shaderType, void* shaderCode, size_t shaderSize, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
		void bind();
		void addSampler(SamplerState& state);
		void unbind();
		void dispatch(int x, int y, int z);
		~Shader() = default;
	private:
		std::vector<std::shared_ptr<SamplerState>> samplerList;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mComputeShader;
		ShaderType mShaderType;
	};
}
