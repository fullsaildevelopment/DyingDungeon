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
#include <fstream>
#include "EventManager.h"
#include "Texture.h"

namespace Odyssey
{
	ShadowPass::ShadowPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int texWidth, int texHeight)
	{
		// Get the device and context
		mDeviceContext = context;
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		mTexWidth = texWidth;
		mTexHeight = texHeight;

		// Create the render state
		mRenderState = renderDevice->createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, true);

		// Create a shadow buffer to tell the pixel shader a shadow map is bound
		DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };
		shadowsEnabled.z = 1.0f / (float)texWidth;
		shadowsEnabled.w = 1.0f / (float)texWidth;
		mShadowBuffer = renderDevice->createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(DirectX::XMFLOAT4)), &shadowsEnabled);
		mShadowBuffer->bind(mDeviceContext, 2, ShaderType::PixelShader);

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
		mVertexShader = renderDevice->createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/DepthVertexShader.cso", vShaderLayout, 7);
		mPixelShader = renderDevice->createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/DepthPixelShader.cso", nullptr, 0);

		// Create the shadow map for dynamic objects
		mDynamicTarget = renderDevice->createRenderTarget(texWidth, texHeight, false);
		mDynamicTarget->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texHeight, 4096);
		mDynamicTarget->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// Create the shadow map viewport
		std::shared_ptr<Viewport> dynamicViewport = renderDevice->createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		mDynamicTarget->setViewport(dynamicViewport);

		// Create the shadow map for static objects
		mStaticTarget = renderDevice->createRenderTarget(texWidth, texHeight, false);
		mStaticTarget->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texWidth, texHeight);
		mStaticTarget->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		std::shared_ptr<Viewport> staticViewport = renderDevice->createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		mStaticTarget->setViewport(staticViewport);

		//std::shared_ptr<Texture> t = renderDevice.createTexture(TextureType::Shadow, "ShadowMap.png");
		//t->bind(8);
		renderStatic = true;
		renderDynamic = true;

		// Subscribe to the scene change event
		EventManager::getInstance().subscribe(this, &ShadowPass::onSceneChange);
	}

	void ShadowPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		if (renderPackage.shadowLight == nullptr)
		{
			DirectX::XMFLOAT4 shadowsEnabled = { 0.0f, 0.0f, 0.0f, 0.0f };
			mShadowBuffer->updateData(mDeviceContext, &shadowsEnabled);
			return;
		}
		else
		{
			DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };
			shadowsEnabled.z = 1.0f / (float)mTexWidth;
			shadowsEnabled.w = 1.0f / (float)mTexWidth;
			mShadowBuffer->updateData(mDeviceContext, &shadowsEnabled);
		}

		// Recalculate the light's transform and set the shader matrix's lightViewProj
		renderPackage.shadowLight->buildLightTransformProjection(renderPackage.sceneCenter, renderPackage.sceneRadius, args.perFrame.lightViewProj);

		// Update the per frame buffer
		updatePerFrameBuffer(mDeviceContext, args.perFrame, args.perFrameBuffer);

		// Bind the render state to the pipeline
		mRenderState->bind(mDeviceContext);

		// Bind the shaders
		mVertexShader->bind(mDeviceContext);
		mPixelShader->bind(mDeviceContext);

		if (renderDynamic)
		{
			// Unbind the depth texture from the pipeline
			mDynamicTarget->unbindDepthTexture(mDeviceContext, 9);

			// Clear the depth texture of the render target
			mDynamicTarget->clearDepth(mDeviceContext);

			// Bind the render target to the pipeline
			mDynamicTarget->bind(mDeviceContext);
		}

		if (renderStatic)
		{
			// Unbind the depth texture from the pipeline
			mStaticTarget->unbindDepthTexture(mDeviceContext, 8);

			// Clear the depth texture of the render target
			mStaticTarget->clearDepth(mDeviceContext);

			// Bind the render target to the pipeline
			mStaticTarget->bind(mDeviceContext);
		}
	}

	void ShadowPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		if (renderPackage.shadowLight == nullptr)
		{
			DirectX::XMFLOAT4 shadowsEnabled = { 0.0f, 0.0f, 0.0f, 0.0f };
			mShadowBuffer->updateData(mDeviceContext, &shadowsEnabled);
			return;
		}

		std::vector<RenderObject*> dynamicObjects;
		std::vector<RenderObject*> staticObjects;

		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.perFrame.view);

		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> performance;
		mDeviceContext->QueryInterface(__uuidof(performance), &performance);

		if (renderStatic || renderDynamic)
		{
			// Iterate over each scene object in the render list
			for (int i = 0; i < renderPackage.renderObjects.size(); i++)
			{
				// Check if the object has a mesh renderer
				if (renderPackage.renderObjects[i].meshRenderer)
				{
					if (renderPackage.renderObjects[i].meshRenderer->isActive())
					{
						if (renderPackage.renderObjects[i].meshRenderer->getEntity()->getStatic())
						{
							if (renderStatic)
								staticObjects.push_back(&renderPackage.renderObjects[i]);
						}
						else
						{
							if (renderDynamic)
								dynamicObjects.push_back(&renderPackage.renderObjects[i]);
						}
					}
				}
			}

			if (renderStatic)
			{
				performance->BeginEvent(L"Static Shadow Pass");
				renderStaticObjects(staticObjects, args);
				mStaticTarget->bindDepthTexture(mDeviceContext, 8);
				performance->EndEvent();
			}

			if (renderDynamic)
			{
				performance->BeginEvent(L"Dynamic Shadow Pass");
				renderDynamicObjects(dynamicObjects, args);
				mDynamicTarget->bindDepthTexture(mDeviceContext, 9);
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

	void ShadowPass::renderDynamicObjects(std::vector<RenderObject*> dynamicList, RenderArgs& args)
	{
		mDynamicTarget->bind(mDeviceContext);
		for (RenderObject* renderObject : dynamicList)
		{
			if (renderObject->animator)
			{
				renderObject->animator->bind(mDeviceContext);
			}

			if (renderObject->meshRenderer)
			{
				if (renderObject->meshRenderer->isActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			if (renderObject->animator)
			{
				renderObject->animator->unbind(mDeviceContext);
			}
		}
		mDynamicTarget->unBind(mDeviceContext);
	}

	void ShadowPass::renderStaticObjects(std::vector<RenderObject*> staticList, RenderArgs& args)
	{
		mStaticTarget->bind(mDeviceContext);
		for (RenderObject* renderObject : staticList)
		{
			if (renderObject->animator)
			{
				renderObject->animator->bind(mDeviceContext);
			}

			if (renderObject->meshRenderer)
			{
				if (renderObject->meshRenderer->isActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			if (renderObject->animator)
			{
				renderObject->animator->unbind(mDeviceContext);
			}
		}
		mStaticTarget->unBind(mDeviceContext);
	}

	void ShadowPass::renderSceneObject(RenderObject* renderObject, RenderArgs& args)
	{
		// Get the object's global transform and set the MVP acoordingly
		args.perObject.world = renderObject->transform->getGlobalTransform();

		// Update and bind the constant buffer
		updatePerObjectBuffer(mDeviceContext, args.perObject, args.perObjectBuffer);

		// Bind the vertex and index buffer of the mesh to the pipeline
		renderObject->meshRenderer->getMesh()->getIndexBuffer()->bind(mDeviceContext);
		renderObject->meshRenderer->getMesh()->getVertexBuffer()->bind(mDeviceContext);

		// Draw the mesh
		mDeviceContext->DrawIndexed(renderObject->meshRenderer->getMesh()->getNumberOfIndices(), 0, 0);
	}
}