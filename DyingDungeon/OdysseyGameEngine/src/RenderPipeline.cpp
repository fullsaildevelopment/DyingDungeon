#include "RenderPipeline.h"
#include "RenderDevice.h"
#include "Scene.h"
#include "GameObject.h"
#include "UICanvas.h"

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

	void RenderPipeline::render(std::shared_ptr<Scene> scene)
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

	void RenderPipeline::generateRenderArgs(std::shared_ptr<Scene> scene)
	{
		args.camera = scene->mMainCamera.get();
		args.lightList = scene->getSceneLights();
		args.renderList = scene->getGameObjects();
		args.canvasList.clear();

		for (std::shared_ptr<GameObject> gameObject : scene->getGameObjects())
		{
			std::vector<UICanvas*> canvasList = gameObject->getComponents<UICanvas>();
			for (UICanvas* canvas : canvasList)
			{
				if (canvas->getActive())
				{
					args.canvasList.push_back(canvas);
				}
			}
		}
	}
}