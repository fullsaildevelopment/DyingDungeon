#include "TransparentPass.h"
#include "RenderManager.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "SceneObject.h"

namespace Odyssey
{
	TransparentPass::TransparentPass(std::shared_ptr<RenderTarget> renderTarget)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		mRenderTarget = renderTarget;

		mRenderState = std::make_unique<RenderState>(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);
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
		std::multimap<float, std::shared_ptr<SceneObject>> renderMap;

		for (std::shared_ptr<SceneObject> renderObject : args.transparentList)
		{
			if (renderObject->hasParticleSystem())
			{
				// Depth sorting
				DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.camera->getInverseViewMatrix());
				DirectX::XMFLOAT4X4 globalTransform;
				renderObject->getGlobalTransform(globalTransform);
				view = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&globalTransform), view);
				float depth = DirectX::XMVectorGetZ(view.r[3]);
				renderMap.insert(std::pair<float, std::shared_ptr<SceneObject>>(depth, renderObject));
			}
		}

		for (auto itr = renderMap.begin(); itr != renderMap.end(); itr++)
		{
			itr->second->getGlobalTransform(args.shaderMatrix.world);
			updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

			itr->second->getParticleSystem()->Run();
		}
	}
}