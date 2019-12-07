#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderTarget;
	class GameObject;
	class RenderState;
	class Shader;

	class SkyboxPass : public RenderPass
	{
	public:
		SkyboxPass(const char* skyboxTexture, std::shared_ptr<RenderTarget> renderTarget);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<GameObject> mSkyBox;
		std::shared_ptr<RenderTarget> mRenderTarget;
		std::unique_ptr<RenderState> mRenderState;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
	};
}