#include "ShadowPass.h"
#include "RenderTarget.h"
#include "Light.h"
#include "RenderState.h"
#include "Buffer.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Shader.h"
#include "Frustum.h"
#include "AABB.h"
#include "Camera.h"
#include "AnimatorDX11.h"
#include <fstream>
#include "EventManager.h"
#include "Texture.h"

namespace Odyssey
{
	ShadowPass::ShadowPass(int texWidth, int texHeight)
	{
		// Store the texture dimensions
		mTexWidth = texWidth;
		mTexHeight = texHeight;

		// Store the render manager instance
		RenderManager& renderManager = RenderManager::getInstance();

		// Create the render state
		mRenderState = renderManager.createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, true);

		// Create a shadow buffer to tell the pixel shader a shadow map is bound
		DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };
		shadowsEnabled.z = 1.0f / (float)texWidth;
		shadowsEnabled.w = 1.0f / (float)texWidth;
		mShadowBuffer = renderManager.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(DirectX::XMFLOAT4)), &shadowsEnabled);

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
		mVertexShader = RenderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/DepthVertexShader.cso", vShaderLayout, 7);
		mPixelShader = RenderManager::getInstance().createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/DepthPixelShader.cso", nullptr, 0);

		// Create the shadow map for dynamic objects
		mDynamicTarget = RenderManager::getInstance().createRenderTarget(texWidth, texHeight, false);
		RenderManager::getInstance().getRenderTarget(mDynamicTarget)->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texHeight, 4096);
		RenderManager::getInstance().getRenderTarget(mDynamicTarget)->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// Create the shadow map viewport
		int dynamicViewport = RenderManager::getInstance().createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		RenderManager::getInstance().getRenderTarget(mDynamicTarget)->setViewport(dynamicViewport);

		// Create the shadow map for static objects
		mStaticTarget = RenderManager::getInstance().createRenderTarget(texWidth, texHeight, false);
		RenderManager::getInstance().getRenderTarget(mStaticTarget)->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texWidth, texHeight);
		RenderManager::getInstance().getRenderTarget(mStaticTarget)->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// Create and set the viewport for the static shadow map
		int staticViewport = RenderManager::getInstance().createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		RenderManager::getInstance().getRenderTarget(mStaticTarget)->setViewport(staticViewport);

		//std::shared_ptr<Texture> t = renderDevice.createTexture(TextureType::Shadow, "ShadowMap.png");
		//t->bind(8);
		renderStatic = true;
		renderDynamic = true;

		// Subscribe to the scene change event
		EventManager::getInstance().subscribe(this, &ShadowPass::onSceneChange);
	}

	void ShadowPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Store the render manager instance
		RenderManager& renderManager = RenderManager::getInstance();

		// Check if we have a shadow light
		if (renderPackage.shadowLight == nullptr)
		{
			// Set the shadows to disabled
			DirectX::XMFLOAT4 shadowsEnabled = { 0.0f, 0.0f, 0.0f, 0.0f };
			renderManager.getBuffer(mShadowBuffer)->updateData(args.context, &shadowsEnabled);
			renderManager.getBuffer(mShadowBuffer)->bind(args.context, 2, ShaderType::PixelShader);
			return;
		}
		else
		{
			// Sets the shadows to enabled and set the texel size
			DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };
			shadowsEnabled.z = 1.0f / (float)mTexWidth;
			shadowsEnabled.w = 1.0f / (float)mTexWidth;
			renderManager.getBuffer(mShadowBuffer)->updateData(args.context, &shadowsEnabled);
			renderManager.getBuffer(mShadowBuffer)->bind(args.context, 2, ShaderType::PixelShader);
		}

		// Recalculate the light's transform and set the shader matrix's lightViewProj
		renderPackage.shadowLight->buildLightTransformProjection(renderPackage.sceneCenter, renderPackage.sceneRadius, args.perFrame.lightViewProj);

		// Update the per frame buffer
		updatePerFrameBuffer(args.context, args.perFrame, args.perFrameBuffer);

		// Bind the render state to the pipeline
		renderManager.getRenderState(mRenderState)->bind(args.context);

		// Bind the shaders
		renderManager.getShader(mVertexShader)->bind(args.context);
		renderManager.getShader(mPixelShader)->bind(args.context);

		// Check if we are rendering dynamic objects
		if (renderDynamic)
		{
			// Get the dynamic render target
			RenderTarget* dynamicTarget = renderManager.getRenderTarget(mDynamicTarget);

			// Unbind the depth texture from the pipeline
			dynamicTarget->unbindDepthTexture(args.context, 9);

			// Clear the depth texture of the render target
			dynamicTarget->clearDepth(args.context);

			// Bind the render target to the pipeline
			dynamicTarget->bind(args.context);
		}

		// Check if we are rendering static objects
		if (renderStatic)
		{
			// Get the static render target
			RenderTarget* staticTarget = renderManager.getRenderTarget(mStaticTarget);

			// Unbind the depth texture from the pipeline
			staticTarget->unbindDepthTexture(args.context, 8);

			// Clear the depth texture of the render target
			staticTarget->clearDepth(args.context);

			// Bind the render target to the pipeline
			staticTarget->bind(args.context);
		}
	}

	void ShadowPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Skip the rendering pass if there is no shadow light
		if (renderPackage.shadowLight == nullptr)
		{
			return;
		}

		// Get the view matrix
		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.perFrame.view);

		// Get the performance profiler from the device context
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> performance;
		args.context->QueryInterface(__uuidof(performance), &performance);

		// Sort dynamic and static objects
		std::vector<RenderObject*> dynamicObjects;
		std::vector<RenderObject*> staticObjects;

		if (renderStatic || renderDynamic)
		{
			// Iterate over each scene object in the render list
			for (int i = 0; i < renderPackage.renderObjects.size(); i++)
			{
				// Check if the object has a mesh renderer
				if (renderPackage.renderObjects[i].meshRenderer)
				{
					// Make sure the mesh renderer is active
					if (renderPackage.renderObjects[i].meshRenderer->isActive() && renderPackage.renderObjects[i].meshRenderer->isShadowCaster())
					{
						// Check if the entity is static or dynamic
						if (renderPackage.renderObjects[i].meshRenderer->getEntity()->getStatic())
						{
							// Add the object to the static render list
							if (renderStatic)
							{
								staticObjects.push_back(&renderPackage.renderObjects[i]);
							}
						}
						else
						{
							// Add the object to the dynamic render list
							if (renderDynamic)
							{
								dynamicObjects.push_back(&renderPackage.renderObjects[i]);
							}
						}
					}
				}
			}

			// Check if we are rendering static objects
			if (renderStatic)
			{
				// Begin performance profiling
				performance->BeginEvent(L"Static Shadow Pass");
				// Render the static objects
				renderStaticObjects(staticObjects, args);
				// Bind the depth map to the pixel shaders
				RenderManager::getInstance().getRenderTarget(mStaticTarget)->bindDepthTexture(args.context, 8);
				// Stop performance profiling
				performance->EndEvent();
			}

			// Check if we are rendering dynamic objects
			if (renderDynamic)
			{
				// Begin performance profiling
				performance->BeginEvent(L"Dynamic Shadow Pass");
				// Render the dynamic objects
				renderDynamicObjects(dynamicObjects, args);
				// Bind the depth map to the pixel shaders
				RenderManager::getInstance().getRenderTarget(mDynamicTarget)->bindDepthTexture(args.context, 9);
				// Stop performance profiling
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
		// Render static objects for the new scene
		renderStatic = true;
	}

	void ShadowPass::renderDynamicObjects(std::vector<RenderObject*> dynamicList, RenderArgs& args)
	{
		// Bind the dynamic render target
		RenderManager::getInstance().getRenderTarget(mDynamicTarget)->bind(args.context);

		// Render each dynamic object
		for (RenderObject* renderObject : dynamicList)
		{
			// Bind the animator if the object has one
			if (AnimatorDX11* animator = renderObject->entity->getRootComponent<AnimatorDX11>())
			{
				animator->bind(args.context);
			}

			// Render the object if the mesh renderer is active
			if (renderObject->meshRenderer)
			{
				if (renderObject->meshRenderer->isActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			// Unbind the animator if the object has one
			if (AnimatorDX11* animator = renderObject->entity->getRootComponent<AnimatorDX11>())
			{
				animator->unbind(args.context);
			}
		}

		// Unbind the dynamic render target
		RenderManager::getInstance().getRenderTarget(mDynamicTarget)->unBind(args.context);
	}

	void ShadowPass::renderStaticObjects(std::vector<RenderObject*> staticList, RenderArgs& args)
	{
		// Bind the static render target
		RenderManager::getInstance().getRenderTarget(mStaticTarget)->bind(args.context);

		// Render each static object
		for (RenderObject* renderObject : staticList)
		{
			// Bind the animator if the object has one
			if (AnimatorDX11* animator = renderObject->entity->getRootComponent<AnimatorDX11>())
			{
				animator->bind(args.context);
			}

			// Render the object if the mesh renderer is active
			if (renderObject->meshRenderer)
			{
				if (renderObject->meshRenderer->isActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			// Unbind the animator if the object has one
			if (AnimatorDX11* animator = renderObject->entity->getRootComponent<AnimatorDX11>())
			{
				animator->unbind(args.context);
			}
		}

		// Unbind the static render target
		RenderManager::getInstance().getRenderTarget(mStaticTarget)->unBind(args.context);
	}

	void ShadowPass::renderSceneObject(RenderObject* renderObject, RenderArgs& args)
	{
		// Get the object's global transform and set the MVP acoordingly
		args.perObject.world = renderObject->transform->getGlobalTransform();

		// Update and bind the constant buffer
		updatePerObjectBuffer(args.context, args.perObject, args.perObjectBuffer);

		// Bind the vertex and index buffer of the mesh to the pipeline
		renderObject->meshRenderer->getMesh()->getIndexBuffer()->bind(args.context);
		renderObject->meshRenderer->getMesh()->getVertexBuffer()->bind(args.context);

		// Draw the mesh
		args.context->DrawIndexed(renderObject->meshRenderer->getMesh()->getNumberOfIndices(), 0, 0);
	}
}