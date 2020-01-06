#include "OpaquePass.h"
#include "SamplerState.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Frustum.h"
#include "AABB.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"
#include "RenderDevice.h"
#include "AnimatorDX11.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"
#include "EngineProfiler.h"
#include "EventManager.h"

namespace Odyssey
{
	OpaquePass::OpaquePass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> renderWindow)
	{
		// Get the device and context
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		// Get and store the render target
		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);

		// Create the render state for opaque objects
		mRenderState = renderDevice.createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, false);

		mLightingBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(SceneLighting)), nullptr);

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
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/LitPixelShader.cso", nullptr);

		// Create the default vertex shader
		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/VertexShader.cso", vShaderLayout, 7);
		mFrustumCull = true;

		// Create the per object lighting buffer
		mLightingBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(SceneLighting)), nullptr);
	}

	void OpaquePass::preRender(RenderArgs& args)
	{
		// Set the view
		if (Camera* camera = args.camera->getComponent<Camera>())
		{
			args.perFrame.view = camera->getInverseViewMatrix();
			// Calculate and set view proj
			DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), DirectX::XMLoadFloat4x4(&camera->getProjectionMatrix()));
			DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);
			// Update the buffer
			updatePerFrameBuffer(args.perFrame, args.perFrameBuffer);
		}

		// Calculate and set view proj
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), DirectX::XMLoadFloat4x4(&args.camera->getComponent<Camera>()->getProjectionMatrix()));
		DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);

		// Update the buffer
		updatePerFrameBuffer(args.perFrame, args.perFrameBuffer);

		// Bind the render target
		mRenderWindow->get3DRenderTarget()->bind();

		// Bind the render state
		mRenderState->bind();

		mVertexShader->bind();
		mPixelShader->bind();
	}

	void OpaquePass::render(RenderArgs& args)
	{
		std::multimap<float, MeshRenderer*> renderMap;
		UINT numCulled = 0;
		UINT total = 0;
		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.perFrame.view);
		DirectX::XMFLOAT4X4 globalTransform;

		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> performance;
		mDeviceContext->QueryInterface(__uuidof(performance), &performance);
		performance->BeginEvent(L"Opaque Pass");

		// Iterate over each object in the render list
		for (MeshRenderer* meshRenderer : args.renderList)
		{
			if (meshRenderer->isActive())
			{
				if (mFrustumCull == true)
				{
					Entity* renderObject = meshRenderer->getEntity();

					if (renderObject->getStatic() == false || args.camera->getComponent<Camera>()->getFrustum()->checkFrustumView(*(renderObject->getComponent<AABB>())))
					{
						// Depth sorting
						globalTransform = renderObject->getComponent<Transform>()->getGlobalTransform();
						DirectX::XMMATRIX viewSpace = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
						float depth = DirectX::XMVectorGetZ(viewSpace.r[3]);
						renderMap.insert(std::pair<float, MeshRenderer*>(depth, meshRenderer));
					}
					else
					{
						numCulled++;
					}
				}
			}
			total++;
		}
		for (auto itr = renderMap.begin(); itr != renderMap.end(); itr++)
		{
			Entity* renderObject = itr->second->getEntity();

			updateLightingBuffer(renderObject, args);

			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->bind();
			}
			updateLightingBuffer(renderObject, args);
			renderSceneObject(renderObject, args);
			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->unbind();
			}
		}

		EventManager::getInstance().publish(new RenderEvent(total, static_cast<unsigned int>(renderMap.size()), numCulled));
		performance->EndEvent();
	}

	void OpaquePass::setFrustumCullEnable(bool enable)
	{
		mFrustumCull = enable;
	}

	void OpaquePass::updateLightingBuffer(Entity* entity, RenderArgs& args)
	{
		// Generate a list of lights on a per-object basis
		SceneLighting sceneLighting;
		sceneLighting.numLights = 0;

		// Set the camera's position for specular highlighting
		sceneLighting.camPos = args.camera->getComponent<Transform>()->getPosition();

		for (std::shared_ptr<Light> light : args.lightList)
		{
			if (sceneLighting.numLights != 8)
			{
				if (light->getLightType() == LightType::Point)
				{
					Sphere sphere;
					light->getPosition(sphere.center);
					sphere.radius = light->getRange();
					if (entity->getComponent<AABB>()->testAABBtoSphere(sphere))
					{
						sceneLighting.sceneLights[sceneLighting.numLights] = *light;
						sceneLighting.numLights++;
					}
				}
				else
				{
					// Directional and spot lights are automatically added to the light list
					sceneLighting.sceneLights[sceneLighting.numLights] = *light;
					sceneLighting.numLights++;
				}
			}

		}

		// Set the lighting constant buffer
		mLightingBuffer->updateData(&sceneLighting);
		mLightingBuffer->bind(1, ShaderType::PixelShader);
	}

	void OpaquePass::renderSceneObject(Entity* object, RenderArgs& args)
	{
		// Set the global transform for the mesh and update the shader matrix buffer
		args.perObject.world = object->getComponent<Transform>()->getGlobalTransform();
		updatePerObjectBuffer(args.perObject, args.perObjectBuffer);

		// Bind the mesh renderer
		object->getComponent<MeshRenderer>()->bind();

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
	}
}