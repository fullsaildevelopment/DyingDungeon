#include "ParticleSystem.h"
#include "Buffer.h"
#include "Material.h"
#include "RenderState.h"
#include "RenderDevice.h"
#include "Shader.h"

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

		mRenderState = renderDevice.createRenderState(Topology::PointList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);
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

	void ParticleSystem::update(double deltaTime)
	{
		//Run();
	}

	void ParticleSystem::Run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind and dispatch the compute shader
		mComputeShader->bind(context);
		mParticleBuffer->bind(context, 0, ShaderType::ComputeShader);
		mComputeShader->dispatch(context, 5000, 1, 1);
		mParticleBuffer->unbind(context, 0, ShaderType::ComputeShader);

		// Bind the geometry shader and particle buffer
		mGeometryShader->bind(context);
		mParticleBuffer->bind(context, 0, ShaderType::GeometryShader);

		// Bind the render state
		mRenderState->bind(context);

		// Bind the vertex shader and material properties
		mVertexShader->bind(context);
		mMaterial->bind(context);

		// Bind the blend state
		float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		context->OMSetBlendState(mBlendState.Get(), blendFactor, 0xffffffff);

		// Draw the particles
		context->Draw(mNumberOfParticles, 0);
		context->OMSetBlendState(0, 0, 0xffffffff);

		// Unbind the shaders and buffers
		mParticleBuffer->unbind(context, 0, ShaderType::ComputeShader);
		mComputeShader->unbind(context);
		mParticleBuffer->unbind(context, 0, ShaderType::GeometryShader);
		mGeometryShader->unbind(context);
		mMaterial->unbind(context);
	}
	void ParticleSystem::setParticleData(std::vector<Particle> particles)
	{
		mParticleData = particles;

		mParticleBuffer = mRenderDevice.createBuffer(BufferBindFlag::StructuredBuffer, size_t(mNumberOfParticles), 
			static_cast<UINT>(sizeof(Particle)), mParticleData.data());
	}
}