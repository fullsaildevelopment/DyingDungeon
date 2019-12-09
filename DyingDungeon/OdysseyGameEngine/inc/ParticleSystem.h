#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "Buffer.h"
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

	class Material;
	class Shader;
	class RenderDevice;

	class ParticleSystem : public Component
	{
		CLASS_DECLARATION(ParticleSystem)
	public: // Constructors
		ParticleSystem(RenderDevice& renderDevice);
	public: // Component Functions
		virtual void initialize(GameObject* parent);
		virtual void update(double deltaTime);
	public: // Public Interface
		void setParticleData(std::vector<Particle> data);
	private: // Helper Functions
		void Run();
	public:
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mGeometryShader;
		std::shared_ptr<Shader> mComputeShader;
		std::shared_ptr<Material> mMaterial;
		std::shared_ptr<Buffer> mParticleBuffer;
		std::shared_ptr<RenderState> mRenderState;
		int mNumberOfParticles;
		std::vector<Particle> mParticleData;
	private:
		RenderDevice& mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
	};
}