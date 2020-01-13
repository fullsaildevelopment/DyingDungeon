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

		mRenderState = renderDevice->createRenderState(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);
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

		mRenderWindow->get3DRenderTarget()->bind(mDeviceContext);
		mRenderState->bind(mDeviceContext);
	}

	void TransparentPass::render(RenderArgs& args)
	{
		std::multimap<float, std::shared_ptr<Entity>> renderMap;

		for (std::shared_ptr<Entity> renderObject : args.transparentList)
		{
			if (renderObject->getComponent<ParticleSystem>())
			{
				// Depth sorting
				DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.camera->getComponent<Camera>()->getInverseViewMatrix());
				DirectX::XMFLOAT4X4 globalTransform = renderObject->getComponent<Transform>()->getGlobalTransform();
				view = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
				float depth = DirectX::XMVectorGetZ(view.r[3]);
				renderMap.insert(std::pair<float, std::shared_ptr<Entity>>(depth, renderObject));
			}
		}

		for (auto itr = renderMap.begin(); itr != renderMap.end(); itr++)
		{
			renderSceneObject(itr->second, args);
		}
	}
	void TransparentPass::renderSceneObject(std::shared_ptr<Entity> object, RenderArgs& args)
	{
		// Set the global transform for the mesh and update the shader matrix buffer
		args.perObject.world = object->getComponent<Transform>()->getGlobalTransform();
		updatePerFrameBuffer(mDeviceContext, args.perFrame, args.perFrameBuffer);

		// Bind the mesh renderer
		object->getComponent<MeshRenderer>()->bind(mDeviceContext);

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
	}
}