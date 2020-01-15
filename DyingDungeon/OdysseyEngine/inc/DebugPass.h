#pragma once
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderState;
	class Shader;
	class RenderDevice;

	class DebugPass : public RenderPass
	{
	public:
		DebugPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
		std::shared_ptr<RenderState> mRenderState;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
	};
}