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
		ShadowPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, int texWidth, int texHeight);
	public: // Inherited overrides
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
		void onSceneChange(SceneChangeEvent* evnt);
	private: // Helper functions
		void renderDynamicObjects(std::vector<RenderObject*> dynamicList, RenderArgs& args);
		void renderStaticObjects(std::vector<RenderObject*> staticList, RenderArgs& args);
		void renderSceneObject(RenderObject* renderObject, RenderArgs& args);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<RenderTarget> mDynamicTarget;
		std::shared_ptr<RenderTarget> mStaticTarget;
		std::shared_ptr<RenderState> mRenderState;
		std::shared_ptr<Buffer> mShadowBuffer;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
		bool renderDynamic;
		bool renderStatic;
		int mTexWidth, mTexHeight;
		std::string dynamicMapName;
	};
}