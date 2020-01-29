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
	DebugPass::DebugPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow)
	{
		mContext = context;
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);
		mRenderState = renderDevice->createRenderState(Topology::LineList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);

		// Create the input layout
		D3D11_INPUT_ELEMENT_DESC cvLayout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		mVertexShader = renderDevice->createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/DebugVertexShader.cso", cvLayout, 2);
		mPixelShader = renderDevice->createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/DebugPixelShader.cso", nullptr);
	}

	void DebugPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Update the buffer
		updatePerFrameBuffer(mContext, args.perFrame, args.perFrameBuffer);

		// Calculate and set view proj
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), 
			DirectX::XMLoadFloat4x4(&renderPackage.camera->getProjectionMatrix()));
		DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);
		// Update the buffer
		updatePerFrameBuffer(mContext, args.perFrame, args.perFrameBuffer);

		mRenderWindow->get3DRenderTarget()->bind(mContext);
		mVertexShader->bind(mContext);
		mPixelShader->bind(mContext);
		mRenderState->bind(mContext);
		DebugManager::getInstance().clearBuffer();
	}

	void DebugPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		for (RenderObject renderObject : renderPackage.renderObjects)
		{
			if (renderObject.animator)
			{
				if (renderObject.animator->getDebugEnabled())
				{
					DirectX::XMFLOAT4X4 transform = renderObject.transform->getLocalTransform();
					renderObject.animator->debugDraw(transform, { 1.0f, 0.0f, 0.0f });
				}
			}

			if (renderObject.aabb)
			{
				renderObject.aabb->debugDraw({ 0,0,1 });
			}
		}

		for (Light* lightObject : renderPackage.sceneLights)
		{
			lightObject->debugDraw();
		}

		DebugManager::getInstance().bind(mContext);
		mContext->Draw(DebugManager::getInstance().getNumberOfVertices(), 0);
	}
}