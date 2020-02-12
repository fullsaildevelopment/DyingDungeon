#pragma once
#include "EngineIncludes.h"
#include "EngineEvents.h"
#include "RenderPass.h"

namespace Odyssey
{
	class Sprite2DPass : public RenderPass
	{
	public:
		Sprite2DPass();
		virtual ~Sprite2DPass() = default;
	public:
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
		void onDebugEngine(DebugEngine* evnt);
	private:
		std::shared_ptr<UICanvas> mDebugCanvas;
	};
}