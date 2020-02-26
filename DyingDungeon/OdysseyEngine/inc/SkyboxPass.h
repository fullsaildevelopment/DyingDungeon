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
		SkyboxPass();
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
	private:
		int mRenderState;
		int mVertexShader;
		int mPixelShader;
	};
}