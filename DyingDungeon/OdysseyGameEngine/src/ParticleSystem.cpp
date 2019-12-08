#include "ParticleSystem.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "Buffer.h"
#include "Material.h"
#include "RenderState.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, ParticleSystem)

	ParticleSystem::ParticleSystem()
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mVertexShader = ShaderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/ParticleVertexShader.cso", nullptr, 0);
		mGeometryShader = ShaderManager::getInstance().createShader(ShaderType::GeometryShader, "../OdysseyGameEngine/shaders/ParticleGeometryShader.cso", nullptr);
		mComputeShader = ShaderManager::getInstance().createShader(ShaderType::ComputeShader, "../OdysseyGameEngine/shaders/ParticleComputeShader.cso", nullptr);

		mRenderState = std::make_unique<RenderState>(Topology::PointList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);
		mNumberOfParticles = 0;

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BlendEnable = true;
		desc.SrcBlend = D3D11_BLEND_ONE;
		desc.DestBlend = D3D11_BLEND_ONE;
		desc.BlendOp = D3D11_BLEND_OP_ADD;
		desc.SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTargetWriteMask = 0x0f;

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = desc;

		mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
	}

	void ParticleSystem::initialize(GameObject* parent)
	{
		mGameObject = parent;
		onEnable();
	}

	void ParticleSystem::update(double deltaTime)
	{
		Run();
	}

	void ParticleSystem::Run()
	{
		// Bind and dispatch the compute shader
		mComputeShader->bind();
		mParticleBuffer->bind(0, ShaderType::ComputeShader);
		mComputeShader->dispatch(5000, 1, 1);
		mParticleBuffer->unbind(0, ShaderType::ComputeShader);

		// Bind the geometry shader and particle buffer
		mGeometryShader->bind();
		mParticleBuffer->bind(0, ShaderType::GeometryShader);

		// Bind the render state
		mRenderState->bind();

		// Bind the vertex shader and material properties
		mVertexShader->bind();
		mMaterial->bind();

		// Bind the blend state
		float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		mDeviceContext->OMSetBlendState(mBlendState.Get(), blendFactor, 0xffffffff);

		// Draw the particles
		mDeviceContext->Draw(mNumberOfParticles, 0);
		mDeviceContext->OMSetBlendState(0, 0, 0xffffffff);

		// Unbind the shaders and buffers
		mParticleBuffer->unbind(0, ShaderType::ComputeShader);
		mComputeShader->unbind();
		mParticleBuffer->unbind(0, ShaderType::GeometryShader);
		mGeometryShader->unbind();
		mMaterial->unbind();
	}
	void ParticleSystem::setParticleData(std::vector<Particle> particles)
	{
		mParticleData = particles;

		mParticleBuffer = std::make_unique<Buffer>(BufferBindFlag::StructuredBuffer, size_t(mNumberOfParticles), 
			static_cast<UINT>(sizeof(Particle)), mParticleData.data());
	}
}