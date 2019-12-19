#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderDevice;
	class Scene;

	class RenderPipeline
	{
	public:
		RenderPipeline(RenderDevice& renderDevice);
		void addRenderPass(std::shared_ptr<RenderPass> renderPass);
		void render(std::shared_ptr<Scene> scene);
		RenderPass* getRenderPass(int index);
	private:
		void generateRenderArgs(std::shared_ptr<Scene> scene);
	private:
		RenderArgs args;
		std::vector<std::shared_ptr<RenderPass>> mRenderPasses;
		PerFrameBuffer perFrame;
		std::shared_ptr<Buffer> mPerFrameBuffer;
		PerObjectBuffer perObject;
		std::shared_ptr<Buffer> mPerObjectBuffer;
	};
}