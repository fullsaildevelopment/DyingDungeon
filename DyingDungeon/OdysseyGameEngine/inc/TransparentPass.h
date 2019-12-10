#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderTarget;
	class RenderState;
	class Shader;
	class RenderDevice;

	class TransparentPass : public RenderPass
	{
	public:
		TransparentPass(RenderDevice& renderDevice, std::shared_ptr<RenderTarget> renderTarget);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		void renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<RenderTarget> mRenderTarget;
		std::shared_ptr<RenderState> mRenderState;
	};
}