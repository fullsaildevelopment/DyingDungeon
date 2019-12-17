#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderDevice;

	class Sprite2DPass : public RenderPass
	{
	public:
		Sprite2DPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> targetWindow);
		~Sprite2DPass() = default;
	public:
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID2D1Factory> mFactory;
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> mRenderTarget;
	};
}