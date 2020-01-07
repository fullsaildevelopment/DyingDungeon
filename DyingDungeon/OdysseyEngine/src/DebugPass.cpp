#include "DebugPass.h"
#include "DebugManager.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "Entity.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Light.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Transform.h"
#include "AnimatorDX11.h"
#include "AABB.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	DebugPass::DebugPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> renderWindow)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);
		mRenderState = renderDevice.createRenderState(Topology::LineList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);

		// Create the input layout
		D3D11_INPUT_ELEMENT_DESC cvLayout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/DebugVertexShader.cso", cvLayout, 2);
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/DebugPixelShader.cso", nullptr);
	}

	void DebugPass::preRender(RenderArgs& args)
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

		mRenderWindow->get3DRenderTarget()->bind();
		mVertexShader->bind();
		mPixelShader->bind();
		mRenderState->bind();
		DebugManager::getInstance().clearBuffer();
	}

	void DebugPass::render(RenderArgs& args)
	{
		for (std::shared_ptr<Entity> debugObject : args.entityList)
		{
			if (AnimatorDX11* animator = debugObject->getComponent<AnimatorDX11>())
			{
				if (animator->getDebugEnabled())
				{
					DirectX::XMFLOAT4X4 transform = debugObject->getComponent<Transform>()->getGlobalTransform();
					animator->debugDraw(transform, { 1.0f, 0.0f, 0.0f });
				}
			}

			if (AABB* aabb = debugObject->getComponent<AABB>())
			{
				aabb->debugDraw({ 0,0,1 });
			}

			for (std::shared_ptr<Entity> child : debugObject->getChildren())
			{
				if (AnimatorDX11* animator = child->getComponent<AnimatorDX11>())
				{
					if (animator->getDebugEnabled())
					{
						DirectX::XMFLOAT4X4 transform = child->getComponent<Transform>()->getGlobalTransform();
						animator->debugDraw(transform, { 1.0f, 0.0f, 0.0f });
					}
				}

				if (AABB* aabb = child->getComponent<AABB>())
				{
					child->getComponent<AABB>()->debugDraw({ 0,0,1 });
					aabb->debugDraw({ 0,0,1 });
				}
			}
		}

		for (std::shared_ptr<Light> lightObject : args.lightList)
		{
			lightObject->debugDraw();
		}

		DebugManager::getInstance().bind();
		mDeviceContext->Draw(DebugManager::getInstance().getNumberOfVertices(), 0);
	}
}