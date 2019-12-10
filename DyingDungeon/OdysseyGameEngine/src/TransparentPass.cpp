#include "TransparentPass.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Mesh.h"

namespace Odyssey
{
	TransparentPass::TransparentPass(RenderDevice& renderDevice, std::shared_ptr<RenderTarget> renderTarget)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		mRenderTarget = renderTarget;

		mRenderState = renderDevice.createRenderState(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);
	}

	void TransparentPass::preRender(RenderArgs& args)
	{
		// Set the view
		args.perFrame.view = args.camera->getInverseViewMatrix();
		// Calculate and set view proj
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), DirectX::XMLoadFloat4x4(&args.camera->getProjectionMatrix()));
		DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);
		// Update the buffer
		updatePerFrameBuffer(args.perFrame, args.perFrameBuffer);

		mRenderTarget->bind();
		mRenderState->bind();
	}
	void TransparentPass::render(RenderArgs& args)
	{
		std::multimap<float, std::shared_ptr<GameObject>> renderMap;

		for (std::shared_ptr<GameObject> renderObject : args.transparentList)
		{
			if (renderObject->getComponent<ParticleSystem>())
			{
				// Depth sorting
				DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.camera->getInverseViewMatrix());
				DirectX::XMFLOAT4X4 globalTransform;
				renderObject->getComponent<Transform>()->getGlobalTransform(globalTransform);
				view = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
				float depth = DirectX::XMVectorGetZ(view.r[3]);
				renderMap.insert(std::pair<float, std::shared_ptr<GameObject>>(depth, renderObject));
			}
		}

		for (auto itr = renderMap.begin(); itr != renderMap.end(); itr++)
		{
			renderSceneObject(itr->second, args);
		}
	}
	void TransparentPass::renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args)
	{
		// Set the global transform for the mesh and update the shader matrix buffer
		object->getComponent<Transform>()->getGlobalTransform(args.perObject.world);
		updatePerFrameBuffer(args.perFrame, args.perFrameBuffer);

		// Bind the mesh renderer
		object->getComponent<MeshRenderer>()->bind();

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
	}
}