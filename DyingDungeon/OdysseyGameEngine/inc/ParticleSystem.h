#pragma once
#include "EngineIncludes.h"
#include "RenderState.h"

namespace Odyssey
{
	struct Particle
	{
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 velocity;
		DirectX::XMFLOAT3 color;
		float lifeTime;
	};

	class Buffer;
	class Material;
	class Shader;
	class RenderState;

	class ParticleSystem
	{
	public:
		ParticleSystem();
		void Run();
		void setParticleData(std::vector<Particle> data);
	public:
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mGeometryShader;
		std::shared_ptr<Shader> mComputeShader;
		std::shared_ptr<Material> mMaterial;
		std::shared_ptr<Buffer> mParticleBuffer;
		std::unique_ptr<RenderState> mRenderState;
		int mNumberOfParticles;
		std::vector<Particle> mParticleData;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
	};
}