#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"
#include "EngineEvents.h"

namespace Odyssey
{
	class RenderDevice;
	class SceneDX11;

	class RenderPipeline
	{
	public:
		RenderPipeline(RenderDevice& renderDevice);
		void addRenderPass(std::shared_ptr<RenderPass> renderPass);
		void render(std::shared_ptr<SceneDX11> scene);
		RenderPass* getRenderPass(int index);
	private:
		void generateRenderArgs(std::shared_ptr<SceneDX11> scene);
	private:
		RenderArgs args;
		RenderPackage mRenderPackage;
		std::vector<std::shared_ptr<RenderPass>> mRenderPasses;
		PerFrameBuffer perFrame;
		std::shared_ptr<Buffer> mPerFrameBuffer;
		PerObjectBuffer perObject;
		std::shared_ptr<Buffer> mPerObjectBuffer;
	};
}