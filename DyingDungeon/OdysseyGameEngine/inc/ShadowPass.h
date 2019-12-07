#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class Buffer;
	class Light;
	class RenderTarget;
	class Viewport;
	class RenderState;
	class Shader;

	class ShadowPass : public RenderPass
	{
	public: // Constructors
		ShadowPass(std::shared_ptr<Light> shadowLight, int texWidth, int texHeight);
	public: // Inherited overrides
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
	private: // Helper functions
		void renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<Light> mShadowLight;
		std::shared_ptr<RenderTarget> mRenderTarget;
		std::unique_ptr<RenderState> mRenderState;
		std::shared_ptr<Buffer> mShadowBuffer;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
	};
}