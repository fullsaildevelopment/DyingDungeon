#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderState;
	class Shader;
	class RenderDevice;

	class TransparentPass : public RenderPass
	{
	public:
		TransparentPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> renderWindow);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		void renderSceneObject(std::shared_ptr<Entity> object, RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
		std::shared_ptr<RenderState> mRenderState;
	};
}