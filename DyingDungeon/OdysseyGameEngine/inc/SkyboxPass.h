#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderTarget;
	class SceneObject;
	class RenderState;
	class Shader;

	class SkyboxPass : public RenderPass
	{
	public:
		SkyboxPass(std::shared_ptr<SceneObject> skybox, std::shared_ptr<RenderTarget> renderTarget);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<SceneObject> mSkyBox;
		std::shared_ptr<RenderTarget> mRenderTarget;
		std::unique_ptr<RenderState> mRenderState;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
	};
}