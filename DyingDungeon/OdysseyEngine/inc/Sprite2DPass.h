#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderDevice;

	class Sprite2DPass : public RenderPass
	{
	public:
		Sprite2DPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> targetWindow);
		virtual ~Sprite2DPass() = default;
	public:
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
	};
}