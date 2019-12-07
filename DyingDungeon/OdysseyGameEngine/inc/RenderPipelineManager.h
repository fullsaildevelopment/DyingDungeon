#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderPass;
	struct RenderArgs;
	class RenderPipelineManager
	{
	public:
		static RenderPipelineManager& getInstance();
		~RenderPipelineManager() = default;
	private:
		RenderPipelineManager() { }
	public:
		void addPass(std::shared_ptr<RenderPass> pass);
		void render(RenderArgs args);
	private:
		std::vector<std::shared_ptr<RenderPass>> mRenderPasses;
	};
}