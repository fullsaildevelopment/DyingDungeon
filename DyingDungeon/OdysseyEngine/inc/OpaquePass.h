#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "RenderPass.h"
#include "Light.h"

namespace Odyssey
{
	class RenderState;
	class Shader;
	class MeshRenderer;

	class OpaquePass : public RenderPass
	{
		struct SceneLighting
		{
			ShaderLight sceneLights[8];
			///////////////////////////// 16 x 8 bytes
			unsigned int numLights;
			DirectX::XMFLOAT3 camPos;
			///////////////////////////// 16 bytes
		};

	public:
		OpaquePass();
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
		void setFrustumCullEnable(bool enable);
	private:
		void updateLightingBuffer(RenderObject* renderObject, RenderArgs& args, RenderPackage& renderPackage);
		void renderSceneObject(RenderObject* renderObject, RenderArgs& args);
	private:
		int mRenderState;
		int mVertexShader;
		int mPixelShader;
		int mLightingBuffer;
		bool mFrustumCull;
	};

	struct Cmp
	{
		bool operator()(const float& v1, const float& v2) const
		{
			return (v1 < v2);
		}
	};
}