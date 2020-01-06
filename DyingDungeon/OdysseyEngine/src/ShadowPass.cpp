#include "ShadowPass.h"
#include "RenderTarget.h"
#include "Light.h"
#include "RenderState.h"
#include "Buffer.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Frustum.h"
#include "AABB.h"
#include "Camera.h"
#include "AnimatorDX11.h"
#include "ScreenGrab.h"
#include "WICTextureLoader.h"
#include <fstream>
#include "EventManager.h"
#include "Texture.h"

namespace Odyssey
{
	ShadowPass::ShadowPass(RenderDevice& renderDevice, std::shared_ptr<Light> shadowLight, int texWidth, int texHeight)
	{
		// Get the device and context
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		// Set the properties of the shadow pass
		mShadowLight = shadowLight;

		// Create the render state
		mRenderState = renderDevice.createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, true);

		// Create a shadow buffer to tell the pixel shader a shadow map is bound
		DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };
		shadowsEnabled.z = 1.0f / texWidth;
		shadowsEnabled.w = 1.0f / texWidth;
		mShadowBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(DirectX::XMFLOAT4)), &shadowsEnabled);
		mShadowBuffer->bind(2, ShaderType::PixelShader);

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
		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/DepthVertexShader.cso", vShaderLayout, 7);
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/DepthPixelShader.cso", nullptr, 0);

		// Create the shadow map for dynamic objects
		mDynamicTarget = renderDevice.createRenderTarget(texWidth, texHeight, false);
		mDynamicTarget->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texHeight, 4096);
		mDynamicTarget->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// Create the shadow map viewport
		std::shared_ptr<Viewport> dynamicViewport = renderDevice.createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		mDynamicTarget->setViewport(dynamicViewport);

		// Create the shadow map for static objects
		mStaticTarget = renderDevice.createRenderTarget(texWidth, texHeight, false);
		mStaticTarget->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texWidth, texHeight);
		mStaticTarget->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		std::shared_ptr<Viewport> staticViewport = renderDevice.createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		mStaticTarget->setViewport(staticViewport);

		//std::shared_ptr<Texture> t = renderDevice.createTexture(TextureType::Shadow, "ShadowMap.png");
		//t->bind(8);
		renderStatic = true;
		renderDynamic = true;

		// Subscribe to the scene change event
		EventManager::getInstance().subscribe(this, &ShadowPass::onSceneChange);
	}

	void ShadowPass::preRender(RenderArgs& args)
	{
		// Recalculate the light's transform and set the shader matrix's lightViewProj
		float sceneRadius = 50.0f;
		DirectX::XMFLOAT3 sceneCenter = { 0.0f, 0.0f, 0.0f };
		mShadowLight->buildLightTransform(sceneCenter, sceneRadius, args.perFrame.view);
		mShadowLight->buildLightTransformProjection(sceneCenter, sceneRadius, args.perFrame.lightViewProj);

		// Update the per frame buffer
		updatePerFrameBuffer(args.perFrame, args.perFrameBuffer);

		// Bind the render state to the pipeline
		mRenderState->bind();

		// Bind the shaders
		mVertexShader->bind();
		mPixelShader->bind();

		if (renderDynamic)
		{
			// Unbind the depth texture from the pipeline
			mDynamicTarget->unbindDepthTexture(9);

			// Clear the depth texture of the render target
			mDynamicTarget->clearDepth();

			// Bind the render target to the pipeline
			mDynamicTarget->bind();
		}

		if (renderStatic)
		{
			// Unbind the depth texture from the pipeline
			mStaticTarget->unbindDepthTexture(8);

			// Clear the depth texture of the render target
			mStaticTarget->clearDepth();

			// Bind the render target to the pipeline
			mStaticTarget->bind();
		}
	}

	void ShadowPass::render(RenderArgs& args)
	{
		std::vector<std::shared_ptr<Entity>> dynamicObjects;
		std::vector<std::shared_ptr<Entity>> staticObjects;

		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.perFrame.view);

		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> performance;
		mDeviceContext->QueryInterface(__uuidof(performance), &performance);

		if (renderStatic || renderDynamic)
		{
			// Iterate over each scene object in the render list
			for (std::shared_ptr<Entity> renderObject : args.renderList)
			{
				// Check if the object has a mesh renderer
				if (MeshRenderer* meshRenderer = renderObject->getComponent<MeshRenderer>())
				{
					if (meshRenderer->isActive())
					{
						if (renderObject->getStatic())
						{
							if (renderStatic)
								staticObjects.push_back(renderObject);
						}
						else
						{
							if (renderDynamic)
								dynamicObjects.push_back(renderObject);
						}
					}
				}

				for (std::shared_ptr<Entity> child : renderObject->getChildren())
				{
					if (MeshRenderer* meshRenderer = child->getComponent<MeshRenderer>())
					{
						if (meshRenderer->isActive())
						{
							if (renderObject->getStatic())
							{
								if (renderStatic)
								{
									staticObjects.push_back(renderObject);
									break;
								}
							}
							else
							{
								if (renderDynamic)
								{
									dynamicObjects.push_back(renderObject);
									break;
								}
							}
						}
					}
				}
			}

			if (renderStatic)
			{
				performance->BeginEvent(L"Static Shadow Pass");
				renderStaticObjects(staticObjects, args);
				mStaticTarget->bindDepthTexture(8);
				performance->EndEvent();
			}

			if (renderDynamic)
			{
				performance->BeginEvent(L"Dynamic Shadow Pass");
				renderDynamicObjects(dynamicObjects, args);
				mDynamicTarget->bindDepthTexture(9);
				performance->EndEvent();
			}
		}

		if (renderStatic)
		{
			// Save out the shadowmap and stop rendering static objects
			//HRESULT hr = DirectX::SaveWICTextureToFile(mDeviceContext.Get(), mStaticTarget->getRenderTexture()->getTextureResource(), GUID_ContainerFormatPng, L"assets/textures/ShadowMap.png");
			//assert(!FAILED(hr));
			renderStatic = false;
		}
	}

	void ShadowPass::onSceneChange(SceneChangeEvent* evnt)
	{
		renderStatic = true;
	}

	void ShadowPass::renderDynamicObjects(std::vector<std::shared_ptr<Entity>> dynamicList, RenderArgs& args)
	{
		mDynamicTarget->bind();
		for (std::shared_ptr<Entity> renderObject : dynamicList)
		{
			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->bind();
			}

			if (MeshRenderer* meshRenderer = renderObject->getComponent<MeshRenderer>())
			{
				if (meshRenderer->isActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			for (std::shared_ptr<Entity> child : renderObject->getChildren())
			{
				if (MeshRenderer* meshRenderer = child->getComponent<MeshRenderer>())
				{
					if (meshRenderer->isActive())
					{
						renderSceneObject(child, args);
					}
				}
			}

			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->unbind();
			}
		}
		mDynamicTarget->unBind();
	}

	void ShadowPass::renderStaticObjects(std::vector<std::shared_ptr<Entity>> staticList, RenderArgs& args)
	{
		mStaticTarget->bind();
		for (std::shared_ptr<Entity> renderObject : staticList)
		{
			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->bind();
			}

			if (MeshRenderer* meshRenderer = renderObject->getComponent<MeshRenderer>())
			{
				if (meshRenderer->isActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			for (std::shared_ptr<Entity> child : renderObject->getChildren())
			{
				if (MeshRenderer* meshRenderer = child->getComponent<MeshRenderer>())
				{
					if (meshRenderer->isActive())
					{
						renderSceneObject(child, args);
					}
				}
			}

			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->unbind();
			}
		}
		mStaticTarget->unBind();
	}

	void ShadowPass::renderSceneObject(std::shared_ptr<Entity> object, RenderArgs& args)
	{
		// Get the object's global transform and set the MVP acoordingly
		args.perObject.world = object->getComponent<Transform>()->getGlobalTransform();

		// Update and bind the constant buffer
		updatePerObjectBuffer(args.perObject, args.perObjectBuffer);

		// Bind the vertex and index buffer of the mesh to the pipeline
		object->getComponent<MeshRenderer>()->getMesh()->getIndexBuffer()->bind();
		object->getComponent<MeshRenderer>()->getMesh()->getVertexBuffer()->bind();

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
	}
}