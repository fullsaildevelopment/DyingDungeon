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
		ShadowPass(int texWidth, int texHeight);
	public: // Inherited overrides
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
		void onSceneChange(SceneChangeEvent* evnt);
	private: // Helper functions
		void renderDynamicObjects(std::vector<RenderObject*> dynamicList, RenderArgs& args);
		void renderStaticObjects(std::vector<RenderObject*> staticList, RenderArgs& args);
		void renderSceneObject(RenderObject* renderObject, RenderArgs& args);
	private:
		int mDynamicTarget;
		int mStaticTarget;
		int mRenderState;
		int mShadowBuffer;
		int mVertexShader;
		int mPixelShader;
		bool renderDynamic;
		bool renderStatic;
		int mTexWidth, mTexHeight;
	};
}