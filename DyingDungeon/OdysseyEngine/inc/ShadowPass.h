#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"
#include "EngineEvents.h"

namespace Odyssey
{
	class Buffer;
	class Light;
	class RenderTarget;
	class Viewport;
	class RenderState;
	class Shader;
	class RenderDevice;

	class ShadowPass : public RenderPass
	{
	public: // Constructors
		ShadowPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Light> shadowLight, int texWidth, int texHeight);
	public: // Inherited overrides
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
		void onSceneChange(SceneChangeEvent* evnt);
	private: // Helper functions
		void renderDynamicObjects(std::vector<std::shared_ptr<Entity>> dynamicList, RenderArgs& args);
		void renderStaticObjects(std::vector<std::shared_ptr<Entity>> staticList, RenderArgs& args);
		void renderSceneObject(std::shared_ptr<Entity> object, RenderArgs& args);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<Light> mShadowLight;
		std::shared_ptr<RenderTarget> mDynamicTarget;
		std::shared_ptr<RenderTarget> mStaticTarget;
		std::shared_ptr<RenderState> mRenderState;
		std::shared_ptr<Buffer> mShadowBuffer;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
		bool renderDynamic;
		bool renderStatic;
		std::string dynamicMapName;
	};
}