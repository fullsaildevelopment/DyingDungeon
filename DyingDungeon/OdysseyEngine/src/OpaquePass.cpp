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
	OpaquePass::OpaquePass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow)
	{
		// Get the device and context
		mDeviceContext = context;
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		// Get and store the render target
		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);

		// Create the render state for opaque objects
		mRenderState = renderDevice->createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, false);

		mLightingBuffer = renderDevice->createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
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
		mPixelShader = renderDevice->createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/LitPixelShader.cso", nullptr);

		// Create the default vertex shader
		mVertexShader = renderDevice->createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/VertexShader.cso", vShaderLayout, 7);
		mFrustumCull = true;

		// Create the per object lighting buffer
		mLightingBuffer = renderDevice->createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(SceneLighting)), nullptr);
	}

	void OpaquePass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Update the buffer
		updatePerFrameBuffer(mDeviceContext, args.perFrame, args.perFrameBuffer);

		// Bind the render target
		mRenderWindow->get3DRenderTarget()->bind(mDeviceContext);

		// Bind the render state
		mRenderState->bind(mDeviceContext);

		mVertexShader->bind(mDeviceContext);
		mPixelShader->bind(mDeviceContext);
	}

	void OpaquePass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		std::multimap<float, RenderObject*> renderMap;
		UINT numCulled = 0;
		UINT total = 0;
		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.perFrame.view);
		DirectX::XMFLOAT4X4 globalTransform;

		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> performance;
		mDeviceContext->QueryInterface(__uuidof(performance), &performance);
		performance->BeginEvent(L"Opaque Pass");

		// Iterate over each object in the render list
		if (renderPackage.camera)
		{
			Frustum* frustum = renderPackage.frustum;
			for (int i = 0; i < renderPackage.renderObjects.size(); i++)
			{
				if (renderPackage.renderObjects[i].meshRenderer->isActive() && renderPackage.renderObjects[i].meshRenderer->getEntity()->isActive() && renderPackage.renderObjects[i].meshRenderer->getEntity()->isVisible())
				{
					if (mFrustumCull == true)
					{
						AABB* aabb = renderPackage.renderObjects[i].aabb;
						bool inFrustum = frustum->checkFrustumView(*(aabb));
						if (renderPackage.renderObjects[i].transform->getEntity()->getStatic() == false || inFrustum)
						{
							// Depth sorting
							Transform* transform = renderPackage.renderObjects[i].transform;
							globalTransform = transform->getGlobalTransform();
							DirectX::XMMATRIX viewSpace = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
							float depth = DirectX::XMVectorGetZ(viewSpace.r[3]);
							renderMap.insert(std::pair<float, RenderObject*>(depth, &renderPackage.renderObjects[i]));
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
				updateLightingBuffer(itr->second, args, renderPackage);

				if (itr->second->animator)
				{
					itr->second->animator->bind(mDeviceContext);
				}

				renderSceneObject(itr->second, args);

				if (itr->second->animator)
				{
					itr->second->animator->unbind(mDeviceContext);
				}
			}
		}
		
		EventManager::getInstance().publish(new RenderEvent(total, static_cast<unsigned int>(renderMap.size()), numCulled));
		performance->EndEvent();
	}

	void OpaquePass::setFrustumCullEnable(bool enable)
	{
		mFrustumCull = enable;
	}

	void OpaquePass::updateLightingBuffer(RenderObject* renderObject, RenderArgs& args, RenderPackage& renderPackage)
	{
		// Generate a list of lights on a per-object basis
		SceneLighting sceneLighting;
		sceneLighting.numLights = 0;

		// Set the camera's position for specular highlighting
		sceneLighting.camPos = renderPackage.cameraPosition;

		for (Light* light : renderPackage.sceneLights)
		{
			ShaderLight shaderLight;
			if (sceneLighting.numLights != 8)
			{
				if (light->getLightType() == LightType::Point)
				{
					Sphere sphere;
					DirectX::XMFLOAT4 pos = light->getPosition();
					sphere.center = { pos.x, pos.y, pos.z };
					sphere.radius = light->getRange();
					if (renderObject->aabb->testAABBtoSphere(sphere))
					{
						shaderLight.color = light->getColor();
						shaderLight.intensity = light->getIntensity();
						shaderLight.range = light->getRange();
						shaderLight.lightType = (UINT)light->getLightType();
						shaderLight.cone = light->getSpotAngle();
						shaderLight.worldPosition = light->getPosition();
						shaderLight.worldDirection = light->getDirection();
						sceneLighting.sceneLights[sceneLighting.numLights] = shaderLight;
						sceneLighting.numLights++;
					}
				}
				else
				{
					// Directional and spot lights are automatically added to the light list
					shaderLight.color = light->getColor();
					shaderLight.intensity = light->getIntensity();
					shaderLight.range = light->getRange();
					shaderLight.lightType = (UINT)light->getLightType();
					shaderLight.cone = light->getSpotAngle();
					shaderLight.worldPosition = light->getPosition();
					shaderLight.worldDirection = light->getDirection();
					sceneLighting.sceneLights[sceneLighting.numLights] = shaderLight;
					sceneLighting.numLights++;
				}
			}

		}

		// Set the lighting constant buffer
		mLightingBuffer->updateData(mDeviceContext, &sceneLighting);
		mLightingBuffer->bind(mDeviceContext, 1, ShaderType::PixelShader);
	}

	void OpaquePass::renderSceneObject(RenderObject* renderObject, RenderArgs& args)
	{
		// Set the global transform for the mesh and update the shader matrix buffer
		args.perObject.world = renderObject->transform->getGlobalTransform();
		updatePerObjectBuffer(mDeviceContext, args.perObject, args.perObjectBuffer);

		// Bind the mesh renderer
		renderObject->meshRenderer->bind(mDeviceContext);

		// Draw the mesh
		mDeviceContext->DrawIndexed(renderObject->meshRenderer->getMesh()->getNumberOfIndices(), 0, 0);
	}
}