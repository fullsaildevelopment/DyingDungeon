#include "SkyboxPass.h"
#include "RenderState.h"
#include "SamplerState.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"

namespace Odyssey
{
	SkyboxPass::SkyboxPass(RenderDevice& renderDevice, const char* skyboxTexture, std::shared_ptr<RenderTarget> renderTarget)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		std::shared_ptr<Texture> texture = renderDevice.createTexture(TextureType::Skybox, skyboxTexture);
		std::shared_ptr<Mesh> mesh = renderDevice.createCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		std::shared_ptr<Material> material = renderDevice.createMaterial(TextureType::Skybox, texture);

		mSkyBox = std::make_shared<GameObject>();
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
		mSkyBox->addComponent<MeshRenderer>(mesh, material);

		mRenderTarget = renderTarget;
		mRenderState = renderDevice.createRenderState(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, true, true, false);

		D3D11_INPUT_ELEMENT_DESC vShaderLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/SkyboxVertexShader.cso", vShaderLayout, 7);
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyGameEngine/shaders/SkyboxPixelShader.cso", nullptr);


		std::shared_ptr<SamplerState> linear = renderDevice.createSamplerState(ComparisonFunc::COMPARISON_NEVER, D3D11_FILTER_ANISOTROPIC, 0);
		mPixelShader->addSampler(linear);
	}

	void SkyboxPass::preRender(RenderArgs& args)
	{
		args.shaderMatrix.view = args.camera->getInverseViewMatrix();
		args.shaderMatrix.proj = args.camera->getProjectionMatrix();
		mRenderTarget->bind();
		mRenderState->bind();
		mVertexShader->bind();
		mPixelShader->bind();
	}

	void SkyboxPass::render(RenderArgs& args)
	{
		// Get the camera's position
		DirectX::XMFLOAT3 camPos = args.camera->getPosition();

		// Set the skybox to the camera's position
		mSkyBox->addComponent<Transform>();
		mSkyBox->getComponent<Transform>()->setPosition(camPos.x, camPos.y, camPos.z);

		// Get the global transform of the skybox and set the mvp matrix
		mSkyBox->getComponent<Transform>()->getLocalTransform(args.shaderMatrix.world);
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

		// Draw the skybox
		if (mSkyBox->getComponent<MeshRenderer>())
		{
			mSkyBox->getComponent<MeshRenderer>()->bind();
			mDeviceContext->DrawIndexed(mSkyBox->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
			mSkyBox->getComponent<MeshRenderer>()->unbind();
		}

		// Clear the depth of the render targets
		mRenderTarget->clearDepth();
	}
}