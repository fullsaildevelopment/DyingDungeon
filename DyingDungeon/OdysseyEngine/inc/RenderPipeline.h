#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"
#include "EngineEvents.h"

namespace Odyssey
{
	class SceneDX11;
	class RenderWindowDX11;

	class RenderPipeline
	{
	public:
		RenderPipeline();
		void addRenderPass(std::shared_ptr<RenderPass> renderPass);
		void render(RenderWindowDX11* activeWindow, SceneDX11* scene);
		RenderPass* getRenderPass(int index);
	private:
		void generateRenderArgs(RenderWindowDX11* activeWindow, SceneDX11* scene);
	private:
		RenderArgs args;
		RenderPackage mRenderPackage;
		std::vector<std::shared_ptr<RenderPass>> mRenderPasses;
		int linearSampler;
		int shadowSampler;
	};
}