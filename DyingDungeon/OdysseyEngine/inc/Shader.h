#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class SamplerState;
	class RenderDevice;

	class Shader
	{
	public:
		Shader(std::shared_ptr<RenderDevice> renderDevice, ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void addSampler(std::shared_ptr<SamplerState> state);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void dispatch(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int x, int y, int z);
		~Shader() = default;
	private:
		LONG readShaderFile(const char* filename, char*& byteCode);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::vector<std::shared_ptr<SamplerState>> samplerList;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mComputeShader;
		ShaderType mShaderType;
	};
}
