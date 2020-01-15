#include "TransparentPass.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "Entity.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Mesh.h"
#include "ParticleSystem.h"
#include "MeshRenderer.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	TransparentPass::TransparentPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow)
	{
		mDeviceContext = context;
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);
	}

	void TransparentPass::preRender(RenderArgs& args)
	{
		// Set the view
		if (Camera* camera = args.camera->getComponent<Camera>())
		{
			args.perFrame.view = camera->getInverseViewMatrix();

			// Calculate and set view proj
			DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), DirectX::XMLoadFloat4x4(&camera->getProjectionMatrix()));
			DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);
		}
		
		// Update the buffer
		updatePerFrameBuffer(mDeviceContext, args.perFrame, args.perFrameBuffer);
		args.perFrameBuffer->bind(mDeviceContext, 0, ShaderType::GeometryShader);

		mRenderWindow->get3DRenderTarget()->bind(mDeviceContext);
	}

	void TransparentPass::render(RenderArgs& args)
	{
		// Bind the world
		for (ParticleSystem* ps : args.systemList)
		{
			args.perObject.world = ps->getEntity()->getComponent<Transform>()->getGlobalTransform();
			updatePerObjectBuffer(mDeviceContext, args.perObject, args.perObjectBuffer);
			args.perObjectBuffer->bind(mDeviceContext, 1, ShaderType::GeometryShader);

			ps->run(mDeviceContext);
		}
	}
}