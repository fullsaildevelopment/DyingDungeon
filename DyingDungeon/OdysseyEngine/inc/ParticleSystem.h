#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "Buffer.h"
#include "RenderState.h"

namespace Odyssey
{
	// Forward declaration
	class Material;
	class Shader;
	class RenderDevice;

	class ParticleSystem : public Component
	{
		CLASS_DECLARATION(ParticleSystem)

	public: // Rule of 3
		ParticleSystem(RenderDevice& renderDevice);

	public: // Interface
		virtual void update(double deltaTime);
		void setParticleData(std::vector<Particle> data);

	private: // Helpers
		void Run();

	public: // Members
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