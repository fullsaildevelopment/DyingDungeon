#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderTarget;

	class ClearRenderTargetPass : public RenderPass
	{
	public:
		ClearRenderTargetPass(const ClearRenderTargetPass& copy);
		ClearRenderTargetPass(std::shared_ptr<RenderTarget> renderTarget, bool clearDepth);
		virtual void render(RenderArgs& args);
	private:
		std::shared_ptr<RenderTarget> mRenderTarget;
		bool mClearDepth;
	};
}