#include "OpaquePass.h"
#include "SamplerState.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Frustum.h"
#include "AABB.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Shader.h"

namespace Odyssey
{
	OpaquePass::OpaquePass(RenderDevice& renderDevice, std::shared_ptr<RenderTarget> renderTarget)
	{
		// Get the device and context
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		// Get and store the render target
		mRenderTarget = renderTarget;

		// Create the render state for opaque objects
		mRenderState = renderDevice.createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, false);

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

		// Create the default pixel shader
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyGameEngine/shaders/LitPixelShader.cso", nullptr);
		
		// Create the default vertex shader
		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/VertexShader.cso", vShaderLayout, 7);

		std::shared_ptr<SamplerState> linear = renderDevice.createSamplerState(ComparisonFunc::COMPARISON_NEVER, D3D11_FILTER_ANISOTROPIC, 0);
		std::shared_ptr<SamplerState> shadow = renderDevice.createSamplerState(ComparisonFunc::COMPARISON_LESS, D3D11_FILTER_COMPARISON_ANISOTROPIC, 1);
		mPixelShader->addSampler(linear);
		mPixelShader->addSampler(shadow);
	}

	void OpaquePass::preRender(RenderArgs& args)
	{
		// Get the view and projection matrices from the camera and set them in the constant buffer variables
		args.shaderMatrix.view = args.camera->getInverseViewMatrix();
		args.shaderMatrix.proj = args.camera->getProjectionMatrix();
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

		// Bind the render target
		mRenderTarget->bind();

		// Bind the render state
		mRenderState->bind();

		mVertexShader->bind();
		mPixelShader->bind();
	}

	void OpaquePass::render(RenderArgs& args)
	{
		std::multimap<float, std::shared_ptr<GameObject>> renderMap;

		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.camera->getInverseViewMatrix());
		DirectX::XMFLOAT4X4 globalTransform;

		// Iterate over each object in the render list
		for (std::shared_ptr<GameObject> renderObject : args.renderList)
		{
			// If the object has a mesh renderer, render it
			if (MeshRenderer* meshRenderer = renderObject->getComponent<MeshRenderer>())
			{
				if (meshRenderer->getActive())
				{
					if (mFrustumCull == false || args.camera->mFrustum->checkFrustumView(*(renderObject->getComponent<AABB>())))
					{
						// Depth sorting
						renderObject->getComponent<Transform>()->getGlobalTransform(globalTransform);
						view = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
						float depth = DirectX::XMVectorGetZ(view.r[3]);
						renderMap.insert(std::pair<float, std::shared_ptr<GameObject>>(depth, renderObject));
					}
				}
			}

			for (std::shared_ptr<GameObject> child : renderObject->getChildren())
			{
				if (MeshRenderer* meshRenderer = child->getComponent<MeshRenderer>())
				{
					if (meshRenderer->getActive())
					{
						if (mFrustumCull == false || args.camera->mFrustum->checkFrustumView(*(child->getComponent<AABB>())))
						{
							// Depth Sorting
							child->getComponent<Transform>()->getGlobalTransform(globalTransform);
							view = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
							float depth = DirectX::XMVectorGetZ(view.r[3]);
							renderMap.insert(std::pair<float, std::shared_ptr<GameObject>>(depth, child));
						}
					}
				}
			}
		}

		for (auto itr = renderMap.begin(); itr != renderMap.end(); itr++)
		{
			if (Animator* rootAnimator = itr->second->getRootComponent<Animator>())
			{
				rootAnimator->bind();
			}
			renderSceneObject(itr->second, args);
			if (Animator* rootAnimator = itr->second->getRootComponent<Animator>())
			{
				rootAnimator->unbind();
			}
		}
	}

	void OpaquePass::setFrustumCullEnable(bool enable)
	{
		mFrustumCull = enable;
	}

	void OpaquePass::renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args)
	{
		// Set the global transform for the mesh and update the shader matrix buffer
		object->getComponent<Transform>()->getGlobalTransform(args.shaderMatrix.world);
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

		// Bind the mesh renderer
		object->getComponent<MeshRenderer>()->bind();

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);

		object->getComponent<MeshRenderer>()->unbind();
	}
}