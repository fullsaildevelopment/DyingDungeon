#include "RenderPipeline.h"
#include "RenderDevice.h"
#include "SceneDX11.h"
#include "Entity.h"
#include "UICanvas.h"
#include "EventManager.h"

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

		// Iterate over each render pass in the list
		for (std::shared_ptr<RenderPass> pass : mRenderPasses)
		{
			// Check if the pass is enabled
			if (pass->isEnabled())
			{
				// Perform pre-render and render processs
				pass->preRender(args);
				pass->render(args);
			}
		}
	}

	RenderPass* RenderPipeline::getRenderPass(int index)
	{
		return mRenderPasses[index].get();
	}

	void RenderPipeline::generateRenderArgs(std::shared_ptr<SceneDX11> scene)
	{
		args.camera = scene->getMainCamera();
		args.lightList = scene->getSceneLights();
		args.entityList = scene->getEntities();
		args.elementList = scene->getElementList();
		args.renderList = scene->getRenderList();
		args.systemList = scene->getSystemList();
	}
}