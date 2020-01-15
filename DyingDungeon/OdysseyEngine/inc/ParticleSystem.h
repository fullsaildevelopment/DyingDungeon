#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "Buffer.h"
#include "RenderState.h"
#include <random>
#include "ReadWriteLock.h"

namespace Odyssey
{
	// Forward declaration
	class Material;
	class Shader;
	class Texture;
	class RenderDevice;

	struct ParticleValueBuffer
	{
		DirectX::XMFLOAT4 startVelocity;
		DirectX::XMFLOAT4 startPosition;
		DirectX::XMFLOAT4 startColor;
		float deltaTime;
		float startLifetime;
		float startSpeed;
		float duration;
	};

	class ParticleSystem : public Component
	{
		CLASS_DECLARATION(ParticleSystem)

	public: // Rule of 3
		ParticleSystem(RenderDevice& renderDevice);

	public: // Interface
		void setParticleData(std::vector<Particle> data);
		virtual void update(double deltaTime);

	public: // Helpers
		void run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	private:
		void updateParticleValues(double deltaTime);
		void createParticle(Particle& particle);
	public: // Members
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mGeometryShader;
		std::shared_ptr<Shader> mComputeShader;
		std::shared_ptr<Shader> mPixelShader;
		std::shared_ptr<RenderState> mRenderState;
		int mNumberOfParticles;
		std::vector<Particle> mParticleData;
		std::shared_ptr<Buffer> mParticleBuffer;
		std::shared_ptr<Texture> mFire;
	private:
		int mNextParticle;
		RenderDevice& mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthDisabled;
		ParticleValueBuffer mParticleValues;
		std::shared_ptr<Buffer> mParticleValueBuffer;
		std::random_device rnd;
		ReadWriteLock mLock;
	};
}