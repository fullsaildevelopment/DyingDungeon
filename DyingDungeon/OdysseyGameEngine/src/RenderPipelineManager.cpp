#include "RenderPipelineManager.h"
#include "RenderPass.h"

namespace Odyssey
{
	RenderPipelineManager& RenderPipelineManager::getInstance()
	{
		static RenderPipelineManager instance;
		return instance;
	}

	void RenderPipelineManager::addPass(std::shared_ptr<RenderPass> pass)
	{
		mRenderPasses.push_back(pass);
	}

	void RenderPipelineManager::render(RenderArgs args)
	{
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
}