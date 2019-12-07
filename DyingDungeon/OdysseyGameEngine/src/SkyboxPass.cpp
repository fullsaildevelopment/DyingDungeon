#include "SkyboxPass.h"
#include "RenderManager.h"
#include "RenderState.h"
#include "ShaderManager.h"
#include "SamplerState.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "SceneObject.h"
#include "MeshRenderer.h"
#include "Mesh.h"

namespace Odyssey
{
	SkyboxPass::SkyboxPass(std::shared_ptr<SceneObject> skybox, std::shared_ptr<RenderTarget> renderTarget)
	{
		mSkyBox = skybox;
		mRenderTarget = renderTarget;
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());
		mRenderState = std::make_unique<RenderState>(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, true, true, false);
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
		mVertexShader = ShaderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/SkyboxVertexShader.cso", vShaderLayout, 7);
		mPixelShader = ShaderManager::getInstance().createShader(ShaderType::PixelShader, "../OdysseyGameEngine/shaders/SkyboxPixelShader.cso", nullptr);


		SamplerState linear(ComparisonFunc::COMPARISON_NEVER, D3D11_FILTER_ANISOTROPIC, 0);

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
		mSkyBox->setPosition(camPos.x, camPos.y, camPos.z);

		// Get the global transform of the skybox and set the mvp matrix
		mSkyBox->getGlobalTransform(args.shaderMatrix.world);
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

		// Draw the skybox
		if (mSkyBox->hasMeshRenderer())
		{
			mSkyBox->getMeshRenderer()->bind();
			mDeviceContext->DrawIndexed(mSkyBox->getMeshRenderer()->getMesh()->getNumberOfIndices(), 0, 0);
			mSkyBox->getMeshRenderer()->unbind();
		}

		// Clear the depth of the render targets
		mRenderTarget->clearDepth();
	}
}