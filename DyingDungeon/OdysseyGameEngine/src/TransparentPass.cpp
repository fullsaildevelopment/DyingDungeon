#include "TransparentPass.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderDevice.h"

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
		args.shaderMatrix.view = args.camera->getInverseViewMatrix();
		args.shaderMatrix.proj = args.camera->getProjectionMatrix();
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

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
			itr->second->getComponent<Transform>()->getGlobalTransform(args.shaderMatrix.world);
			updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);
		}
	}
}