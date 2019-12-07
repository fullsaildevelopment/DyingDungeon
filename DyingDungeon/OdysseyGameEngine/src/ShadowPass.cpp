#include "ShadowPass.h"
#include "RenderManager.h"
#include "RenderTarget.h"
#include "BufferManager.h"
#include "ShaderManager.h"
#include "Light.h"
#include "RenderState.h"
#include "Buffer.h"
#include "GameObject.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"

namespace Odyssey
{
	ShadowPass::ShadowPass(std::shared_ptr<Light> shadowLight, int texWidth, int texHeight)
	{
		// Get the device and context
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		// Set the properties of the shadow pass
		mShadowLight = shadowLight;

		// Create the shadow map render target
		mRenderTarget = std::make_shared <RenderTarget>(texWidth, texHeight, false);
		mRenderTarget->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texWidth, texHeight);
		mRenderTarget->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// Create the shadow map viewport
		Viewport shadowViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		mRenderTarget->setViewport(shadowViewport);

		// Create the render state
		mRenderState = std::make_unique<RenderState>(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, true);

		DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };

		mShadowBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(DirectX::XMFLOAT4), &shadowsEnabled);

		// Create the vertex shader
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

		// Create the depth vertex and pixel shaders
		mVertexShader = ShaderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/DepthVertexShader.cso", vShaderLayout, 7);
		mPixelShader = ShaderManager::getInstance().createShader(ShaderType::PixelShader, "../OdysseyGameEngine/shaders/DepthPixelShader.cso", nullptr, 0);
	}

	void ShadowPass::preRender(RenderArgs& args)
	{
		// Recalculate the light's transform and set the shader matrix's lightViewProj
		float sceneRadius = 75.0f;
		DirectX::XMFLOAT3 sceneCenter = { -30.0f, 0.0f, 25.0f };
		DirectX::XMFLOAT4X4 lightTransform = mShadowLight->buildLightTransform(sceneRadius, sceneCenter);
		DirectX::XMStoreFloat4x4(&args.shaderMatrix.lightViewProj, DirectX::XMLoadFloat4x4(&lightTransform));

		// Unbind the depth texture from the pipeline
		mRenderTarget->unbindDepthTexture();

		// Clear the depth texture of the render target
		mRenderTarget->clearDepth();

		// Bind the render target to the pipeline
		mRenderTarget->bind();

		// Bind the render state to the pipeline
		mRenderState->bind();

		// Update the shadow buffer to set the pixel shader to use shadows
		mShadowBuffer->bind(2, ShaderType::PixelShader);

		mVertexShader->bind();
		mPixelShader->bind();
	}

	void ShadowPass::render(RenderArgs& args)
	{
		// Iterate over each scene object in the render list
		for (std::shared_ptr<GameObject> renderObject : args.renderList)
		{
			// If the object has an animator, bind it to the vertex shader
			if (Animator* animator = renderObject->getComponent<Animator>())
			{
				animator->bind();
			}

			// If the object has a mesh renderer, render it
			if (MeshRenderer* meshRenderer = renderObject->getComponent<MeshRenderer>())
			{
				if (meshRenderer->getActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			// Iterate through the object's children and perform the same rendering checks
			for (std::shared_ptr<GameObject> child : renderObject->getChildren())
			{
				// If the object has a mesh renderer, render it
				if (MeshRenderer* meshRenderer = child->getComponent<MeshRenderer>())
				{
					if (meshRenderer->getActive())
					{
						renderSceneObject(child, args);
					}
				}
			}

			// If the original render object has an animator, unbind it to the vertex shader
			if (Animator* animator = renderObject->getComponent<Animator>())
			{
				animator->unbind();
			}
		}

		// Unbind the render target from the pipeline
		mRenderTarget->unBind();

		// Bind the depth texture to the pixel shader pipeline
		mRenderTarget->bindDepthTexture();
	}

	void ShadowPass::renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args)
	{
		// Get the object's global transform and set the MVP acoordingly
		object->getComponent<Transform>()->getGlobalTransform(args.shaderMatrix.world);

		// Update and bind the constant buffer
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

		// Bind the vertex and index buffer of the mesh to the pipeline
		object->getComponent<MeshRenderer>()->getMesh()->getIndexBuffer()->bind();
		object->getComponent<MeshRenderer>()->getMesh()->getVertexBuffer()->bind();

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
	}
}