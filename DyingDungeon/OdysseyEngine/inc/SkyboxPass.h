#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class Entity;
	class RenderState;
	class Shader;
	class RenderDevice;

	class SkyboxPass : public RenderPass
	{
	public:
		SkyboxPass(RenderDevice& renderDevice, const char* skyboxTexture, std::shared_ptr<RenderWindow> renderWindow);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<Entity> mSkyBox;
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
		std::shared_ptr<RenderState> mRenderState;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
	};
}