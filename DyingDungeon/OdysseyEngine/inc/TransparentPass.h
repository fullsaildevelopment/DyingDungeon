#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class TransparentPass : public RenderPass
	{
	public:
		TransparentPass();
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
	private:
		unsigned int mBlendState;
	};
}