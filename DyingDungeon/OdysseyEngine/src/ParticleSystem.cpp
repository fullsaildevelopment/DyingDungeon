#include "ParticleSystem.h"
#include "Buffer.h"
#include "Material.h"
#include "RenderState.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, ParticleSystem)

		ParticleSystem::ParticleSystem(RenderDevice& renderDevice)
		: mRenderDevice(renderDevice)
	{
		mDevice = renderDevice.getDevice();

		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/ParticleVertexShader.cso", nullptr, 0);
		mGeometryShader = renderDevice.createShader(ShaderType::GeometryShader, "../OdysseyEngine/shaders/ParticleGeometryShader.cso", nullptr);
		mComputeShader = renderDevice.createShader(ShaderType::ComputeShader, "../OdysseyEngine/shaders/ParticleComputeShader.cso", nullptr);
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/ParticlePixelShader.cso", nullptr);
		mFire = renderDevice.createTexture(TextureType::Diffuse, "Flare.png");

		mRenderState = renderDevice.createRenderState(Topology::PointList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, false, false);

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthDisabled);

		mNumberOfParticles = 0;
		setParticleData(mParticleData);

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BlendEnable = true;
		desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.DestBlend = D3D11_BLEND_ONE;
		desc.BlendOp = D3D11_BLEND_OP_ADD;
		desc.SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.DestBlendAlpha = D3D11_BLEND_ONE;
		desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = desc;

		mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
	}

	void ParticleSystem::run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mLock.lock(LockState::Read);
		// Unbind any vertex buffers from the pipeline
		ID3D11Buffer* nBuffs[] = { nullptr };
		UINT stride = 0;
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, nBuffs, &stride, &offset);

		// Bind the depth state
		context->OMSetDepthStencilState(mDepthDisabled.Get(), 0);

		// Update the particle buffer
		mParticleBuffer->updateData(context, mParticleData.data());

		// Bind the geometry shader
		mGeometryShader->bind(context);

		// Bind the particle buffer
		mParticleBuffer->bind(context, 0, ShaderType::GeometryShader);

		// Bind the render state
		mRenderState->bind(context);

		// Bind the vertex shader and material properties
		mVertexShader->bind(context);
		mPixelShader->bind(context);

		// Bind the blend state
		float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->OMSetBlendState(mBlendState.Get(), blendFactor, 0xffffffff);

		mFire->bind(context);

		// Draw the particles
		context->Draw(mNumberOfParticles, 0);
		context->OMSetBlendState(0, 0, 0xffffffff);

		// Unbind the shaders and buffers
		mParticleBuffer->unbind(context, 0, ShaderType::GeometryShader);
		mGeometryShader->unbind(context);
		mPixelShader->unbind(context);
		mVertexShader->unbind(context);

		context->OMSetDepthStencilState(nullptr, 0);
		mLock.unlock(LockState::Read);
	}

	void ParticleSystem::updateParticleValues(double deltaTime)
	{
		mParticleValues.deltaTime = deltaTime;
		mParticleValues.startSpeed = 1.0f;
	}

	void ParticleSystem::createParticle(Particle& particle)
	{
		std::mt19937 generator(rnd());
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> yVel(0.25f, 0.95f);
		std::uniform_real_distribution<float> lf(0.3f, 0.6f);

		particle.origin = DirectX::XMFLOAT3(dist(generator), dist(generator), dist(generator));
		particle.position = particle.origin;
		particle.velocity = DirectX::XMFLOAT3(0.0f, yVel(generator), 0.0f);
		particle.active = true;
		particle.lifeTime = lf(generator);
	}

	void ParticleSystem::setParticleData(std::vector<Particle> particles)
	{
		mParticleData.resize(100);

		mParticleBuffer = mRenderDevice.createBuffer(BufferBindFlag::StructuredBuffer, size_t(100),
			static_cast<UINT>(sizeof(Particle)), mParticleData.data());

		mParticleValueBuffer = mRenderDevice.createBuffer(BufferBindFlag::ConstantBuffer, sizeof(ParticleValueBuffer),
			1, &mParticleValues);
	}

	void ParticleSystem::update(double deltaTime)
	{
		int emissionRate = 1;

		mLock.lock(LockState::Write);
		for (int i = 0; i < mParticleData.size(); i++)
		{
			mParticleData[i].lifeTime -= deltaTime;

			if (mParticleData[i].lifeTime <= 0.0f && mParticleData[i].active)
			{
				mParticleData[i].active = false;
				mNumberOfParticles--;
			}
			else if (mParticleData[i].active)
			{
				mParticleData[i].position.x += mParticleData[i].velocity.x * deltaTime;
				mParticleData[i].position.y += mParticleData[i].velocity.y * deltaTime;
				mParticleData[i].position.z += mParticleData[i].velocity.z * deltaTime;
			}

			if (mParticleData[i].active == false && emissionRate != 0)
			{
				createParticle(mParticleData[i]);
				mNumberOfParticles++;
				emissionRate--;
			}
		}
		mLock.unlock(LockState::Write);
	}
}