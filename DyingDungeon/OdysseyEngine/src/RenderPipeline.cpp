#include "RenderPipeline.h"
#include "RenderDevice.h"
#include "SceneDX11.h"
#include "Entity.h"
#include "UICanvas.h"
#include "EventManager.h"
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"

namespace Odyssey
{
	RenderPipeline::RenderPipeline(RenderDevice& renderDevice)
	{
		mPerFrameBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(PerFrameBuffer)), nullptr);

		mPerObjectBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(PerObjectBuffer)), nullptr);

		args.perFrameBuffer = mPerFrameBuffer.get();
		args.perObjectBuffer = mPerObjectBuffer.get();
	}

	void RenderPipeline::addRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		mRenderPasses.emplace_back(renderPass);
	}

	void RenderPipeline::render(std::shared_ptr<SceneDX11> scene)
	{
		generateRenderArgs(scene);
		scene->getRenderPackage(mRenderPackage);
		// Iterate over each render pass in the list
		for (std::shared_ptr<RenderPass> pass : mRenderPasses)
		{
			// Check if the pass is enabled
			if (pass->isEnabled())
			{
				// Perform pre-render and render processs
				pass->preRender(args, mRenderPackage);
				pass->render(args, mRenderPackage);
			}
		}
	}

	RenderPass* RenderPipeline::getRenderPass(int index)
	{
		return mRenderPasses[index].get();
	}

	void RenderPipeline::generateRenderArgs(std::shared_ptr<SceneDX11> scene)
	{
		if (Camera* camera = scene->getMainCamera()->getComponent<Camera>())
		{
			// Get the inverse view matrix
			args.perFrame.view = camera->getInverseViewMatrix();

			// Calculate and set view proj
			DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), DirectX::XMLoadFloat4x4(&camera->getProjectionMatrix()));
			DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);
		}
	}
}